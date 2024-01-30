from typing import List

from dx_engine import capi as C


class InferenceOption:
    def __init__(
        self,
        mode,
        num_npus: int,
        num_threads: int,
        num_buffering: int,
        reset_addr: bool,
        use_ppu: bool,
        npus: List[int],
    ) -> None:
        """
        Args:
            mode (InferenceMode):  inference mode
            num_npus (int):  number of devices to use (default 0 to apply number of automatically detected devices)
            num_threads (int):  number of threads to use for CPU task (default 1)
            num_buffering (int):  number of device memory buffers
                1 = for standalone device,
                2 = for accelerator device
            reset_addr (bool):  whether to reset device memory buffer address
            use_ppu (bool):  whether to use device PPU(Post-Processing Unit) (default enable for PPU-integrated device )
            npus (List[int]):  vector of NPU device IDs to use
        """

        self.mode = mode
        self.num_npus = num_npus
        self.num_threads = num_threads
        self.num_buffering = num_buffering
        self.reset_addr = reset_addr
        self.use_ppu = use_ppu
        self.npus = npus

    def summary(self) -> str:
        """List of attribute's label and data."""
        return self.engine.summary()
