# DX-Fit Quick Start Guide

This guide distills the essential steps to get productive with DX-Fit in minutes.

## Step 1: Install dependencies

```bash
cd tool/dx-fit

# Install required Python packages
./install.sh

# Or install manually
pip install pandas pyyaml matplotlib seaborn scikit-optimize
```

> **💡 Tip:** Bayesian Optimization typically finds strong parameters far faster than manual tuning.

## Step 2: Run your first optimization

### Option A: Bayesian Optimization ⭐ Recommended

Use this option when you want fast, sample-efficient optimization.

```bash
cd tool/dx-fit

# Short Bayesian sweep
python3 dx-fit examples/03_bayesian_quick.yaml

# Standard Bayesian sweep (recommended default)
python3 dx-fit examples/04_bayesian_standard.yaml
```

### Option B: Random Search

Choose this path for the quickest initial exploration.

```bash
cd tool/dx-fit

# Lightweight random sweep
python3 dx-fit examples/02_quick_random.yaml
```

### Option C: Grid Search

Run this when you need exhaustive parameter coverage and can afford longer runtimes.

```bash
cd tool/dx-fit

# Compact grid sweep
python3 dx-fit examples/05_grid_small.yaml

# Full grid sweep
python3 dx-fit examples/08_grid_full.yaml
```

Sample output:

```text
=== DX-RT Parameter Fitting Tool ===
Model: /path/to/model.dxnn
Strategy: grid
Total combinations: 48
Results: results_20241015_120000.csv

Starting parameter tuning...

--- Test 1 ---
Parameters: {'DXRT_TASK_MAX_LOAD': 2, 'CUSTOM_INTRA_OP_THREADS_COUNT': 1, ...}
SUCCESS: FPS=150.23, Latency=6.65ms, Run Time=5.2s
```

## Step 3: Review the results

```bash
cd tool/dx-fit

# Analyze the latest results
python3 dx-fit-analyze results_*.csv -o analysis
```

**Sample analysis report:**

```text
============================================================
DX-FIT Results Analysis
============================================================
Total tests: 50
Successful tests: 50 (100%)
Failed tests: 0

Performance Statistics:
----------------------------------------
FPS: min=120.45, max=178.79, mean=155.32, std=12.45

Best Configuration:
----------------------------------------
FPS: 178.79
Latency: 5.59ms
Parameters:
  DXRT_TASK_MAX_LOAD: 14
  CUSTOM_INTRA_OP_THREADS_COUNT: 1
  CUSTOM_INTER_OP_THREADS_COUNT: 1
  NFH_OUTPUT_WORKER_THREADS: 8
```

Analysis artifacts are stored in `analysis/`:

- Detailed statistics report
- Parameter impact summaries
- Visualization bundle (`plots/`)

## Step 4: Apply the best configuration

```bash
# Inspect the best configuration JSON
cat best_config_20251016_*.json

# Export the tuned parameters
export DXRT_TASK_MAX_LOAD=14
export CUSTOM_INTRA_OP_THREADS_COUNT=1
export CUSTOM_INTER_OP_THREADS_COUNT=1
export NFH_OUTPUT_WORKER_THREADS=8

# Run the model with tuned settings
run_model -m /path/to/model.dxnn -l 100 --use-ort -v
```

## Next steps

### Deeper optimization

```bash
cd tool/dx-fit

# Standard Bayesian sweep (50 iterations)
python3 dx-fit examples/04_bayesian_standard.yaml

# Bayesian sweep with thermal management
python3 dx-fit examples/06_thermal_bayesian.yaml
```

### Customize your own configuration

Copy the template and adjust:

```bash
cd tool/dx-fit
cp examples/01_template.yaml my_config.yaml
```

Edit `my_config.yaml`:

```yaml
model_path: /path/to/your/model.dxnn
loop_count: 50
use_ort: true
timeout: 300

# Bayesian Optimization (recommended)
strategy: bayesian
bayesian:
  n_calls: 30
  n_initial_points: 5

parameters:
  DXRT_TASK_MAX_LOAD: [4, 6, 8, 10, 12, 14]
  CUSTOM_INTRA_OP_THREADS_COUNT: [1, 2, 4]
  CUSTOM_INTER_OP_THREADS_COUNT: [1, 2]
  NFH_OUTPUT_WORKER_THREADS: [4, 6, 8]
```

Run the custom configuration:

```bash
python3 dx-fit my_config.yaml
```

## Common workflows

### Pattern 1: Quick optimization (recommended)

```bash
cd tool/dx-fit

# 1. Run a quick Bayesian sweep
python3 dx-fit examples/03_bayesian_quick.yaml

# 2. Analyze the results
python3 dx-fit-analyze results_*.csv -o analysis

# 3. Inspect and apply the best configuration
cat best_config_*.json
```

### Pattern 2: Precision optimization

```bash
cd tool/dx-fit

# 1. Standard Bayesian sweep (50 iterations)
python3 dx-fit examples/04_bayesian_standard.yaml

# 2. Re-run with thermal control for stability
python3 dx-fit examples/06_thermal_bayesian.yaml

# 3. Compare with the analysis tool
python3 dx-fit-analyze results_*.csv -o analysis
```

### Pattern 3: Automated script

```bash
#!/bin/bash
# auto_optimize.sh

cd tool/dx-fit

echo "Starting Bayesian optimization..."
python3 dx-fit examples/04_bayesian_standard.yaml

# Locate the latest results
RESULT=$(ls -t results_*.csv | head -1)
BEST_CONFIG=$(ls -t best_config_*.json | head -1)

# Analyze
python3 dx-fit-analyze "$RESULT" -o analysis

# Check best FPS
BEST_FPS=$(jq -r '.fps' "$BEST_CONFIG")
echo "Best FPS: $BEST_FPS"

# Verify against a threshold
if (( $(echo "$BEST_FPS > 150" | bc -l) )); then
    echo "✓ Performance target achieved!"
    # Generate environment exports
    jq -r '.parameters | to_entries[] | "export \(.key)=\(.value)"' "$BEST_CONFIG" > apply_config.sh
    echo "Run: source apply_config.sh"
else
    echo "✗ Performance below target, consider revisiting the search space"
fi
```

## Tips

1. **Start with `03_bayesian_quick.yaml`** – Validate your setup quickly.
2. **Compare runs** – Repeat sweeps to confirm consistency.
3. **Watch thermals** – Enable cooling controls for long sessions.
4. **Keep the CSV logs** – Historical data is valuable for regression checks.
5. **Tune per system** – Optimal values vary across hardware profiles.

## Need help?

See the full [README.md](README.md) for detailed guidance.

If you encounter issues:

1. Confirm the `run_model` command works manually.
2. Inspect system status with `dxrt-cli -s`.
3. Increase the timeout value.
4. Narrow the parameter ranges to isolate problems.
