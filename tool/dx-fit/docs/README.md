# DX-Fit: DX-RT Performance Parameter Tuning Tool

DX-Fit automatically finds the best combination of DX-RT environment variables to maximize inference performance.

## ⚡ Key Capabilities

- **Bayesian Optimization** ⭐ – Discovers strong configurations with minimal samples.
- **Automated parameter search** – Grid, Random, and Bayesian strategies built in.
- **Intelligent loop selection** 🆕 – Automatic loop count based on target duration or manual configuration.
- **Performance telemetry** – Measures FPS, latency, queue load, NPU processing time, and more.
- **Thermal management** – Monitors NPU temperature and inserts cooling waits when needed.
- **Result analysis** – Produces CSV, JSON, text summaries, and visualization-ready artifacts.

## 🚀 Quick Start

### 1. Install dependencies

```bash
cd tool/dx-fit
./install.sh  # installs pandas, pyyaml, matplotlib, seaborn, scikit-optimize
```

Or install manually:

```bash
pip install pandas pyyaml matplotlib seaborn scikit-optimize
```

### 2. First run (Bayesian recommended)

```bash
# Bayesian quick optimization (short sweep example)
python3 dx-fit examples/03_bayesian_quick.yaml

# Bayesian standard optimization (recommended baseline)
python3 dx-fit examples/04_bayesian_standard.yaml
```

## 📖 Usage Guide

### Basic execution

```bash
cd tool/dx-fit

# Bayesian optimization (recommended)
python3 dx-fit examples/04_bayesian_standard.yaml

# Fast random exploration
python3 dx-fit examples/02_quick_random.yaml

# Full grid exploration
python3 dx-fit examples/05_grid_small.yaml
```

### Provided example configurations

| File | Strategy | Characteristics | Primary use |
|------|----------|-----------------|-------------|
| `01_template.yaml` | - | Baseline template | Starting point for new configs |
| `02_quick_random.yaml` | Random | Short search window | Quick initial sweep |
| `03_bayesian_quick.yaml` | Bayesian | Limited trials | Rapid optimization |
| `04_bayesian_standard.yaml` ⭐ | Bayesian | Recommended defaults | **Go-to configuration** |
| `05_grid_small.yaml` | Grid | Compact full-factor sweep | Exhaustive example |
| `06_thermal_bayesian.yaml` | Bayesian | Includes thermal stabilization | Thermal-sensitive scenarios |
| `07_thermal_fixed_cooldown.yaml` | Random | Fixed cooldown delays | Environments without sensors |
| `08_grid_full.yaml` | Grid | Large full-factor sweep | Deep analysis |
| `09_bayesian_target_duration.yaml` 🆕 | Bayesian | **Automatic loop selection** | **Intelligent loop count based on FPS** |

See [examples/README.md](../examples/README.md) for detailed descriptions.

### Configuration file (YAML)

Bayesian optimization example:

```yaml
model_path: /path/to/model.dxnn

# Execution settings - OPTION 1: Manual loop count
loop_count: 100

# Execution settings - OPTION 2: Automatic loop selection (NEW!) 🆕
# target_duration: 3.0  # Target measurement duration in seconds
                        # Uncomment to enable automatic loop count
                        # based on measured model FPS

use_ort: true
warmup_runs: 5
timeout: 600

# Strategy
strategy: bayesian
max_random_samples: 50  # Total number of evaluations for Bayesian

# Parameters
parameters:
  DXRT_TASK_MAX_LOAD: [2, 4, 6, 8, 10, 12, 14]
  CUSTOM_INTRA_OP_THREADS_COUNT: [1, 2, 4]
  CUSTOM_INTER_OP_THREADS_COUNT: [1, 2]
  NFH_OUTPUT_WORKER_THREADS: [4, 6, 8]

# Thermal management (optional)
thermal_management:
  enabled: true
  target_temperature: 65.0
```

### NEW: Target Duration - Automatic Loop Selection 🆕

Instead of manually specifying `loop_count`, you can use `target_duration` to let dx-fit automatically calculate the optimal loop count based on model performance:

```yaml
model_path: /path/to/model.dxnn

# Use target_duration instead of loop_count
target_duration: 3.0  # Target measurement duration in seconds

# ... rest of configuration
```

**How it works:**
1. dx-fit measures the model's FPS using a quick initial test
2. Calculates optimal loop count: `loops = FPS × target_duration`
3. Uses the calculated loop count for all parameter tests

**Recommended values:**
- `2.0s`: Fast testing (less precise, good for quick exploration)
- `3.0s`: Balanced (recommended for most use cases)
- `5.0s`: High precision (longer but more accurate)

**Benefits:**
- No need to manually tune loop count for each model
- Ensures consistent measurement duration across different models
- Automatically adapts to model performance (fast or slow models)
- Minimum 1 loop guaranteed (even for very slow models)

**Note:** If both `target_duration` and `loop_count` are specified, `target_duration` takes precedence.

Refer to `examples/01_template.yaml` for the full option list.

## 📊 Output Artifacts

DX-Fit produces the following files after a tuning run:

```text
results_YYYYMMDD_HHMMSS.csv       # Detailed trial log
best_config_YYYYMMDD_HHMMSS.json  # Best configuration summary
tuning_report_YYYYMMDD_HHMMSS.txt # Human-readable report
```

> **Note:** Visualization assets are generated with the `dx-fit-analyze` helper.

### CSV result file

| Column | Description |
|--------|-------------|
| `test_id` | Trial identifier |
| `success` | Success flag (True/False) |
| `fps` | Frames per second |
| `latency` | Average latency (ms) |
| `npu_processing_time` | NPU processing time (ms) |
| `run_time` | Test execution time (s) |
| `cooling_time` | Cooling wait time (s) |
| `pre_test_temp` | Temperature before the trial (°C) |
| `post_test_temp` | Temperature after the trial (°C) |
| `pre_test_voltage` | Voltage before the trial (mV) |
| `post_test_voltage` | Voltage after the trial (mV) |
| `DXRT_*` | Environment variables applied |

### Best configuration JSON

```json
{
  "fps": 178.79,
  "latency": 5.59,
  "npu_time": 2.34,
  "parameters": {
    "DXRT_TASK_MAX_LOAD": 14,
    "CUSTOM_INTRA_OP_THREADS_COUNT": 1,
    "NFH_OUTPUT_WORKER_THREADS": 8
  },
  "thermal_data": {
    "pre_test_temp": 62.5,
    "post_test_temp": 68.3,
    "pre_test_voltage": 825.0,
    "post_test_voltage": 825.0,
    "cooling_time": 0.0
  },
  "timestamp": "2025-10-17T15:30:45.123456"
}
```

## 📈 Analyzing Results

### Automated analysis helper

```bash
cd tool/dx-fit

# Generate statistics and plots
python3 dx-fit-analyze results_*.csv -o analysis
```

Generated assets:

- Performance summary report
- Parameter correlation breakdown
- Visualization bundle (`analysis/plots/`)
  - FPS distribution histogram
  - Parameter impact bar charts
  - Temperature vs. performance correlation

### Applying the best configuration

```bash
# Inspect the JSON file
cat best_config_*.json

# Export environment variables
export DXRT_TASK_MAX_LOAD=14
export CUSTOM_INTRA_OP_THREADS_COUNT=1
export NFH_OUTPUT_WORKER_THREADS=8

# Or create an auto-apply script
jq -r '.parameters | to_entries[] | "export \(.key)=\(.value)"' \
  best_config_*.json > apply_config.sh
source apply_config.sh

# Validate with the tuned settings
run_model -m /path/to/model.dxnn -l 100 --use-ort -v
```

## Parameter Reference

### DX-RT parameters

| Parameter | Description | Recommended range |
|-----------|-------------|-------------------|
| `DXRT_TASK_MAX_LOAD` | Maximum concurrent tasks | 2–12 |
| `DXRT_DYNAMIC_CPU_THREAD` | Enable dynamic CPU threads | 0, 1 |
| `DXRT_ACL_FAST_MATH` | Use fast math kernels | 0, 1 |

### ONNX Runtime parameters

| Parameter | Description | Recommended range |
|-----------|-------------|-------------------|
| `CUSTOM_INTRA_OP_THREADS_COUNT` | Intra-op thread count | 1–8 |
| `CUSTOM_INTER_OP_THREADS_COUNT` | Inter-op thread count | 1–4 |

### NFH (NPU Format Handler) parameters

| Parameter | Description | Recommended range |
|-----------|-------------|-------------------|
| `NFH_INPUT_WORKER_THREADS` | Input worker threads | 1–4 |
| `NFH_OUTPUT_WORKER_THREADS` | Output worker threads | 2–8 |

## Thermal Management

DX-Fit can monitor NPU temperatures and automatically wait for cooldown periods.

### Cooling modes

1. **Adaptive Cooling** (`thermal_management.enabled: true`)
   - Monitors temperature in real time.
   - Waits until the target temperature is reached.
   - Enforces a maximum cooling delay.

2. **Fixed Cooldown** (`thermal_management.enabled: false`, `fixed_cooldown_seconds > 0`)
   - Waits a fixed duration after each trial.
   - Suitable when temperature sensors are unavailable.

3. **No Cooling** (both disabled)
   - Runs trials back-to-back with no waits.
   - Best for quick smoke tests.

### Thermal configuration example

```yaml
thermal_management:
  enabled: true
  max_temperature: 80.0       # Warn when exceeding this value
  target_temperature: 65.0    # Wait until the NPU cools to this level
  check_interval: 5           # Poll temperature every 5 seconds
  max_cooling_time: 300       # Cap cooldown to 300 seconds
```

## 💼 Usage Scenarios

### 1. New model tuning (recommended path)

```bash
cd tool/dx-fit

# 1. Start with a quick Bayesian sweep
python3 dx-fit examples/03_bayesian_quick.yaml

# 2. Review the results
python3 dx-fit-analyze results_*.csv -o analysis

# 3. Refine with the standard Bayesian configuration
python3 dx-fit examples/04_bayesian_standard.yaml

# 4. Apply the best settings
source apply_config.sh
```

### 2. Thermal-sensitive environments

```bash
cd tool/dx-fit

# Bayesian optimization with cooling
python3 dx-fit examples/06_thermal_bayesian.yaml

# Inspect temperature impact
python3 dx-fit-analyze results_*.csv -o analysis
```

### 3. CI/CD integration

```bash
#!/bin/bash
# regression_test.sh

cd tool/dx-fit

# Quick Bayesian regression check
python3 dx-fit examples/03_bayesian_quick.yaml

# Validate against an FPS threshold
best_config=$(ls -t best_config_*.json | head -1)
fps=$(jq -r '.fps' "$best_config")

if (( $(echo "$fps < 150" | bc -l) )); then
    echo "❌ Performance regression: FPS = $fps"
    exit 1
fi

echo "✅ Performance OK: FPS = $fps"
```

## 🔧 Troubleshooting

### Unable to read NPU temperature

```bash
# Ensure dxrt-cli works
dxrt-cli -s

# Mitigation options
# 1. Set thermal_management.enabled: false
# 2. Use fixed_cooldown_seconds instead
```

### All trials failing

- Verify the model path: `ls /path/to/model.dxnn`
- Manually run `run_model` to confirm baseline execution.
- Increase the timeout (for example, 300 → 600 seconds).


## 💡 Best Practices

1. **Default to Bayesian** – Delivers the best results for most models.
2. **Start small** – Validate with a quick example before long sweeps.
3. **Enable thermal control** – Recommended for extended sessions.
4. **Archive outputs** – Timestamped filenames make versioning easy.
5. **Study correlations** – Use dx-fit-analyze to understand parameter impact.

## 📚 Learn More

- **Quick start**: [QUICKSTART.md](QUICKSTART.md)
- **Strategy comparison**: [STRATEGY_COMPARISON.md](STRATEGY_COMPARISON.md)
- **Example breakdown**: [../examples/README.md](../examples/README.md)
- **Overview**: [../README.md](../README.md)

## Dependencies

### Required

- Python 3.6+
- DX-RT (`run_model` command)

### Python packages

```bash
pip install pandas pyyaml matplotlib seaborn scikit-optimize
```

Or run:

```bash
cd tool/dx-fit && ./install.sh
```

---

**Version**: 1.1.0 (Bayesian Optimization enabled)  
**Last updated**: 2025-10-16
