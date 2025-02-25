from dataclasses import dataclass, field
from typing import List

@dataclass
class TestStatistics:
    pass_count: int = 0
    total_count: int = 0
    failed_jobs: List[int] = field(default_factory=list)
    execution_times: List[float] = field(default_factory=list)
    latency_mean: float = 0.0
    latency_stddev: float = 0.0
    latency_CV: float = 0.0
    inf_time_mean: float = 0.0
    inf_time_stddev: float = 0.0
    inf_time_CV: float = 0.0