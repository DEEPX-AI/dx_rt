# loop-selector

# loop-selector

**Intelligent loop count recommendation tool for DX models**

Automatically analyzes model performance and recommends optimal loop count based on FPS measurement and target duration.

## Quick Start

```bash
# Basic usage (3 second target)
loop-selector model.dxnn

# Custom target duration
loop-selector model.dxnn --target-duration 5.0

# Update YAML config directly
loop-selector model.dxnn --update-yaml config.yaml
```

## How It Works

**Intelligent Measurement Process**:

1. **Initial Assessment**: Analyzes model file size to determine optimal initial test parameters
2. **Adaptive Measurement**: Performs multiple FPS measurements with statistical analysis
3. **Stability Compensation**: If measurements are unstable (CV > 5%), automatically performs enhanced measurement:
   - **Iterative Retry**: Up to 3 compensation attempts
   - **Progressive Enhancement**: Each attempt doubles loops and samples (e.g., 30→60→120 loops)
   - **Smart Termination**: Stops when CV improves or stops improving
   - **Thermal Stability**: Adds extra warmup runs for each attempt
4. **Loop Calculation**: Computes optimal loop count using `FPS × target_duration`
5. **Result Validation**: Applies nice number rounding for human-friendly output

**Stability Enhancement Details**:
- **CV Threshold**: 5% coefficient of variation triggers compensation
- **Maximum Attempts**: Up to 3 iterative retries to achieve stable measurement
- **Early Stopping**: If compensation doesn't improve CV, previous best result is used
- **Performance Impact**: Compensation may take 2-3x longer but provides more reliable results
- **Compensation Tracking**: Output indicates when compensation was performed

## Installation

Already included in `tool/loop-selector/loop-selector`. Just run it!

## Usage

### Basic Commands

```bash
# Standard recommendation (3s target)
loop-selector /path/to/model.dxnn

# Custom target duration
loop-selector model.dxnn --target-duration 5.0

# Custom sample count for better statistics
loop-selector model.dxnn --samples 5

# Common target durations
loop-selector model.dxnn --target-duration 1.0   # Quick testing
loop-selector model.dxnn --target-duration 3.0   # Standard (default)
loop-selector model.dxnn --target-duration 5.0   # High precision
loop-selector model.dxnn --target-duration 10.0  # Ultra-high precision
```

### Output Formats

```bash
# Text (default) - human readable
loop-selector model.dxnn
# Output:
# ✓ Recommended: 800 loops
#   Target: 3.0s, FPS: 267.1, CV: 1.2%
#   Latency: 3.74ms, NPU: 1.82ms

# With compensation (unstable measurement)
loop-selector model.dxnn --samples 2
# Output:
# ✓ Recommended: 150 loops
#   Target: 3.0s, FPS: 55.1, CV: 2.7%
#   Compensation: Performed
#   Latency: 92.72ms, NPU: 7.43ms

# Value only - for scripts
LOOPS=$(loop-selector model.dxnn --format value)
echo $LOOPS  # 800

# JSON - structured data
loop-selector model.dxnn --format json
# {
#   "loops": 800,
#   "target_duration": 3.0,
#   "measured_fps": 267.1,
#   "latency_ms": 3.74,
#   "npu_time_ms": 1.82,
#   "cv_percent": 1.2,
#   "is_stable": true,
#   "compensation_performed": false,
#   "reason": "Target: 3.0s, FPS: 267.1, Calculated: 802 → Final: 800"
# }

# YAML - for config files
loop-selector model.dxnn --format yaml
# loop_count: 800

# Environment variables
eval $(loop-selector model.dxnn --format env)
echo $DX_LOOP_COUNT         # 800
echo $DX_TARGET_DURATION    # 3.0
echo $DX_MEASURED_FPS       # 267.1
```

### YAML Integration

```bash
# Update existing YAML config
loop-selector model.dxnn --update-yaml myconfig.yaml
# ✓ Recommended: 800 loops
# ✓ Updated myconfig.yaml: loop_count = 800

# Then run dx-fit
dx-fit myconfig.yaml
```

### Detailed Explanation

```bash
loop-selector model.dxnn --explain

# Custom number of samples
loop-selector model.dxnn --samples 5

# Output:
# ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
#  Loop Selector - Calculation Details
# ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
# 
# 📊 Model Performance Measurement:
#    Model: AlexNet_5.dxnn
#    Measured FPS: 267.3 (CV: 1.2%)
#    Latency: 3.74ms
#    NPU Time: 1.82ms
# 
# 🎯 Target Configuration:
#    Target duration: 3.0 seconds
# 
# 📐 Loop Calculation:
#    Formula: loops = FPS × target_duration
#    Calculated: 267.3 × 3.0 = 802 loops
#    Final (with rounding): 800 loops
# 
# ✓ Recommended: 800 loops
#   Expected duration: ~3.0 seconds
# 
# 💡 Tips:
#    • Faster testing: --target-duration 1.0 → ~267 loops
#    • Higher precision: --target-duration 5.0 → ~1335 loops
```

## Examples

### Workflow 1: dx-fit with auto loop selection

```bash
# Step 1: Generate recommended loop count
loop-selector model.dxnn --update-yaml myconfig.yaml

# Step 2: Run dx-fit
dx-fit myconfig.yaml
```

### Workflow 2: Batch processing

```bash
#!/bin/bash
# Process multiple models

for model in models/*.dxnn; do
    echo "Processing $model..."
    
    # Get recommended loops
    loops=$(loop-selector "$model" --format value)
    
    # Update config
    sed -i "s/loop_count:.*/loop_count: $loops/" config.yaml
    
    # Run dx-fit
    dx-fit config.yaml
done
```

### Workflow 3: Python integration

```python
import subprocess
import json

def get_optimal_loops(model_path, target_duration=3.0):
    result = subprocess.run(
        ['loop-selector', model_path, 
         '--target-duration', str(target_duration),
         '--format', 'json'],
        capture_output=True,
        text=True
    )
    
    data = json.loads(result.stdout)
    return data['loops']

# Usage
loops = get_optimal_loops('model.dxnn', target_duration=5.0)
print(f"Recommended loops: {loops}")
```

**Note**: Loop counts are calculated directly from `FPS × target_duration` without any artificial constraints. Unstable measurements (CV > 5%) are automatically compensated with up to 3 iterative retries for improved reliability.

## Advanced Options

```bash
# Custom run_model path
loop-selector model.dxnn --run-model /custom/path/run_model

# Combine options
loop-selector model.dxnn \
    --target-duration 5.0 \
    --format json \
    --update-yaml config.yaml \
    > loops_report.json
```

## FAQ

**Q: When should I use this instead of manual loop count?**
A: Always! Unless you have a very specific reason (e.g., comparing against fixed benchmark), auto-selection is more reliable.

**Q: Can I use this without dx-fit?**
A: Yes! It works standalone and outputs in multiple formats for any workflow.

**Q: Can I integrate this into CI/CD?**
A: Absolutely! Use `--format value` or `--format json` for easy parsing.

## See Also

- [dx-fit documentation](../dx-fit/README.md)
- [dx-fit-automation](../dx-fit-automation/README.md)
- [Loop Selection Policy Details](../dx-fit-automation/LOOP_SELECTION.md)

## Troubleshooting

**Error: "run_model not found"**
```bash
# Solution: Specify path explicitly
loop-selector model.dxnn --run-model /path/to/run_model
```

**Error: "Model file not found"**
```bash
# Use absolute path
loop-selector /absolute/path/to/model.dxnn
```

**Loops seem too high/low?**
```bash
# Check with explanation
loop-selector model.dxnn --explain

# Adjust target duration
loop-selector model.dxnn --target-duration 2.0  # Reduce loops
loop-selector model.dxnn --target-duration 5.0  # Increase loops
```
