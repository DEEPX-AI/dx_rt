import dx_engine.capi._pydxrt as C


def parse_model(model_path) -> str:
    return C.parse_model(model_path)
