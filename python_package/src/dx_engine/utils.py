import numpy as np
import warnings
from typing import List
import dx_engine.capi._pydxrt as C


def parse_model(model_path) -> str:
    return C.parse_model(model_path)


def ensure_contiguous(input_feed_list: List[np.ndarray]) -> List[np.ndarray]:
    """Ensure all input arrays are contiguous, issuing a warning if conversion is needed."""
    contiguous_input = []
    
    for i, inp in enumerate(input_feed_list):
        if not inp.flags['C_CONTIGUOUS']:
            warnings.warn(
                f"Warning: Input at index {i} is not contiguous. "
                f"Automatically converting to a contiguous array, which may introduce memory copy overhead and reduce performance.",
                UserWarning
            )
            try:
                contiguous_input.append(np.ascontiguousarray(inp))
            except MemoryError:
                raise MemoryError(
                    f"Failed to allocate contiguous memory for input shape {inp.shape}, dtype {inp.dtype}. "
                    "Reduce input size or optimize memory usage."
                )
        else:
            contiguous_input.append(inp)

    return contiguous_input
