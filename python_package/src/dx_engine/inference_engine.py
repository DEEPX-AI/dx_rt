from typing import List

import numpy as np

import dx_engine.capi._pydxrt as C
from dx_engine.dtype import NumpyDataTypeMapper


def raise_not_implemented_error() -> None:
    NOT_IMPLEMENTED_ERROR_MSG = "Not Pybinded Yet"
    raise NotImplementedError(NOT_IMPLEMENTED_ERROR_MSG)


class InferenceEngine:
    def __init__(
        self,
        model_path: str,
        inference_option=None,  # TODO: Will be used after pybind complete
    ) -> None:
        self.model_path = model_path
        self.inference_option = inference_option
        self.engine = C.InferenceEngine(model_path)

    def run(self, input_feed_list: List[np.ndarray]) -> List[np.ndarray]:
        """Return Normal Inference Result.

        Args:
            input_feed_list (List[np.ndarray]): List of Numpy array.

        Returns:
            List[np.ndarray]
        """
        # self.check_inputs(input_feed_list) # TODO - Will be uncommented after pybind complete
        return C.run_engine(self.engine, input_feed_list)

    def validate_device(self, input_feed_list: List[np.ndarray], device_id=0) -> List[np.ndarray]:
        """Return Device validation run result.

        Args:
            input_feed_list (List[np.ndarray]): List of Numpy array.

        Returns:
            List[np.ndarray]
        """
        # self.check_inputs(input_feed_list) # TODO - Will be uncommented after pybind complete
        return C.validate_device(self.engine, input_feed_list, device_id)

    def benchmark(self, loop_cnt=30) -> float:
        """Retrun Benchmark result.

        Returns:
            float: fps
        """
        return self.engine.benchmark(loop_cnt)

    def arun(self, input_feed_list: List[np.ndarray]) -> List[np.ndarray]:
        """Return Asynchronous Inference Result

        Args:
            input_feed_list (List[np.ndarray]): List of Numpy array.

        Returns:
            List[np.ndarray]
        """
        raise_not_implemented_error()  # TODO
        self.check_inputs(input_feed_list)
        return self.engine.arun(input_feed_list)

    def input_size(self) -> List[int]:
        """Get engine's input size."""
        return self.engine.input_size()

    def output_size(self) -> List[int]:
        """Get engine's output size."""
        return self.engine.output_size()

    def input_dtype(self) -> List[str]:
        """Get required input data-type as string"""
        # return self.engine.get_output_size()
        return C.input_dtype(self.engine)

    def output_dtype(self) -> List[str]:
        """Get required output data-type as string"""
        # return self.engine.get_output_size()
        return C.output_dtype(self.engine)

    def summary(self) -> str:
        """Concatenated List of attribute's label and data."""
        raise_not_implemented_error()  # TODO
        return self.engine.summary()

    def get_bitmatch_mask(self, index=0) -> np.ndarray:
        """Get bit match mask array in single file"""
        mask_list = np.array(self.engine.bitmatch_mask(index), dtype=np.uint8)
        mask = np.unpackbits(mask_list, bitorder="little")
        return mask

    def check_inputs(self, input_feed_list: List[np.ndarray]) -> None:
        """Check input if it is valid inputs.

            Check list
                1. Check if given len(input) are same as len(required_input).
                2. Check if given input's dtype are exact same as required inputs.

        Args:
            input_feed_list (List[np.ndarray]):
        """
        raise_not_implemented_error()  # TODO
        self._check_input_dtype(input_feed_list)
        self._check_input_length(input_feed_list)

    def _check_input_dtype(self, input_feed_list: List[np.ndarray]) -> None:
        """Compare input's data type with required data type.

        Args:
            input_feed_list (List[np.ndarray]): List of Numpy array.

        Raises:
            TypeError: Raise Error if given input's data type is differ from required dtype.
        """
        raise_not_implemented_error()  # TODO
        for required_input_dtype, actual_input in zip(self.input_dtype(), input_feed_list):
            actual_input_dtype = actual_input.dtype
            if NumpyDataTypeMapper[required_input_dtype] != actual_input_dtype:
                raise TypeError(f"Required {required_input_dtype} but {actual_input_dtype} was given")

    def _check_input_length(self, input_feed_list: List[np.ndarray]):
        raise_not_implemented_error()  # TODO
        if len(self.input_size()) == len(input_feed_list):
            raise ValueError(f"Required {len(self.input_size())} inputs but {len(input_feed_list)} is(are) given.")

    def latency(self):
        return self.engine.latency()

    def inf_time(self):
        return self.engine.inf_time()
