import argparse
import json
import matplotlib.pyplot as plt
import matplotlib.colors as mcolors
from datetime import datetime, timedelta
import re
import math

def extract_job_id(event_name):
    """Extract job ID from event name like 'NPU Input Preprocess[Job_3][Task_X][Req_42]'"""
    match = re.search(r'\[Job_(\d+)\]', event_name)
    if match:
        return int(match.group(1))
    return None
def get_job_colors(json_data):
    """
    Generates a color mapping for different job IDs, ensuring good contrast
    between colors, especially for neighboring IDs, by cycling through a distinct colormap
    with a strategic indexing to maximize contrast for adjacent IDs.
    """
    job_ids = set()
    for event_name in json_data.keys():
        job_id = extract_job_id(event_name)
        if job_id is not None:
            job_ids.add(job_id)

    # Sort job IDs to ensure consistent color assignment
    sorted_job_ids = sorted(list(job_ids))

    # Use a colormap designed for categorical data
    cmap = plt.colormaps['tab20']
    num_colors_in_cmap = cmap.N # Number of colors in tab20 is 20

    job_colors = {}

    golden_ratio_conjugate = (1 + 5**0.5) / 2
    
    for i, job_id in enumerate(sorted_job_ids):
        # Calculate a float index using the golden ratio conjugate
        # This spreads the indices across the range [0, 1) fairly evenly.
        float_index = (i * golden_ratio_conjugate) % 1
        
        # Scale the float_index to the number of colors in the colormap
        color_index = int(float_index * num_colors_in_cmap)
        
        # Get the color from the colormap (RGBA) and convert it to hex
        rgba_color = cmap(color_index)
        hex_color = mcolors.to_hex(rgba_color) 
        
        job_colors[job_id] = hex_color

    return job_colors



def group_events_by_type(json_data):
    """Group events by their type and sub-identifier (Separate by channel/core/thread, ignore job/req)
    - PCIe Write/Read: Group by channel number (ex: PCIe Write(1))
    - NPU Core: Group by core number (ex: NPU Core_2)
    - NPU Task: Group by task name (ex: NPU Task(npu_0))
    - CPU Task: Group by thread (ex: CPU Task(cpu_0) - (t0))
    - NPU Output Postprocess: Group by thread (ex: NPU Output Postprocess - (tN))
    - Service Process Wait: Group by core number (ex: Service Process Wait_1)
    """
    grouped_data = {}
    for event_name, timing_data in json_data.items():
        # PCIe Write/Read: Group by channel number
        m = re.match(r'(PCIe (Write|Read))\[Job_\d+\]\[.*?\]\[Req_\d+\]\((\d+)\)', event_name)
        if m:
            group_key = f"{m.group(1)}({m.group(3)})"
        # NPU Core: Group by core number
        elif re.match(r'NPU Core\[Job_\d+\]\[.*?\]\[Req_\d+\]_(\d+)', event_name):
            m = re.match(r'NPU Core\[Job_\d+\]\[.*?\]\[Req_\d+\]_(\d+)', event_name)
            group_key = f"NPU Core_{m.group(1)}"
        # Service Process Wait: Group by core number
        elif re.match(r'Service Process Wait\[Job_\d+\]\[.*?\]\[Req_\d+\]_(\d+)', event_name):
            m = re.match(r'Service Process Wait\[Job_\d+\]\[.*?\]\[Req_\d+\]_(\d+)', event_name)
            group_key = f"Service Process Wait_{m.group(1)}"
        # NPU Task: Group by task name
        elif re.match(r'NPU Task\[Job_\d+\]\[(.*?)\]\[Req_\d+\]', event_name):
            m = re.match(r'NPU Task\[Job_\d+\]\[(.*?)\]\[Req_\d+\]', event_name)
            group_key = f"NPU Task({m.group(1)})"
        # CPU Task: Group by thread
        elif re.match(r'cpu_\d+\[Job_\d+\]\[(cpu_\d+)\]\[Req_\d+\]_t(\d+)', event_name):
            m = re.match(r'cpu_\d+\[Job_\d+\]\[(cpu_\d+)\]\[Req_\d+\]_t(\d+)', event_name)
            group_key = f"CPU Task({m.group(1)}) - (t{m.group(2)})"
        # NPU Output Postprocess: Group by thread
        elif re.match(r'NPU Output Format Handler\[Job_\d+\]\[.*?\]\[Req_\d+\]\((\d+)\)', event_name):
            m = re.match(r'NPU Output Format Handler\[Job_\d+\]\[.*?\]\[Req_\d+\]\((\d+)\)', event_name)
            group_key = f"NPU Output Formatting - (t{m.group(1)})"
        # CPU Task(name is CPU Task[...]) also separate by thread
        elif event_name.startswith("CPU Task["):
            group_key = event_name
        # NPU Input/Output Format Handler: ignore job/req, only type
        elif event_name.startswith("NPU Input Format Handler["):
            group_key = "NPU Input Formatting"
        elif event_name.startswith("Input Request["):
            group_key = event_name
        elif event_name.startswith("/dev/"):
            group_key = event_name
        else:
            group_key = event_name
        if group_key not in grouped_data:
            grouped_data[group_key] = []
        for timing in timing_data:
            timing_with_source = timing.copy()
            timing_with_source['source_event'] = event_name
            grouped_data[group_key].append(timing_with_source)
    return grouped_data

def sort_events_by_priority(grouped_data):
    """Sort grouped events to show in the specified order"""
    # Specified priority
    priority = [
        'CPU Task',
        'NPU Task',
        'NPU Input Formatting',
        'PCIe Write',
        'NPU Core',
        'PCIe Read',
        'NPU Output Formatting',
        'Service Process Wait',  # Don't display separately, just collect data
    ]
    # Match the actual name pattern of each group
    def get_priority(g):
        if g.startswith('CPU Task('):
            return (0, g)
        if g.startswith('NPU Task('):
            return (1, g)
        if g == 'NPU Input Formatting':
            return (2, g)
        if g.startswith('PCIe Write'):
            return (3, g)
        if g.startswith('NPU Core'):
            return (4, g)
        if g.startswith('PCIe Read'):
            return (5, g)
        if g == 'NPU Output Formatting':
            return (6, g)
        if g.startswith('Service Process Wait'):
            return (99, g)  # Put at the end, won't be displayed separately
        return (98, g)
    return sorted(grouped_data.keys(), key=get_priority)

def plot(input, output, start_ratio, end_ratio, show_gap, hide_text):
    print("Input : ", input)
    print("Output : ", output)
    # Read profiler json file
    with open(input, "r") as json_file:
        json_data = json.load(json_file)

    # Group events by type instead of keeping them separate
    grouped_data = group_events_by_type(json_data)
    
    # Sort grouped events to prioritize task-level data at the top
    sorted_group_names = sort_events_by_priority(grouped_data)
    
    # Filter out Service Process Wait from display (we'll use it for overlay)
    service_wait_groups = {k: v for k, v in grouped_data.items() if k.startswith('Service Process Wait')}
    display_group_names = [g for g in sorted_group_names if not g.startswith('Service Process Wait')]

    # Define entire interval

    non_service_timings = []
    for group_name, timing_list in grouped_data.items():
        if not group_name.startswith("Service Process Wait"):
            non_service_timings.extend(timing_list)
    min_start = min(event_timing["start"] for event_timing in non_service_timings)
    max_end = max(event_timing["end"] for event_timing in non_service_timings)
    interval_all = max_end - min_start

    # Calculate actual time window
    interval_start = min_start + interval_all * start_ratio
    interval_end = min_start + interval_all * end_ratio
    interval = interval_end - interval_start

    # Setup plot
    fig, ax = plt.subplots(figsize=(15, 8), dpi=300)

    # Get job ID based color mapping
    job_colors = get_job_colors(json_data)

    # Visualize by grouped event type
    colors = list(mcolors.TABLEAU_COLORS.values())
    multi = int(len(display_group_names) / len(colors) + 1)
    colors *= multi

    # Color guide for time units
    unit_color_legend = '(us: silver, ms: red, s: darkblue)'

    # NPU Task(npu_N) y-axis area is allocated 2 times
    npu_task_indices = [i for i, g in enumerate(display_group_names) if g.startswith("NPU Task(")]
    npu_task_count = len(npu_task_indices)
    n_groups = len(display_group_names) + npu_task_count 
    plt.yticks(range(n_groups), ['' for _ in range(n_groups)])
    y_map = {}
    y_shift = 0
    for idx, label in enumerate(display_group_names):
        if label.startswith("NPU Task("):
            y = idx + y_shift
            y_map[label] = y
            y_shift += 1  
            ax.text(-interval * 0.01, y + 1, label, ha='right', va='center', fontsize=10, transform=ax.transData)
        else:
            y = idx + y_shift
            y_map[label] = y
            ax.text(-interval * 0.01, y + 0.35, label, ha='right', va='center', fontsize=10, transform=ax.transData)
    plt.ylim(-0.1, n_groups)
    ax.invert_yaxis()

    for y in range(n_groups):
        ax.axhline(y, color='gray', linewidth=0.5, alpha=0.2, zorder=0)

    for idx, group_name in enumerate(display_group_names):
        y = y_map[group_name]
        timing_data = grouped_data[group_name]
        sorted_timings = sorted(timing_data, key=lambda x: x["start"])
        # NPU Core only apply shift logic
        if group_name.startswith("NPU Core"):
            n = len(sorted_timings)
            adjusted = [dict(start=et["start"], end=et["end"], original_start=et["start"], source_event=et.get("source_event", "")) for et in sorted_timings]
            for i in range(n - 2, -1, -1):
                duration = adjusted[i]["end"] - adjusted[i]["start"]
                if adjusted[i]["end"] > adjusted[i + 1]["start"]:
                    adjusted[i]["end"] = adjusted[i + 1]["start"]
                    adjusted[i]["start"] = adjusted[i]["end"] - duration
            adjusted_timings = adjusted
        else:
            adjusted_timings = [dict(start=et["start"], end=et["end"], original_start=et["start"], source_event=et.get("source_event", "")) for et in sorted_timings]
        # NPU Task(npu_*) distributed across multiple lines, double y-axis area
        if group_name.startswith("NPU Task("):
            n_lines = 10
            for i, event_timing in enumerate(adjusted_timings):
                start = event_timing["start"]
                end = event_timing["end"]
                original_start = event_timing["original_start"]
                source_event = event_timing.get("source_event", "")
                if start > 0 and end > 0 and original_start >= interval_start and original_start <= interval_end and end > start:
                    plot_start = start - interval_start
                    plot_end = end - interval_start
                    duration = plot_end - plot_start
                    job_id = extract_job_id(source_event if source_event else group_name)
                    if job_id is not None and job_id in job_colors:
                        color = job_colors[job_id]
                    else:
                        color = colors[idx]
                    subline = i % n_lines
                    rect = plt.Rectangle((plot_start, y + 2 * subline / n_lines), duration, 1.4 / n_lines, linewidth=1, edgecolor=color, facecolor=color, alpha=0.8)
                    ax.add_patch(rect)
                    text_x = plot_start + duration / 2
                    text_y = y + 2 * subline / n_lines + 0.6 / n_lines
                    text_fontsize = 5
                    if not hide_text:
                        if duration >= 1000000:
                            duration_str = f"{duration/1000000:.2f}"
                            unit = 'ms'
                            text_color = 'darkblue'
                            text_fontsize = 4
                        elif duration >= 1000:
                            duration_str = f"{duration/1000:.1f}"
                            unit = 'us'
                            text_color = 'red'
                            text_fontsize = 4
                        else:
                            duration_str = f"{duration}"
                            unit = 'ns'
                            text_color = 'silver'
                            text_fontsize = 3
                        text_y_pos = text_y #+ 0.1 / n_lines
                        plt.text(text_x, text_y_pos, duration_str, ha='center', va='center', color=text_color, fontsize=text_fontsize)
        else:
            last_plotted_start = 0
            for i, event_timing in enumerate(adjusted_timings):
                start = event_timing["start"]
                end = event_timing["end"]
                original_start = event_timing["original_start"]
                source_event = event_timing.get("source_event", "")
                if start > 0 and end > 0 and original_start >= interval_start and original_start <= interval_end and end > start:
                    plot_start = start - interval_start
                    plot_end = end - interval_start
                    duration = plot_end - plot_start
                    job_id = extract_job_id(source_event if source_event else group_name)
                    if job_id is not None and job_id in job_colors:
                        color = job_colors[job_id]
                    else:
                        color = colors[idx]
                    rect = plt.Rectangle((plot_start, y), duration, 0.7, linewidth=1, edgecolor=color, facecolor=color, alpha=0.8)
                    ax.add_patch(rect)
                    
                    # For NPU Core, overlay Service Process Wait as thin horizontal line
                    '''
                    if group_name.startswith("NPU Core_"):
                        core_num = group_name.split("_")[1]
                        service_wait_key = f"Service Process Wait_{core_num}"
                        if service_wait_key in service_wait_groups:
                            # Find matching Service Process Wait by Job ID and Core ID
                            found_match = False
                            for wait_timing in service_wait_groups[service_wait_key]:
                                wait_source = wait_timing.get("source_event", "")
                                wait_job_id = extract_job_id(wait_source)
                                # Match by Job ID only (same core is already guaranteed by service_wait_key)
                                if wait_job_id == job_id:
                                    # Check if this wait timing overlaps with current NPU Core timing
                                    wait_start = wait_timing["start"]
                                    wait_end = wait_timing["end"]
                                    # Service wait should overlap with NPU Core time (more flexible matching)
                                    if not (wait_end < start or wait_start > end):
                                        found_match = True
                                        # Clamp wait_start to interval_start if it's too early
                                        wait_plot_start = max(wait_start, min_start) - interval_start
                                        wait_plot_end = wait_end - interval_start
                                        
                                        # Always draw the wait line (removed visibility condition)
                                        # Draw thin horizontal line for Service Process Wait
                                        line_y = y + 0.35  # Middle of the rectangle
                                        ax.plot([wait_plot_start, wait_plot_end], [line_y, line_y], 
                                               color='black', linewidth=0.5, alpha=0.2, zorder=10, linestyle="--")
                                        
                                        # Draw vertical lines at the ends only if they're visible
                                        if wait_plot_start >= 0:
                                            ax.plot([wait_plot_start, wait_plot_start], [y + 0.1, y + 0.6], 
                                                   color='black', linewidth=0.6, alpha=0.5, zorder=10)
                                        if wait_plot_end <= interval:
                                            ax.plot([wait_plot_end, wait_plot_end], [y + 0.1, y + 0.6], 
                                                   color='black', linewidth=0.6, alpha=0.5, zorder=10)
                                        
                                        # Add text label for wait duration if not hiding text
                                        
                                        if not hide_text and wait_plot_end - wait_plot_start > interval * 0.05:
                                            wait_duration_ns = wait_end - wait_start
                                            if wait_duration_ns >= 10000000000:  # 10 seconds
                                                wait_text = f"Wait: {wait_duration_ns/1000000000:.1f}s"
                                            elif wait_duration_ns >= 10000000:  # 10 ms
                                                wait_text = f"Wait: {wait_duration_ns/1000000:.1f}ms"
                                            else:
                                                wait_text = f"Wait: {wait_duration_ns/1000:.1f}us"
                                            
                                            # Position text above the line
                                            text_x = (wait_plot_start + wait_plot_end) / 2
                                            ax.text(text_x, y - 0.15, wait_text, 
                                                   ha='center', va='bottom', 
                                                   color='black', fontsize=3, alpha=0.8)

                                        break
                            
                            # If no exact match found, try to find any Service Process Wait for this core
                            if not found_match:
                                print(f"No exact match found for Job {job_id} on Core {core_num}")
                                print(f"NPU Core timing: {start} - {end}")
                                print(f"Available Service Process Wait timings for core {core_num}:")
                                for wait_timing in service_wait_groups[service_wait_key]:
                                    wait_source = wait_timing.get("source_event", "")
                                    wait_job_id = extract_job_id(wait_source)
                                    wait_start = wait_timing["start"]
                                    wait_end = wait_timing["end"]
                                    print(f"  Job {wait_job_id}: {wait_start} - {wait_end}")
                                
                                # Draw any available Service Process Wait for this core as a fallback
                                if service_wait_groups[service_wait_key]:
                                    wait_timing = service_wait_groups[service_wait_key][0]  # Use first available
                                    wait_start = wait_timing["start"]
                                    wait_end = wait_timing["end"]
                                    wait_plot_start = max(wait_start, min_start) - interval_start
                                    wait_plot_end = wait_end - interval_start
                                    
                                    # Draw fallback line in red to indicate mismatch
                                    line_y = y + 0.35
                                    ax.plot([wait_plot_start, wait_plot_end], [line_y, line_y], 
                                           color='red', linewidth=1.5, alpha=0.7, zorder=10, linestyle='--')
                        else:
                            print(f"No Service Process Wait group found for core {core_num}")
                            print(f"Available Service Process Wait groups: {list(service_wait_groups.keys())}")
                    '''
                    
                    text_x = plot_start + duration / 2
                    text_y = y + 0.3
                    text_fontsize = 5
                    if not hide_text:
                        if duration >= 1000000:
                            duration_str = f"{duration/1000000:.2f}"
                            unit = 'ms'
                            text_color = 'darkblue'
                            text_fontsize = 5
                        elif duration >= 1000:
                            duration_str = f"{duration/1000:.1f}"
                            unit = 'us'
                            text_color = 'red'
                            text_fontsize = 4
                        else:
                            duration_str = f"{duration}"
                            unit = 'ns'
                            text_color = 'silver'
                            text_fontsize = 3
                        if i % 2 == 0:
                            text_y_pos = text_y - 0.15
                        else:
                            text_y_pos = text_y + 0.22
                        plt.text(text_x, text_y_pos, duration_str, ha='center', va='center', color=text_color, fontsize=text_fontsize)
                    if show_gap and group_name.startswith("PCIe Write"):
                        if i > 0 and plot_start > last_plotted_start and not hide_text:
                            time_difference = plot_start - last_plotted_start
                            plt.text((last_plotted_start + plot_start) / 2, text_y - 0.4, f"Δ {time_difference / 1000}us", ha='center', va='center', color='black', fontsize=text_fontsize)
                        last_plotted_start = plot_start

    # Create legend for job IDs
    # Maximum 30 items in legend
    if job_colors:
        legend_elements = []
        for i, (job_id, color) in enumerate(sorted(job_colors.items())):
            if i >= 30:
                legend_elements.append(plt.Rectangle((0,0),1,1, facecolor='white', edgecolor='black', alpha=0.0, label='...'))
                break
            legend_elements.append(plt.Rectangle((0,0),1,1, facecolor=color, edgecolor=color, alpha=0.5, label=f'Job {job_id}'))
        plt.legend(handles=legend_elements, loc='upper right', bbox_to_anchor=(1.15, 1))

    plt.xlabel("Time (ns)")
    # Title color information text
    unit_color_legend = '(time unit: ns=silver, us=red, ms=darkblue)'
    plt.title(f"DX-RT Profiler\n{unit_color_legend}")
    plt.xlim(0, interval)
    plt.savefig(output, bbox_inches="tight", dpi=300)
    print(interval, "ns / ", interval_all, "ns")
    print("Done.")

if __name__ == '__main__':
    parser = argparse.ArgumentParser(description="Draw timing chart from profiler data.")
    parser.add_argument("-i", "--input", default="profiler.json", help="Input json file to plot")
    parser.add_argument("-o", "--output", default="profiler.png", help="Output image file to save the plot")
    parser.add_argument("-s", "--start", type=float, default=0.0, help="Starting point( > 0.0) when the entire interval is 1")
    parser.add_argument("-e", "--end", type=float, default=1.0, help="End point( < 1.0) when the entire interval is 1")
    parser.add_argument("-g", "--show_gap", action="store_true", help="Show time gap between starting points")
    parser.add_argument("-t", "--hide_text", action="store_true", default=False, help="Hide duration text")
    args = parser.parse_args()
    input = args.input
    output = args.output
    start_ratio = args.start
    end_ratio = args.end
    show_gap = args.show_gap
    hide_text = args.hide_text
    
    plot(input, output, start_ratio, end_ratio, show_gap, hide_text)