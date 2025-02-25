from dataclasses import dataclass

@dataclass
class TestConfig:
    use_ort: bool = False
    batch_mode: bool = False
    iterations: int = 1
    verbose: bool = True
    log_enabled: bool = True