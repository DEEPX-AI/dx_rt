# DX-Fit - DXRT Parameter Tuning Tool

DX-Fit is an automated parameter tuning toolkit designed to maximize inference performance on DEEPX NPUs.

## ⚡ Key Features

- **Bayesian Optimization** ⭐ – Finds near-optimal configurations with minimal samples.
- **Multiple search strategies** – Grid, Random, and Bayesian search in one interface.
- **Intelligent loop selection** – Automatic loop count based on target duration or manual configuration.
- **Comprehensive telemetry** – FPS, latency, NPU time, and thermal data.
- **Thermal management** – Optional cooling control with temperature monitoring.
- **Result analysis** – CSV, JSON, text summaries, and visualization helpers.

## 🚀 Quick Start

### 1. Install

```bash
cd tool/dx-fit
./install.sh  # installs pandas, pyyaml, matplotlib, seaborn, scikit-optimize
```

### 2. First run (Bayesian recommended)

```bash
# Bayesian quick sweep (small search space example)
python3 dx-fit examples/03_bayesian_quick.yaml

# Bayesian standard sweep (recommended baseline)
python3 dx-fit examples/04_bayesian_standard.yaml
```

### 3. Analyze results

```bash
# Results are automatically organized in results_TIMESTAMP/ directory
python3 dx-fit-analyze results_20241017_143052/results.csv

# Or use the suggested command from dx-fit output
```

## 💡 Usage Examples

### Example 1: Recommended Bayesian optimization

```bash
python3 dx-fit examples/04_bayesian_standard.yaml
```

### Example 2: Fast random exploration

```bash
python3 dx-fit examples/02_quick_random.yaml
```

### Example 3: Exhaustive grid sweep

```bash
python3 dx-fit examples/05_grid_small.yaml
```

### Example 4: Dynamic loop count with target duration

```bash
# Use target_duration instead of loop_count
# Loop count is automatically calculated based on model FPS
python3 dx-fit examples/09_bayesian_target_duration.yaml
```

See [examples/README.md](examples/README.md) for detailed configuration options.

## 📁 Project Layout

```text
dx-fit/
├── dx-fit                        # main tuning entry point
├── dx-fit-analyze                # result analysis helper
├── cleanup_old_results.sh        # cleanup helper for old scattered files
├── install.sh                    # dependency bootstrapper
├── README.md                     # this overview
├── docs/                         # detailed documentation
│   ├── README.md                 # complete user guide
│   ├── QUICKSTART.md             # quick start walkthrough
│   └── STRATEGY_COMPARISON.md    # strategy comparison details
├── examples/                     # ready-to-run configuration samples
│   ├── README.md                 # example descriptions
│   ├── 01_template.yaml          # boilerplate template
│   ├── 02_quick_random.yaml      # Random (short exploration)
│   ├── 03_bayesian_quick.yaml    # Bayesian (small search space)
│   ├── 04_bayesian_standard.yaml # Bayesian baseline ⭐
│   ├── 05_grid_small.yaml        # Grid (compact sweep)
│   ├── 06_thermal_bayesian.yaml  # Bayesian + thermal management
│   ├── 07_thermal_fixed_cooldown.yaml
│   └── 08_grid_full.yaml         # Grid (complete sweep)
└── results_TIMESTAMP/            # Output directory (auto-created)
    ├── results.csv               # Raw test data
    ├── best_config.json          # Optimal configuration
    ├── tuning_report.txt         # Summary report
    ├── analysis_plots.png        # Visualization (after dx-fit-analyze)
    └── analysis_report.txt       # Analysis report (after dx-fit-analyze)
```

**Note**: All result files are now organized in timestamped `results_TIMESTAMP/` directories for cleaner workspace management.

## 🧠 Search Strategies

| Strategy | Characteristics | When to use | Recommendation |
|----------|-----------------|-------------|----------------|
| **Bayesian** | Adaptive search, fast convergence | Most scenarios where time matters | ⭐⭐⭐⭐⭐ |
| Random | Simple random sampling | Need a quick coarse scan | ⭐⭐⭐ |
| Grid | Exhaustive enumeration | Small parameter spaces that need full coverage | ⭐⭐ |

For a deeper comparison see [docs/STRATEGY_COMPARISON.md](docs/STRATEGY_COMPARISON.md).

## 📊 Real-World Result Snapshot

YOLOv5S evaluation (Bayesian strategy):

- **Runs executed**: 30
- **Performance gain**: **2.57×** (69.59 → 178.79 FPS)
- **Recommended parameters**: TASK_MAX_LOAD=14, INTRA=1, INTER=1, NFH_OUT=8

## 📖 Documentation

- **[docs/QUICKSTART.md](docs/QUICKSTART.md)** – Quick start guide
- **[docs/README.md](docs/README.md)** – Full user guide
- **[docs/STRATEGY_COMPARISON.md](docs/STRATEGY_COMPARISON.md)** – Strategy breakdown
- **[examples/README.md](examples/README.md)** – Example catalog

## 📝 Primary Tunable Parameters

DX-Fit focuses on the following environment variables:

- `DXRT_TASK_MAX_LOAD` – Maximum concurrent tasks (2–14)
- `CUSTOM_INTRA_OP_THREADS_COUNT` – Intra-op thread count (1–4)
- `CUSTOM_INTER_OP_THREADS_COUNT` – Inter-op thread count (1–2)
- `NFH_OUTPUT_WORKER_THREADS` – NFH output workers (4–8)

## 🔧 System Requirements

- Python 3.6+
- DXRT 3.0.0+
- DEEPX NPU (for example, M1B)
- Python packages: `pandas`, `pyyaml`, `matplotlib`, `seaborn`, `scikit-optimize`

## 🧹 Workspace Management

### Clean up old scattered result files

If you have old result files from previous versions scattered in the directory:

```bash
# Archive old result files to old_results_TIMESTAMP/
./cleanup_old_results.sh
```

This will move old `results_*.csv`, `best_config_*.json`, `tuning_report_*.txt`, and `plots_*` directories to an archive folder.

### Current organized structure

All new results are automatically organized in `results_TIMESTAMP/` directories:

```bash
ls -l results_*/
# results_20241017_143052/
#   ├── results.csv
#   ├── best_config.json
#   ├── tuning_report.txt
#   ├── analysis_plots.png      (after dx-fit-analyze)
#   └── analysis_report.txt     (after dx-fit-analyze)
```

## 🐛 Troubleshooting

```bash
# Verify dependencies
pip list | grep -E "pandas|pyyaml|scikit-optimize"

# Run a standalone model check
run_model -m /path/to/model.dxnn -l 10 --use-ort -v

# If thermal reading fails, disable thermal management
# (set thermal_management.enabled: false in the config)
```

More troubleshooting tips are available in [docs/README.md](docs/README.md#-troubleshooting).

## 📄 License

Copyright (C) 2018- DEEPX Ltd. All rights reserved.

---

**Version**: 1.3.0  
**Last updated**: 2025-10-20

**Release Highlights**:
- **NEW**: Intelligent loop selection with `target_duration` option
- **NEW**: Automatic loop count calculation based on model FPS
- Improved error handling and diagnostic messages
- Enhanced result file detection with fallback logic
- Extended timeout for large models (300s)
- Environment variable naming consistency
