"""Parse DX-RT profiler.json into structured data for visualization."""

import colorsys
import json
import re
from collections import defaultdict

import pandas as pd

# ---------------------------------------------------------------------------
# Constants
# ---------------------------------------------------------------------------

EXCLUDED_EVENTS = {
    "Framework Overhead",
    "Service Process Wait",
}

NPU_SUBGRAPH_INNER_ORDER = [
    "Buffer Pool Wait",
    "NPU Input Format Handler",
    "H2D",
    "Inference Core 0",
    "Inference Core 1",
    "Inference Core 2",
    "D2H",
    "NPU Output Format Handler",
]

CPU_SUBGRAPH_INNER_ORDER = [
    "Buffer Pool Wait",
    "CPU Dispatch Wait",
    # cpu_N execution events are appended dynamically after this
]

TAIL_EVENTS = {"NPU Task"}

# Event types that need lane splitting when they overlap
LANE_SPLIT_EVENTS = {
    "NPU Task", "CPU Dispatch Wait",
    "NPU Input Format Handler", "H2D", "D2H", "NPU Output Format Handler",
}

# Events where split lanes are labeled "buffer" (others use "thread")
BUFFER_LANE_EVENTS = {"NPU Task", "CPU Dispatch Wait"}

SG_BASE = 100      # gap between subgraphs in sort_index
INNER_MUL = 10     # gap between event types within a subgraph
USER_EVENTS_SG = "User Events"


# ---------------------------------------------------------------------------
# Event name parsing
# ---------------------------------------------------------------------------

def parse_event_name(name):
    """Parse a profiler event name into its components."""

    # User event: "User:preprocess[0]"
    if name.startswith("User:"):
        user_part = name[5:]  # strip "User:" prefix
        event_name = user_part.split("[")[0].rstrip()
        return {
            "event_type": event_name,
            "device_id": None,
            "job_id": None,
            "subgraph": None,
            "request_id": None,
            "sub_id": None,
            "is_user_event": True,
        }

    info = {
        "event_type": name.split("[")[0].rstrip(),
        "device_id": None,
        "job_id": None,
        "subgraph": None,
        "request_id": None,
        "sub_id": None,
        "is_user_event": False,
    }

    m = re.search(r"\[Device_(-?\d+)\]", name)
    if m:
        info["device_id"] = int(m.group(1))

    m = re.search(r"\[Job_(\d+)\]", name)
    if m:
        info["job_id"] = int(m.group(1))

    m = re.search(r"\[Req_(\d+)\]", name)
    if m:
        info["request_id"] = int(m.group(1))

    # Subgraph: first bracket that isn't Device_/Job_/Req_
    for bracket in re.findall(r"\[([^\]]+)\]", name):
        if not bracket.startswith(("Device_", "Job_", "Req_")):
            info["subgraph"] = bracket
            break

    # Sub-identifier (channel / thread)
    etype = info["event_type"]
    if etype in ("H2D", "D2H"):
        m = re.search(r"\((\d+)\)$", name)
        if m:
            info["sub_id"] = "ch" + m.group(1)
    elif etype.startswith("Inference Core"):
        m = re.search(r"_(\d+)$", name)
        if m:
            info["sub_id"] = "ch" + m.group(1)
    elif etype in ("NPU Input Format Handler", "NPU Output Format Handler"):
        m = re.search(r"\((\d+)\)$", name)
        if m:
            info["sub_id"] = "t" + m.group(1)
    elif etype.startswith("cpu_"):
        m = re.search(r"_t(\d+)$", name)
        info["sub_id"] = "t" + m.group(1) if m else "t0"

    return info


def _inner_order(subgraph, event_type):
    """Return inner-pipeline sort position for an event type within a subgraph."""
    if subgraph and subgraph.startswith("npu_"):
        order = NPU_SUBGRAPH_INNER_ORDER
    elif subgraph and subgraph.startswith("cpu_"):
        order = CPU_SUBGRAPH_INNER_ORDER
    else:
        order = []

    # cpu_N execution events come after the fixed order
    if event_type.startswith("cpu_"):
        return len(order)

    try:
        return order.index(event_type)
    except ValueError:
        return len(order) + 1


# ---------------------------------------------------------------------------
# Color assignment
# ---------------------------------------------------------------------------

def _make_job_colors(job_ids):
    """Assign visually distinct colors to job IDs using golden ratio."""
    colors = {}
    for j in job_ids:
        hue = (j * 0.618033988749895) % 1.0
        r, g, b = colorsys.hsv_to_rgb(hue, 0.65, 0.88)
        colors[j] = f"rgb({int(r*255)},{int(g*255)},{int(b*255)})"
    return colors


def _make_user_event_colors(event_names):
    """Assign visually distinct colors to user event names.

    Uses golden ratio hue spacing with offset 0.7 to avoid overlap
    with job colors (which start near hue 0.0).
    """
    colors = {}
    for i, name in enumerate(sorted(event_names)):
        hue = (0.7 + i * 0.618033988749895) % 1.0
        r, g, b = colorsys.hsv_to_rgb(hue, 0.55, 0.85)
        colors[name] = f"rgb({int(r*255)},{int(g*255)},{int(b*255)})"
    return colors


# ---------------------------------------------------------------------------
# Duration formatting
# ---------------------------------------------------------------------------

# ---------------------------------------------------------------------------
# Utilization / Bottleneck analysis
# ---------------------------------------------------------------------------

def _merged_busy_ns(starts, ends):
    """Compute total busy time after merging overlapping intervals."""
    intervals = sorted(zip(starts, ends))
    if not intervals:
        return 0
    total = 0
    cur_s, cur_e = intervals[0]
    for s, e in intervals[1:]:
        if s <= cur_e:
            cur_e = max(cur_e, e)
        else:
            total += cur_e - cur_s
            cur_s, cur_e = s, e
    total += cur_e - cur_s
    return total


def compute_utilization(df, device_id):
    """Compute per-channel utilization for a single device.

    Returns a list of dicts (rows) sorted by pipeline order, with columns:
        Stage, Channels, Saturation (%), Avg Util (%)
    Excludes derivative stages (NPU Task, CPU Task Queue Wait) that are
    not independent bottleneck indicators.
    """
    # Stages to exclude from per-device bottleneck analysis:
    # - NPU Task: aggregate of NPU sub-stages, not an independent metric
    # - CPU Dispatch Wait: shown in shared CPU table instead
    # - CPU subgraph stages (cpu_N, Buffer Pool Wait[cpu_*]): shared across devices,
    #   shown separately via compute_shared_utilization()
    excluded_stages = {"NPU Task", "CPU Dispatch Wait"}

    dev_df = df[df["device_id"] == device_id]
    if dev_df.empty:
        return []

    wall_start = dev_df["start_ns"].min()
    wall_end = dev_df["end_ns"].max()
    wall_ns = wall_end - wall_start
    if wall_ns <= 0:
        return []

    # Group by (event_type, sub_id) — sub_id is the channel/thread
    # For events without sub_id, treat as single channel
    rows = []
    stage_order = (
        dev_df[["event_type", "sort_index"]]
        .drop_duplicates()
        .groupby("event_type")["sort_index"]
        .min()
        .to_dict()
    )

    for etype in sorted(stage_order, key=lambda e: stage_order[e]):
        if etype in excluded_stages:
            continue
        et_df = dev_df[dev_df["event_type"] == etype]
        # Skip CPU subgraph stages — they are shared resources shown separately
        if not et_df.empty:
            sample_sg = et_df["subgraph"].iloc[0]
            if _is_cpu_subgraph_stage(etype, sample_sg):
                continue
        # Determine channels
        channels = et_df["sub_id"].fillna("—").unique()
        channel_utils = []

        for ch in sorted(channels):
            if ch == "—":
                ch_df = et_df[et_df["sub_id"].isna() | (et_df["sub_id"] == "")]
            else:
                ch_df = et_df[et_df["sub_id"] == ch]
            if ch_df.empty:
                continue
            busy = _merged_busy_ns(
                ch_df["start_ns"].values, ch_df["end_ns"].values
            )
            util = round(busy / wall_ns * 100, 1)
            channel_utils.append((ch, util))

        if not channel_utils:
            continue

        saturation = max(u for _, u in channel_utils)
        n_ch = len(channel_utils)
        avg_util = round(sum(u for _, u in channel_utils) / n_ch, 1)

        rows.append({
            "Stage": etype,
            "Channels": n_ch,
            "Saturation (%)": saturation,
            "Avg Util (%)": avg_util,
            "_order": stage_order[etype],
        })

    rows.sort(key=lambda r: r["_order"])
    for r in rows:
        del r["_order"]

    return rows, round(wall_ns / 1e6, 1)  # rows, wall_time_ms


def _is_cpu_subgraph_stage(event_type, subgraph):
    """Return True if this event belongs to a CPU subgraph (shared resource)."""
    if subgraph and subgraph.startswith("cpu_"):
        return True
    if event_type.startswith("cpu_"):
        return True
    return False


def compute_shared_utilization(df):
    """Compute utilization for shared CPU resources across ALL devices.

    CPU threads/queues are shared by all devices, so per-device measurement
    underestimates the real utilization.  This function merges intervals from
    all devices to show the true picture.

    Returns (rows, wall_time_ms) or None if no CPU stages exist.
    """
    excluded_stages = {"NPU Task"}

    # Collect only CPU-subgraph stages
    cpu_mask = df["subgraph"].fillna("").str.startswith("cpu_") | \
               df["event_type"].str.startswith("cpu_")
    cpu_df = df[cpu_mask].copy()
    if cpu_df.empty:
        return None

    # Wall time = global span of ALL events (same reference as per-device)
    wall_start = df["start_ns"].min()
    wall_end = df["end_ns"].max()
    wall_ns = wall_end - wall_start
    if wall_ns <= 0:
        return None

    stage_order = (
        cpu_df[["event_type", "sort_index"]]
        .drop_duplicates()
        .groupby("event_type")["sort_index"]
        .min()
        .to_dict()
    )

    rows = []
    for etype in sorted(stage_order, key=lambda e: stage_order[e]):
        if etype in excluded_stages:
            continue
        et_df = cpu_df[cpu_df["event_type"] == etype]
        channels = et_df["sub_id"].fillna("\u2014").unique()
        channel_utils = []

        for ch in sorted(channels):
            if ch == "\u2014":
                ch_df = et_df[et_df["sub_id"].isna() | (et_df["sub_id"] == "")]
            else:
                ch_df = et_df[et_df["sub_id"] == ch]
            if ch_df.empty:
                continue
            busy = _merged_busy_ns(
                ch_df["start_ns"].values, ch_df["end_ns"].values
            )
            util = round(busy / wall_ns * 100, 1)
            channel_utils.append((ch, util))

        if not channel_utils:
            continue

        saturation = max(u for _, u in channel_utils)
        n_ch = len(channel_utils)
        avg_util = round(sum(u for _, u in channel_utils) / n_ch, 1)

        rows.append({
            "Stage": etype,
            "Channels": n_ch,
            "Saturation (%)": saturation,
            "Avg Util (%)": avg_util,
            "_order": stage_order[etype],
        })

    rows.sort(key=lambda r: r["_order"])
    for r in rows:
        del r["_order"]

    if not rows:
        return None
    return rows, round(wall_ns / 1e6, 1)


def format_duration(ns):
    """Format nanosecond duration for display."""
    if ns < 1000:
        return f"{int(ns)} ns"
    us = ns / 1000
    if us < 1000:
        return f"{us:.1f} µs"
    ms = us / 1000
    return f"{ms:.2f} ms"


def _extract_event_type(track_name):
    """Extract the event type from a track name.

    Examples:
        'npu_0 / H2D (ch0)'        -> 'H2D'
        'cpu_0 / Buffer Pool Wait' -> 'Buffer Pool Wait'
        'npu_0 / Inference Core 0 (Device 0)' -> 'Inference'
    """
    if " / " in track_name:
        part = track_name.split(" / ", 1)[1]
    else:
        part = track_name
    # Strip channel/thread/device suffix like ' (ch0)', ' (Device 0)'
    idx = part.find(" (")
    if idx != -1:
        part = part[:idx]
    part = part.strip()
    # Merge per-core Inference events into a single "Inference" label
    if part.startswith("Inference Core"):
        return "Inference"
    return part


def compute_transition_latency(flow_data, job_ids=None, device_filter=None,
                               job_to_device=None):
    """Compute transition gap statistics between consecutive pipeline stages.

    Parameters
    ----------
    flow_data : dict[int, list[dict]]
        Per-job flow arrows from load_profiler().
    job_ids : list[int] | None
        If given, restrict to these jobs.  Otherwise use all jobs in flow_data.
    device_filter : list[int] | None
        If given, restrict to jobs on these devices.
    job_to_device : dict[int, int] | None
        Mapping from job_id to device_id (needed when device_filter is used).

    Returns
    -------
    list[dict]
        Rows sorted by pipeline order with columns:
            Transition, Count, Avg (µs), Min (µs), Max (µs), Median (µs)
    """
    from collections import defaultdict
    import statistics

    gaps = defaultdict(list)  # transition_label -> [gap_us, ...]
    order = {}               # transition_label -> first-seen index
    idx_counter = 0

    target_jobs = job_ids if job_ids else list(flow_data.keys())

    for jid in target_jobs:
        if device_filter and job_to_device:
            if job_to_device.get(jid) not in device_filter:
                continue
        flows = flow_data.get(jid, [])
        for flow in flows:
            et_from = _extract_event_type(flow["track_from"])
            et_to = _extract_event_type(flow["track_to"])
            label = f"{et_from} \u2192 {et_to}"
            gap_us = flow["start_us"] - flow["end_us"]
            gaps[label].append(gap_us)
            if label not in order:
                order[label] = idx_counter
                idx_counter += 1

    if not gaps:
        return []

    rows = []
    for label in sorted(order, key=lambda k: order[k]):
        vals = gaps[label]
        rows.append({
            "Transition": label,
            "Count": len(vals),
            "Avg (\u00b5s)": round(statistics.mean(vals), 1),
            "Min (\u00b5s)": round(min(vals), 1),
            "Max (\u00b5s)": round(max(vals), 1),
            "Median (\u00b5s)": round(statistics.median(vals), 1),
        })
    return rows


# ---------------------------------------------------------------------------
# Main loader
# ---------------------------------------------------------------------------

def load_profiler(filepath):
    """Load and parse profiler.json.

    Returns a dict with:
        df            : DataFrame with all event data
        track_order   : dict[int, list[str]] — track names per device in order
        device_ids    : list[int]
        job_ids       : list[int]
        job_colors    : dict[int, str]
        job_to_device : dict[int, int]
        flow_data     : dict[int, list[dict]] — flow arrows per job
        global_start_ns : int
        subgraph_order  : list[str]
        filepath        : str
    """
    with open(filepath) as f:
        raw = json.load(f)

    # --- Pass 1: parse all events into rows ---
    rows = []
    job_to_device = {}
    sg_first_start = {}

    for name, timings in raw.items():
        info = parse_event_name(name)

        if info.get("is_user_event"):
            info["subgraph"] = USER_EVENTS_SG

        if info["event_type"] in EXCLUDED_EVENTS:
            continue

        # Track job -> device mapping
        if info["device_id"] is not None and info["job_id"] is not None:
            job_to_device[info["job_id"]] = info["device_id"]

        # Track subgraph first-start for ordering
        if info["subgraph"] and info["event_type"] not in TAIL_EVENTS:
            for t in timings:
                if t["start"] > 0:
                    prev = sg_first_start.get(info["subgraph"], float("inf"))
                    sg_first_start[info["subgraph"]] = min(prev, t["start"])

        for t in timings:
            if t["start"] <= 0 or t["end"] <= t["start"]:
                continue
            rows.append({
                "raw_name": name,
                "event_type": info["event_type"],
                "device_id": info["device_id"],
                "job_id": info["job_id"],
                "subgraph": info["subgraph"],
                "request_id": info["request_id"],
                "sub_id": info["sub_id"],
                "start_ns": t["start"],
                "end_ns": t["end"],
                "is_user_event": info.get("is_user_event", False),
            })

    df = pd.DataFrame(rows)

    # --- Resolve device_id for CPU events ---
    mask_no_dev = df["device_id"].isna() & ~df["is_user_event"]
    df.loc[mask_no_dev, "device_id"] = (
        df.loc[mask_no_dev, "job_id"].map(job_to_device)
    )

    # --- Derived columns ---
    df["duration_ns"] = df["end_ns"] - df["start_ns"]
    global_start = int(df["start_ns"].min())
    df["start_us"] = (df["start_ns"] - global_start) / 1000.0
    df["end_us"] = (df["end_ns"] - global_start) / 1000.0
    df["duration_us"] = df["duration_ns"] / 1000.0

    # --- Subgraph order ---
    sg_order = sorted(sg_first_start.keys(), key=lambda s: sg_first_start[s])
    has_user_events = df["is_user_event"].any()
    if has_user_events:
        sg_order = [USER_EVENTS_SG] + [s for s in sg_order if s != USER_EVENTS_SG]

    # --- Track name (vectorized) ---
    mask_tail = df["event_type"].isin(TAIL_EVENTS)
    mask_sub = df["sub_id"].notna() & (df["sub_id"] != "")
    mask_user = df["is_user_event"]

    # Determine which subgraphs span multiple devices
    sg_dev_counts = (
        df[~mask_tail & ~mask_user]
        .groupby("subgraph")["device_id"]
        .nunique()
    )
    multi_dev_sgs = set(sg_dev_counts[sg_dev_counts > 1].index)

    # Build device suffix column: " (Device N)" for all NPU-side events
    # in multi-device subgraphs.  Every pipeline stage (H2D, D2H, NFH,
    # Inference Core, …) is device-specific, so device separation must come
    # before thread/lane splitting.
    df["_dev_suffix"] = ""
    mask_has_dev = ~mask_tail & ~mask_user & df["device_id"].notna()
    mask_multi_dev = mask_has_dev & df["subgraph"].isin(multi_dev_sgs)
    df.loc[mask_multi_dev, "_dev_suffix"] = (
        " (Device " + df.loc[mask_multi_dev, "device_id"].astype(int).astype(str) + ")"
    )

    df["track"] = ""
    df.loc[mask_tail & ~mask_user, "track"] = df.loc[mask_tail & ~mask_user, "event_type"]
    df.loc[~mask_tail & ~mask_user & mask_sub, "track"] = (
        df.loc[~mask_tail & ~mask_user & mask_sub, "subgraph"].astype(str) + " / " +
        df.loc[~mask_tail & ~mask_user & mask_sub, "event_type"] + " (" +
        df.loc[~mask_tail & ~mask_user & mask_sub, "sub_id"].astype(str) + ")" +
        df.loc[~mask_tail & ~mask_user & mask_sub, "_dev_suffix"]
    )
    df.loc[~mask_tail & ~mask_user & ~mask_sub, "track"] = (
        df.loc[~mask_tail & ~mask_user & ~mask_sub, "subgraph"].astype(str) + " / " +
        df.loc[~mask_tail & ~mask_user & ~mask_sub, "event_type"] +
        df.loc[~mask_tail & ~mask_user & ~mask_sub, "_dev_suffix"]
    )
    df.loc[mask_user, "track"] = (
        USER_EVENTS_SG + " / " + df.loc[mask_user, "event_type"]
    )

    df.drop(columns=["_dev_suffix"], inplace=True)

    # --- Sort index ---
    sg_map = {sg: i for i, sg in enumerate(sg_order)}

    df["_sg_idx"] = df["subgraph"].map(sg_map).fillna(len(sg_order))
    df.loc[mask_user, "_sg_idx"] = -1
    df.loc[mask_tail, "_sg_idx"] = len(sg_order) + 1

    df["_inner_idx"] = df.apply(
        lambda r: _inner_order(r["subgraph"], r["event_type"]), axis=1
    )
    if has_user_events:
        user_event_names = sorted(df.loc[mask_user, "event_type"].unique())
        user_name_order = {n: i for i, n in enumerate(user_event_names)}
        df.loc[mask_user, "_inner_idx"] = (
            df.loc[mask_user, "event_type"].map(user_name_order)
        )

    # For multi-device subgraphs, add device offset to sort_index so that
    # same-event-type tracks are grouped by device (Device 0, then Device 1).
    df["_dev_offset"] = 0

    # Non-Inference events: simple device_id offset
    mask_multi_non_inf = (
        ~mask_tail
        & df["subgraph"].isin(multi_dev_sgs)
        & df["device_id"].notna()
        & ~df["event_type"].str.startswith("Inference Core")
    )
    if mask_multi_non_inf.any():
        df.loc[mask_multi_non_inf, "_dev_offset"] = (
            df.loc[mask_multi_non_inf, "device_id"].fillna(0).astype(int)
        )

    # Inference Core events: device*n_cores + core offset (groups all cores
    # of the same device together: Core 0,1,2 Dev 0 → Core 0,1,2 Dev 1).
    mask_inf_sort = df["event_type"].str.startswith("Inference Core") & ~mask_tail
    if mask_inf_sort.any():
        inf_base = df.loc[mask_inf_sort, "_inner_idx"].min()
        core_num = df.loc[mask_inf_sort, "_inner_idx"] - inf_base
        dev_id_filled = df.loc[mask_inf_sort, "device_id"].fillna(0).astype(int)
        n_cores = int(core_num.max()) + 1
        df.loc[mask_inf_sort, "_inner_idx"] = inf_base
        df.loc[mask_inf_sort, "_dev_offset"] = dev_id_filled * n_cores + core_num

    sub_nums = df["sub_id"].str.extract(r"(\d+)", expand=False)
    df["_sub_num"] = pd.to_numeric(sub_nums, errors="coerce").fillna(0).astype(int)

    df["sort_index"] = (
        df["_sg_idx"] * SG_BASE +
        df["_inner_idx"] * INNER_MUL +
        df["_dev_offset"] +
        df["_sub_num"]
    )

    df.drop(columns=["_sg_idx", "_inner_idx", "_sub_num", "_dev_offset"], inplace=True)

    # --- Track order per subgraph ---
    track_order = {}
    for sg in sg_order:
        sg_df = df[df["subgraph"] == sg]
        if sg_df.empty:
            continue
        # Sort by pipeline stage (sort_index) first, then device_id within
        # the same stage — so Inference Core tracks group by stage, not device
        sg_tracks = (
            sg_df[["track", "sort_index", "device_id"]]
            .drop_duplicates(subset=["track"])
            .sort_values(["sort_index", "device_id"])
        )
        track_order[sg] = sg_tracks["track"].tolist()

    # TAIL_EVENTS (e.g. NPU Task) are not prefixed with subgraph in their
    # track name, but they DO have a subgraph column.  Add them at the end
    # of their respective subgraph's track list.
    tail_df = df[df["event_type"].isin(TAIL_EVENTS)]
    for sg in sg_order:
        sg_tail = tail_df[tail_df["subgraph"] == sg]
        if sg_tail.empty:
            continue
        tail_tracks = (
            sg_tail[["track", "sort_index"]]
            .drop_duplicates(subset=["track"])
            .sort_values("sort_index")
        )
        existing = track_order.get(sg, [])
        for t in tail_tracks["track"].tolist():
            if t not in existing:
                existing.append(t)
        track_order[sg] = existing

    # --- Job colors ---
    job_ids = sorted(df["job_id"].dropna().unique().astype(int))
    job_colors = _make_job_colors(job_ids)
    df["color"] = df["job_id"].map(job_colors).fillna("rgb(180,180,180)")
    if has_user_events:
        user_event_names = sorted(df.loc[mask_user, "event_type"].unique())
        user_colors = _make_user_event_colors(user_event_names)
        df.loc[mask_user, "color"] = df.loc[mask_user, "event_type"].map(user_colors)

    # --- Hover text ---
    def _hover(row):
        dur_str = format_duration(row["duration_ns"])
        if row.get("is_user_event"):
            return f"<b>{row['event_type']}</b><br>Duration: {dur_str}"
        lines = [
            f"<b>{row['event_type']}</b>",
            f"Job: {int(row['job_id'])}" if pd.notna(row['job_id']) else "Job: N/A",
            f"Duration: {dur_str}",
            f"Subgraph: {row['subgraph']}",
        ]
        if row["sub_id"] and pd.notna(row["sub_id"]):
            lines.append(f"Channel/Thread: {row['sub_id']}")
        if pd.notna(row["request_id"]):
            lines.append(f"Request: {int(row['request_id'])}")
        if pd.notna(row["device_id"]):
            lines.append(f"Device: {int(row['device_id'])}")
        return "<br>".join(lines)

    df["hover"] = df.apply(_hover, axis=1)

    device_ids = sorted(df["device_id"].dropna().unique().astype(int))

    # --- Lane splitting for overlapping events (greedy interval coloring) ---
    for split_etype in LANE_SPLIT_EVENTS:
        etype_mask = df["event_type"] == split_etype
        if not etype_mask.any():
            continue
        for dev_id in device_ids:
            dev_mask = etype_mask & (df["device_id"] == dev_id)
            if not dev_mask.any():
                continue

            # For TAIL_EVENTS (e.g. NPU Task): group per device (no subgraph prefix)
            # For others: group per track name so that sub_id (channel/thread)
            #   differences are respected — e.g. Inference (ch0) and Inference (ch1)
            #   get independent lane assignments.
            if split_etype in TAIL_EVENTS:
                groups = [(None, df.loc[dev_mask])]
            else:
                groups = [
                    (track_name, df.loc[dev_mask & (df["track"] == track_name)])
                    for track_name in df.loc[dev_mask, "track"].unique()
                ]

            for base_name, grp_df in groups:
                grp_df = grp_df.sort_values("start_ns")
                if grp_df.empty:
                    continue

                # Assign lanes — greedy: pick first lane whose last event ends
                # before this event starts
                lane_ends = []   # lane_ends[i] = end_ns of last event in lane i
                lane_assignments = []
                for _, row in grp_df.iterrows():
                    placed = False
                    for li, le in enumerate(lane_ends):
                        if row["start_ns"] >= le:
                            lane_ends[li] = row["end_ns"]
                            lane_assignments.append(li)
                            placed = True
                            break
                    if not placed:
                        lane_assignments.append(len(lane_ends))
                        lane_ends.append(row["end_ns"])

                n_lanes = len(lane_ends)
                # base_name for TAIL_EVENTS
                if split_etype in TAIL_EVENTS:
                    base_name = split_etype
                    if len(device_ids) > 1:
                        base_name = f"{split_etype} (Device {dev_id})"

                # Update track names for multi-lane
                # Buffer-type events use "buffer", others use "thread"
                lane_label = "buffer" if split_etype in BUFFER_LANE_EVENTS else "thread"
                new_tracks = []
                for lane_idx in lane_assignments:
                    if n_lanes > 1:
                        new_tracks.append(f"{base_name} ({lane_label} {lane_idx})")
                    else:
                        new_tracks.append(base_name)
                df.loc[grp_df.index, "track"] = new_tracks

                # Update sort_index so lanes are consecutive
                base_sort = df.loc[grp_df.index, "sort_index"].iloc[0]
                new_sorts = [base_sort + li for li in lane_assignments]
                df.loc[grp_df.index, "sort_index"] = new_sorts

    # Rebuild track_order with updated lanes
    track_order = {}
    for sg in sg_order:
        sg_df = df[df["subgraph"] == sg]
        if sg_df.empty:
            continue
        sg_tracks = (
            sg_df[["track", "sort_index", "device_id"]]
            .drop_duplicates(subset=["track"])
            .sort_values(["sort_index", "device_id"])
        )
        track_order[sg] = sg_tracks["track"].tolist()

    tail_df_rebuild = df[df["event_type"].isin(TAIL_EVENTS)]
    for sg in sg_order:
        sg_tail = tail_df_rebuild[tail_df_rebuild["subgraph"] == sg]
        if sg_tail.empty:
            continue
        tail_tracks = (
            sg_tail[["track", "sort_index"]]
            .drop_duplicates(subset=["track"])
            .sort_values("sort_index")
        )
        existing = track_order.get(sg, [])
        for t in tail_tracks["track"].tolist():
            if t not in existing:
                existing.append(t)
        track_order[sg] = existing
    if has_user_events:
        user_df = df[df["is_user_event"]]
        user_tracks = (
            user_df[["track", "sort_index"]]
            .drop_duplicates(subset=["track"])
            .sort_values("sort_index")
        )
        track_order[USER_EVENTS_SG] = user_tracks["track"].tolist()

    # --- Flow data per job (built AFTER lane splitting so track names are final) ---
    flow_df = df[~df["event_type"].isin(TAIL_EVENTS)].copy()
    flow_data = {}
    for jid, grp in flow_df.groupby("job_id"):
        grp_sorted = grp.sort_values("start_ns")
        flows = []
        prev = None
        for _, row in grp_sorted.iterrows():
            if prev is not None:
                flows.append({
                    "track_from": prev["track"],
                    "end_us": prev["end_us"],
                    "track_to": row["track"],
                    "start_us": row["start_us"],
                    "device_id": int(row["device_id"]),
                    "subgraph_from": prev["subgraph"],
                    "subgraph_to": row["subgraph"],
                })
            prev = row
        flow_data[int(jid)] = flows

    return {
        "df": df,
        "track_order": track_order,
        "device_ids": device_ids,
        "job_ids": job_ids,
        "job_colors": job_colors,
        "job_to_device": job_to_device,
        "flow_data": flow_data,
        "global_start_ns": global_start,
        "subgraph_order": sg_order,
        "filepath": filepath,
    }
