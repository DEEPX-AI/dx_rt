# DX-Fit Strategy Comparison

This guide compares the optimization strategies supported by DX-Fit, highlights their strengths and weaknesses, and outlines when to use each approach.

## 📊 Implemented Strategies

### 1. Grid Search

#### How it works (Grid)

Systematically explores every combination of the provided parameter values.

```python
# Example: two parameters, three values each
PARAM_A: [1, 2, 3]
PARAM_B: [10, 20, 30]

# 9 total combinations (3 × 3)
(1,10), (1,20), (1,30)
(2,10), (2,20), (2,30)
(3,10), (3,20), (3,30)
```

#### ✅ Advantages (Grid)

1. **Completeness**
   - Evaluates every possible combination.
   - No risk of missing the global optimum.
   - Produces highly reliable baselines.

2. **Reproducibility**
   - Identical configuration → identical results.
   - Straightforward to debug and validate.

3. **Clear parameter impact**
   - Quantifies how each parameter affects performance.
   - Enables interaction analysis between parameters.
   - Builds a structured performance map.

4. **Simple implementation**
   - No complex algorithms required.
   - Easy to reason about and explain.

#### ❌ Drawbacks (Grid)

1. **Combinatorial explosion**
   - Grows exponentially with parameter count.
   - Example: 5 parameters × 10 values = 100,000 trials.
   - Quickly becomes impractical in runtime and cost.

2. **Resource overhead**
   - Tests obviously poor combinations.
   - Spends time on unproductive regions.

3. **Scalability limits**
   - Hard to apply to continuous ranges.
   - Not feasible for large parameter spaces.

#### 📈 Best suited for (Grid)

- ≤ 4 parameters.
- ≤ 10 candidate values per parameter.
- Exhaustive diagnostics or academic studies.
- Environments without tight time constraints.

#### 📊 Complexity (Grid)

- **Time complexity**: $O(n_1 \times n_2 \times \dots \times n_k)$ where $n_i$ is the number of values for parameter $i$.
- **Space complexity**: $O(k)$ where $k$ is the number of parameters.

---

### 2. Random Search

#### How it works (Random)

Randomly samples $N$ combinations from the search space.

```python
# Example: up to five samples
PARAM_A: [1, 2, 3]
PARAM_B: [10, 20, 30]

# Random selections (5 total)
(2,20), (1,30), (3,10), (1,10), (2,30)
```

#### ✅ Advantages (Random)

1. **Efficiency**
   - Fixed budget regardless of parameter count.
   - Produces a quick baseline.
   - Good for early-stage exploration.

2. **Scalability**
   - Handles many parameters gracefully.
   - Accepts continuous ranges.
   - Less sensitive to the curse of dimensionality than grid search.

3. **Broad coverage**
   - Samples the overall space evenly on average.
   - Can uncover surprising high-performing regions.

4. **Anytime execution**
   - Stop at any point and keep the current best result.
   - Easy to resume with additional samples later.

#### ❌ Drawbacks (Random)

1. **Non-deterministic**
   - Results vary between runs.
   - Requires a fixed random seed for reproducibility.

2. **Incomplete**
   - May miss the global optimum entirely.
   - Some regions might never be sampled.

3. **Potential duplicates**
   - The same combination can appear multiple times.
   - (Current implementation does not deduplicate.)

4. **Limited insight**
   - Harder to quantify parameter influence.
   - Less suitable for interaction analysis.

#### 📈 Best suited for (Random)

- ≥ 5 parameters.
- Strict time budgets.
- Rapid estimation rather than precise tuning.
- Continuous or high-dimensional spaces.

#### 📊 Complexity (Random)

- **Time complexity**: $O(N)$ where $N = \text{max\_random\_samples}$.
- **Space complexity**: $O(k)$ where $k$ is the number of parameters.

---

### 3. Bayesian Optimization ⭐⭐⭐⭐⭐

#### How it works (Bayesian)

Uses previous observations to intelligently pick the next parameters to evaluate.

```text
1. Collect 5–10 random seed samples.
2. Train a Gaussian Process surrogate model.
3. Select the next point via Expected Improvement:
   - Exploration: probe uncertain regions.
   - Exploitation: refine promising areas.
4. Evaluate, update the model, and repeat (typically 30–50 rounds).
```

#### ✅ Advantages (Bayesian)

1. **Highly sample-efficient**
   - Finds strong configurations with minimal trials.
   - Up to 90% fewer runs than grid search.
   - Often converges within 30–50 evaluations.

2. **Intelligent search**
   - Focuses on promising regions quickly.
   - Learns from past results.
   - Balances exploration and exploitation automatically.

3. **Continuous parameter support**
   - Handles integer and real-valued parameters naturally.
   - Considers the entire range, not just discrete points.

4. **Robust to noise**
   - Tolerates measurement variance.
   - Provides statistically grounded guidance.

#### ❌ Drawbacks (Bayesian)

1. **Extra dependencies**
   - Requires `scikit-optimize` (and its SciPy/Numpy stack).

2. **Modeling overhead**
   - Updating the Gaussian Process adds compute cost.
   - Slows down with a large parameter set.

3. **Needs seed samples**
   - First 5–10 evaluations are random.
   - Works best with at least 20 total runs.

#### 📈 Best suited for (Bayesian)

- **Default recommendation** ⭐
- Medium parameter counts (3–10).
- Expensive trial costs or limited time budgets.
- Precision tuning for NPU workloads.

#### 📊 Complexity (Bayesian)

- **Time complexity**: $O(N)$ where $N = \text{n\_calls}$ (typically 30–50).
- **Space complexity**: $O(N \times k)$ due to the surrogate model.

#### 💡 Example configuration

```yaml
strategy: bayesian

bayesian:
  n_calls: 50              # total evaluations
  n_initial_points: 10     # random seed samples
  acquisition_function: EI # Expected Improvement

parameters:
  DXRT_TASK_MAX_LOAD: [2, 4, 6, 8, 10, 12, 14]
  CUSTOM_INTRA_OP_THREADS_COUNT: [1, 2, 4]
  NFH_OUTPUT_WORKER_THREADS: [4, 6, 8]
```

#### 🎯 Real-world snapshot

YOLOv5S benchmark:

- **Trials executed**: 30
- **Performance gain**: 2.57× (69.59 → 178.79 FPS)
- **Success rate**: 100%
- **Note**: Runtime varies with hardware, model size, and quota policies.

---

## 🔄 Strategy Comparison Table

| Dimension | Bayesian | Random | Grid |
|-----------|----------|--------|------|
| **Completeness** | ⭐⭐⭐⭐ | ⭐⭐ | ⭐⭐⭐⭐⭐ |
| **Efficiency** | ⭐⭐⭐⭐⭐ | ⭐⭐⭐⭐ | ⭐⭐ |
| **Scalability** | ⭐⭐⭐⭐ | ⭐⭐⭐⭐⭐ | ⭐⭐ |
| **Reproducibility** | ⭐⭐⭐⭐ | ⭐⭐⭐ | ⭐⭐⭐⭐⭐ |
| **Search intelligence** | ⭐⭐⭐⭐⭐ | ⭐ | ⭐⭐ |
| **Setup complexity** | Medium | Low | Low |
| **Typical runtime** | 30–50 runs | $N$ runs (user-defined) | $n_1×n_2×…×n_k$ |
| **Optimality** | ✅ Very high | ⚠️ Moderate | ✅ Guaranteed |
| **Recommended order** | 🥇 1st | 🥉 3rd | 🥈 2nd |

---

## 📋 Recommended Strategy by Scenario

### 1. Rapid initial exploration (short runs)

**Use**: Random Search

```yaml
strategy: random
max_random_samples: 20
```

**Example**: `examples/02_quick_random.yaml`

**Best for**:

- First-time model bring-up.
- Gauging rough parameter ranges.
- Quick prototypes.

---

### 2. Precision tuning (expanded search) ⭐ Recommended

**Use**: Bayesian Optimization

```yaml
strategy: bayesian

bayesian:
  n_calls: 50
  n_initial_points: 10
```

**Examples**:

- `examples/03_bayesian_quick.yaml` (≈20 trials).
- `examples/04_bayesian_standard.yaml` (≈50 trials) ⭐

**Best for**:

- The default tuning workflow.
- Practical optimization with time constraints.
- High-cost NPU evaluations.

---

### 3. Full analysis (complete enumeration)

**Use**: Grid Search

```yaml
strategy: grid
```

**Examples**:

- `examples/05_grid_small.yaml` (144 combinations).
- `examples/08_grid_full.yaml` (1,920 combinations).

**Best for**:

- Exhaustive studies.
- Detailed parameter impact reports.
- Research-grade investigations.
- Situations without strict time limits.

---

### 4. Thermal-sensitive workloads

**Use**: Bayesian + Thermal Management

```yaml
strategy: bayesian

thermal_management:
  enabled: true
  target_temperature: 65.0
```

**Example**: `examples/06_thermal_bayesian.yaml`

**Best for**:

- Long tuning sessions.
- Thermal-constrained hardware.
- Highly stable measurement requirements.

---

## 🎯 Summary

### Current status

| Strategy | Implementation | Priority |
|----------|----------------|----------|
| **Bayesian Optimization** | ✅ Complete | 🥇 1st |
| Random Search | ✅ Complete | 🥉 3rd |
| Grid Search | ✅ Complete | 🥈 2nd |

### Quick selection guide

- Need results immediately? Use `Random (02_quick_random.yaml)`.
- Want the most balanced approach? Pick `Bayesian (04_bayesian_standard.yaml)` ⭐.
- Require exhaustive coverage? Choose `Grid (08_grid_full.yaml)`.

### Measured performance (YOLOv5S example)

| Strategy | Representative runs | Characteristics | Peak FPS | Notes |
|----------|---------------------|-----------------|---------|-------|
| Bayesian | 30 | Environment-dependent | 178.79 | ⭐ Recommended |
| Random | 20 | Fast initial sampling | ≈150 | Great for quick scans |
| Grid Small | 144 | Enumerates all combos | – | Comprehensive but costly |
| Grid Full | 1,920 | Full-factor sweep | – | Deep dive analysis |

---

**Last updated**: 2025-10-16  
**Version**: 1.1.0 (Bayesian Optimization included)

## 🎯 Top Strategy Additions for DX-Fit

### 🏆 Priority Pick: Bayesian Optimization

#### Why it matters

1. NPU trials are expensive (seconds per run).
2. Parameter counts are moderate (2–10).
3. Minimizing samples directly saves wall-clock time.
4. Each evaluation consumes notable system resources.

#### Implementation priority

```python
# Add the dependency
pip install scikit-optimize

# Enable in dx-fit
strategy: "bayesian"
n_calls: 50
acquisition_function: "EI"  # Expected Improvement
```

#### Expected impact

- **Up to 90% time savings** compared to grid search.
- **Higher-quality optima** than random sampling.

---

### 🥈 Secondary Pick: Latin Hypercube Sampling

#### Why consider it

1. An easy upgrade over pure random sampling.
2. Simple to implement (requires SciPy only).
3. Can be rolled out immediately.
4. No compatibility drawbacks.

#### Implementation effort

- **Low**: ~10–20 lines of code.
- SciPy ships alongside pandas dependencies.

---

## 📝 Implementation Plan

### Phase 1: Latin Hypercube (short term)

```python
def _generate_lhs_combinations(self, n_samples):
    from scipy.stats import qmc

    param_names = list(self.parameters.keys())
    n_params = len(param_names)

    sampler = qmc.LatinHypercube(d=n_params)
    samples = sampler.random(n=n_samples)

    # Scale to the parameter space
    combinations = []
    for sample in samples:
        combo = {}
        for i, name in enumerate(param_names):
            values = self.parameters[name]
            idx = int(sample[i] * len(values))
            combo[name] = values[min(idx, len(values) - 1)]
        combinations.append(combo)

    return combinations
```

### Phase 2: Bayesian Optimization (mid term)

```python
def _bayesian_optimization(self, n_calls):
    from skopt import gp_minimize
    from skopt.space import Categorical

    # Define the search space
    space = []
    for name, values in self.parameters.items():
        space.append(Categorical(values, name=name))

    # Objective function
    def objective(params):
        param_dict = dict(zip(self.parameters.keys(), params))
        result = self._run_single_test(param_dict, 0)
        return -result.fps if result.fps else float("inf")

    # Optimize
    result = gp_minimize(
        objective, space,
        n_calls=n_calls,
        n_initial_points=5,
        acq_func="EI"
    )

    return result.x_iters
```

---

## 🎬 Final Thoughts

### Current capabilities

- ✅ Grid Search: Ideal for small, discrete spaces.
- ✅ Random Search: Practical for high-dimensional sweeps.

### Recommended enhancements

1. **Bayesian Optimization** ⭐⭐⭐⭐⭐
   - Highest efficiency.
   - Tailor-made for NPU tuning workloads.

2. **Latin Hypercube Sampling** ⭐⭐⭐⭐
   - Lightweight upgrade.
   - Smooth transition from random search.

### Priority roadmap

- Latin Hypercube (short-term scope).
- Bayesian Optimization (mid-term scope).
- Adaptive grid (optional future refinement).

---

**Authored on**: 2025-10-16  
**Version**: 1.0  
**References**: scikit-optimize, SciPy documentation
