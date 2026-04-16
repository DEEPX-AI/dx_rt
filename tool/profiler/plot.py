import argparse
import importlib
import json
import os
import re
import sys

plt = None
mcolors = None


def ensure_dependencies():
    """Check required third-party modules and print install guidance."""
    global plt, mcolors

    required_modules = {
        "matplotlib": "matplotlib",
    }
    missing = []

    for import_name, package_name in required_modules.items():
        try:
            importlib.import_module(import_name)
        except ImportError:
            missing.append((import_name, package_name))

    if missing:
        print("[Dependency check] Missing required Python module(s):")
        for import_name, _ in missing:
            print(f"  - {import_name}")

        packages = " ".join(sorted({pkg for _, pkg in missing}))
        python_cmd = sys.executable or "python3"

        print("\nPlease install the required module(s) and run again.")
        print("Install command:")
        print(f"  {python_cmd} -m pip install {packages}")
        print("\nIf your environment blocks global installs, use one of these:")
        print(f"  {python_cmd} -m pip install --user {packages}")
        print("  python3 -m venv .venv && source .venv/bin/activate")
        print(f"  {python_cmd} -m pip install {packages}")
        return False

    import matplotlib.pyplot as _plt
    import matplotlib.colors as _mcolors
    plt = _plt
    mcolors = _mcolors
    return True


# ---------------------------------------------------------------------------
# RT flow order definitions
# ---------------------------------------------------------------------------

# Full RT pipeline order (device + common events combined)
RT_FLOW_ORDER = [
    "Buffer Wait",
    "NPU Input Format Handler",
    "PCIe Write",
    "NPU Core",
    "PCIe Read",
    "NPU Output Format Handler",
    "NPU Task",
    # CPU Task Queue Wait and cpu_N events are appended dynamically after this
]

# Events to exclude from the plot
EXCLUDED_EVENTS = {
    "Framework Response Handling Delay",
    "Service Process Wait",
}

# Number of jobs to select from the centre when --auto-select is used
AUTO_SELECT_COUNT = 200


# ---------------------------------------------------------------------------
# Event name parsing helpers
# ---------------------------------------------------------------------------

def extract_job_id(event_name):
    """Extract job ID from event name."""
    match = re.search(r"\[Job_(\d+)\]", event_name)
    return int(match.group(1)) if match else None


def extract_device_id(event_name):
    """Extract device ID from event name."""
    match = re.search(r"\[Device_(\d+)\]", event_name)
    return int(match.group(1)) if match else None


def get_event_type(event_name):
    """Return the base event type (everything before the first '[')."""
    return event_name.split("[")[0].rstrip()


def get_sub_identifier(event_name, event_type):
    """Return a sub-identifier (channel / thread) for row-level grouping.

    Examples
    -------
    PCIe Write[...](0)                -> 'ch0'
    NPU Core[...][Req_0]_2            -> 'ch2'
    NPU Output Format Handler[...](1) -> 't1'
    cpu_0[...][Req_15]_t0             -> 't0'
    """
    if event_type in ("PCIe Write", "PCIe Read"):
        m = re.search(r"\((\d+)\)$", event_name)
        if m:
            return "ch" + m.group(1)

    if event_type == "NPU Core":
        m = re.search(r"_(\d+)$", event_name)
        if m:
            return "ch" + m.group(1)

    if event_type in ("NPU Input Format Handler", "NPU Output Format Handler"):
        m = re.search(r"\((\d+)\)$", event_name)
        if m:
            return "t" + m.group(1)

    if event_type.startswith("cpu_"):
        m = re.search(r"_t(\d+)$", event_name)
        return "t" + m.group(1) if m else "t0"

    return None


# ---------------------------------------------------------------------------
# Classification & grouping
# ---------------------------------------------------------------------------

def build_per_device_events(json_data):
    """Build per-device event dicts that include both device-specific
    and common (device-independent) events.

    Returns
    -------
    per_device  : dict[int, dict]  - device_id -> {event_name: timing_list}
    cpu_task_types: list[str]       - sorted unique cpu_N base names
    """
    device_events = {}
    common_events = {}
    cpu_task_types = set()

    for event_name, timing_data in json_data.items():
        event_type = get_event_type(event_name)

        # Skip excluded events
        if event_type in EXCLUDED_EVENTS:
            continue

        device_id = extract_device_id(event_name)
        if device_id is not None:
            device_events.setdefault(device_id, {})[event_name] = timing_data
        else:
            common_events[event_name] = timing_data
            if event_type.startswith("cpu_"):
                cpu_task_types.add(event_type)

    # Merge common events into every device bucket
    for dev_id in device_events:
        device_events[dev_id].update(common_events)

    # If no device events exist but common events do, create a pseudo device -1
    if not device_events and common_events:
        device_events[-1] = common_events

    return device_events, sorted(cpu_task_types)


def _sort_key_for_group(group_key, order_list):
    """Return a tuple for sorting group_key according to order_list."""
    base = group_key.split(" (")[0] if " (" in group_key else group_key
    try:
        return (order_list.index(base), group_key)
    except ValueError:
        return (len(order_list), group_key)


def group_events(events, order_list):
    """Group raw events by (event_type, sub_id) and sort by order_list.

    Returns
    -------
    grouped    : dict[str, list]  - group_key -> list of timing dicts
    sorted_keys: list[str]        - group keys in RT-flow order
    """
    grouped = {}
    for event_name, timing_data in events.items():
        event_type = get_event_type(event_name)
        sub_id = get_sub_identifier(event_name, event_type)
        group_key = f"{event_type} ({sub_id})" if sub_id else event_type

        if group_key not in grouped:
            grouped[group_key] = []

        for timing in timing_data:
            entry = dict(timing, source_event=event_name)
            grouped[group_key].append(entry)

    sorted_keys = sorted(
        grouped.keys(),
        key=lambda k: _sort_key_for_group(k, order_list),
    )
    return grouped, sorted_keys


# ---------------------------------------------------------------------------
# Job-level helpers
# ---------------------------------------------------------------------------

def collect_all_job_ids(json_data):
    """Return a sorted list of every job ID present in json_data."""
    ids = set()
    for name in json_data:
        jid = extract_job_id(name)
        if jid is not None:
            ids.add(jid)
    return sorted(ids)


def build_job_chunks(sorted_job_ids, chunk_size):
    """Split sorted_job_ids into consecutive chunks of chunk_size."""
    return [
        sorted_job_ids[i : i + chunk_size]
        for i in range(0, len(sorted_job_ids), chunk_size)
    ]


def select_center_jobs(sorted_job_ids, count=AUTO_SELECT_COUNT):
    """Select *count* jobs centred at the midpoint of sorted_job_ids.

    This drops early fluctuation data and focuses on the stable region.
    If fewer jobs exist than *count*, all jobs are returned.
    """
    n = len(sorted_job_ids)
    if n <= count:
        return sorted_job_ids

    mid = n // 2
    half = count // 2
    start = mid - half
    end = start + count
    return sorted_job_ids[start:end]


def get_job_colors(job_ids):
    """Generate visually distinct colours for each job ID."""
    cmap = plt.colormaps["tab20"]
    n_colors = cmap.N
    golden = (1 + 5 ** 0.5) / 2
    colors = {}
    for i, jid in enumerate(sorted(job_ids)):
        idx = int(((i * golden) % 1) * n_colors)
        colors[jid] = mcolors.to_hex(cmap(idx))
    return colors


def filter_by_jobs(grouped, job_set):
    """Return a copy of grouped containing only events whose Job ID is in job_set."""
    filtered = {}
    for key, timings in grouped.items():
        kept = [
            t for t in timings
            if extract_job_id(t.get("source_event", "")) in job_set
        ]
        if kept:
            filtered[key] = kept
    return filtered


# ---------------------------------------------------------------------------
# Time-window computation
# ---------------------------------------------------------------------------

def compute_time_window(all_timings, start_ratio, end_ratio):
    """Return (window_start, window_end) in nanoseconds."""
    valid = [
        t for t in all_timings
        if t.get("start", 0) > 0 and t.get("end", 0) > 0
    ]
    if not valid:
        return 0, 0

    global_start = min(t["start"] for t in valid)
    global_end = max(t["end"] for t in valid)
    span = global_end - global_start

    return (global_start + span * start_ratio,
            global_start + span * end_ratio)


# ---------------------------------------------------------------------------
# Plotting
# ---------------------------------------------------------------------------

def _draw_duration_text(ax, plot_start, duration, actual_ns,
                        y_center, fontsize_base, is_even):
    """Draw a colour-coded duration label on a bar."""
    if actual_ns >= 1_000_000:
        text = f"{actual_ns / 1_000_000:.2f}"
        color = "darkblue"
        fs = fontsize_base
    elif actual_ns >= 1_000:
        text = f"{actual_ns / 1_000:.1f}"
        color = "red"
        fs = fontsize_base - 1
    else:
        text = str(int(actual_ns))
        color = "silver"
        fs = fontsize_base - 2

    text_y = y_center - 0.15 if is_even else y_center + 0.15
    ax.text(
        plot_start + duration / 2, text_y, text,
        ha="center", va="center", color=color, fontsize=fs,
    )


def plot_timeline(grouped, sorted_keys, job_colors,
                  window_start, window_end, title, output_path, show_text):
    """Render a single timeline image and save it."""
    interval = window_end - window_start
    if interval <= 0:
        return

    # Rows that need expanded (sub-line) layout to avoid overlap
    def _needs_expanded(key):
        base = key.split(" (")[0] if " (" in key else key
        return (base.startswith("NPU Task")
                or base.startswith("CPU Task Queue Wait"))

    y_map = {}
    y_pos = 0
    for key in sorted_keys:
        y_map[key] = y_pos
        y_pos += 2 if _needs_expanded(key) else 1
    total_rows = y_pos

    if total_rows == 0:
        return

    fig_height = max(4, 1.0 + total_rows * 0.55)
    fig, ax = plt.subplots(figsize=(15, fig_height), dpi=300)

    # Y-axis labels
    for key in sorted_keys:
        y = y_map[key]
        label_y = y + 1.0 if _needs_expanded(key) else y + 0.35
        ax.text(
            -interval * 0.008, label_y, key,
            ha="right", va="center", fontsize=9, transform=ax.transData,
        )

    # Grid lines
    for y in range(total_rows + 1):
        ax.axhline(y, color="gray", linewidth=0.5, alpha=0.2, zorder=0)

    # Draw bars
    for group_key in sorted_keys:
        y = y_map[group_key]
        timings = sorted(grouped[group_key], key=lambda t: t["start"])
        expanded = _needs_expanded(group_key)
        n_sub_lines = 10

        for i, t in enumerate(timings):
            start_ns = t["start"]
            end_ns = t["end"]
            if start_ns <= 0 or end_ns <= start_ns:
                continue
            if start_ns > window_end or end_ns < window_start:
                continue

            plot_start = max(start_ns - window_start, 0)
            plot_end = min(end_ns - window_start, interval)
            duration = plot_end - plot_start
            actual_ns = end_ns - start_ns

            job_id = extract_job_id(t.get("source_event", ""))
            color = job_colors.get(job_id, "#999999")

            if expanded:
                sub = i % n_sub_lines
                rect_y = y + 2 * sub / n_sub_lines
                rect_h = 1.6 / n_sub_lines
            else:
                rect_y = y
                rect_h = 0.7

            ax.add_patch(
                plt.Rectangle(
                    (plot_start, rect_y), duration, rect_h,
                    linewidth=0.5, edgecolor=color, facecolor=color, alpha=0.8,
                )
            )

            if show_text:
                if expanded:
                    fs = 4
                    center_y = rect_y + rect_h / 2
                else:
                    fs = 5
                    center_y = y + 0.35
                _draw_duration_text(
                    ax, plot_start, duration, actual_ns,
                    center_y, fs, i % 2 == 0,
                )

    # Legend (max 30 entries)
    if job_colors:
        handles = []
        for j, (jid, c) in enumerate(sorted(job_colors.items())):
            if j >= 30:
                handles.append(
                    plt.Rectangle(
                        (0, 0), 1, 1,
                        facecolor="white", edgecolor="black",
                        alpha=0, label="...",
                    )
                )
                break
            handles.append(
                plt.Rectangle(
                    (0, 0), 1, 1,
                    facecolor=c, edgecolor=c, alpha=0.5,
                    label=f"Job {jid}",
                )
            )
        ax.legend(
            handles=handles, loc="upper right",
            bbox_to_anchor=(1.13, 1), fontsize=6,
        )

    ax.set_title(f"{title}\n(ns=silver, us=red, ms=darkblue)", fontsize=11)
    ax.set_xlabel("Time (ns)")
    ax.set_xlim(0, interval)
    ax.set_ylim(-0.1, total_rows + 0.1)
    ax.invert_yaxis()
    ax.set_yticks(range(total_rows + 1))
    ax.set_yticklabels([""] * (total_rows + 1))

    plt.savefig(output_path, bbox_inches="tight", dpi=300)
    plt.close(fig)
    print(f"  Saved: {output_path}")


# ---------------------------------------------------------------------------
# Image generation driver
# ---------------------------------------------------------------------------

def generate_images_for_group(grouped, sorted_keys, all_job_colors,
                              job_chunks, output_dir, base_name, ext,
                              tag, start_ratio, end_ratio, show_text):
    """Generate one image per job-chunk for a given event group."""
    total_chunks = len(job_chunks)
    for chunk_idx, chunk in enumerate(job_chunks, start=1):
        job_set = set(chunk)
        filtered = filter_by_jobs(grouped, job_set)
        filtered_keys = [k for k in sorted_keys if k in filtered]
        if not filtered:
            continue

        all_timings = [t for tl in filtered.values() for t in tl]
        w_start, w_end = compute_time_window(all_timings, start_ratio, end_ratio)
        if w_end <= w_start:
            continue

        chunk_colors = {
            jid: all_job_colors[jid]
            for jid in chunk if jid in all_job_colors
        }
        if total_chunks == 1:
            fname = f"{base_name}_{tag}{ext}"
        else:
            fname = f"{base_name}_{tag}_part{chunk_idx}{ext}"
        fpath = os.path.join(output_dir, fname)
        title_tag = tag.replace("_", " ")
        title = f"{title_tag} \u2014 Jobs {chunk[0]}\u2013{chunk[-1]}"

        plot_timeline(
            filtered, filtered_keys, chunk_colors,
            w_start, w_end, title, fpath, show_text,
        )


def plot_profiler(input_file, output_base, start_ratio, end_ratio,
                  jobs_per_image, show_text, auto_select):
    """Main entry point: load JSON, classify, split, and render images."""
    print(f"Input : {input_file}")

    with open(input_file, "r") as f:
        json_data = json.load(f)

    per_device, cpu_task_types = build_per_device_events(json_data)
    all_job_ids = collect_all_job_ids(json_data)

    if auto_select:
        selected_ids = select_center_jobs(all_job_ids)
        print(f"Auto-select: {len(selected_ids)} jobs from centre "
              f"(Job {selected_ids[0]}\u2013{selected_ids[-1]} "
              f"out of {len(all_job_ids)} total)")
        job_chunks = [selected_ids]
    else:
        selected_ids = all_job_ids
        job_chunks = build_job_chunks(all_job_ids, jobs_per_image)

    all_job_colors = get_job_colors(selected_ids)

    if not job_chunks:
        print("No job data found.")
        return

    output_dir = os.path.dirname(output_base) or "."
    base_name = os.path.splitext(os.path.basename(output_base))[0]
    ext = os.path.splitext(output_base)[1] or ".png"

    full_order = RT_FLOW_ORDER + ["CPU Task Queue Wait"] + cpu_task_types

    for dev_id in sorted(per_device.keys()):
        grouped, sorted_keys = group_events(per_device[dev_id], full_order)
        tag = f"Device_{dev_id}"
        print(f"[Device {dev_id}]")
        generate_images_for_group(
            grouped, sorted_keys, all_job_colors,
            job_chunks, output_dir, base_name, ext,
            tag, start_ratio, end_ratio, show_text,
        )

    print("Done.")


# ---------------------------------------------------------------------------
# CLI
# ---------------------------------------------------------------------------

if __name__ == "__main__":
    if not ensure_dependencies():
        sys.exit(1)

    parser = argparse.ArgumentParser(
        description="Draw timeline charts from DX-RT profiler JSON data.",
    )
    parser.add_argument(
        "-i", "--input", default="profiler.json",
        help="Input profiler JSON file (default: profiler.json)",
    )
    parser.add_argument(
        "-o", "--output", default="profiler.png",
        help="Output base filename; device/job suffixes are added automatically",
    )
    parser.add_argument(
        "-s", "--start", type=float, default=0.0,
        help="Start ratio (0.0-1.0) of the time window",
    )
    parser.add_argument(
        "-e", "--end", type=float, default=1.0,
        help="End ratio (0.0-1.0) of the time window",
    )
    parser.add_argument(
        "-j", "--jobs-per-image", type=int, default=200,
        help="Max jobs per image before splitting (default: 200)",
    )
    parser.add_argument(
        "-t", "--show_text", action="store_true", default=False,
        help="Show duration text labels on bars",
    )
    parser.add_argument(
        "-a", "--auto-select", action="store_true", default=False,
        help="Auto-select 200 jobs from the stable centre region",
    )
    if len(sys.argv) == 1:
        parser.print_help()
        sys.exit(0)
    args = parser.parse_args()

    plot_profiler(
        args.input, args.output,
        args.start, args.end,
        args.jobs_per_image, args.show_text,
        args.auto_select,
    )
