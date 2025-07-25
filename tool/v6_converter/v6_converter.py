import json
from dataclasses import asdict, dataclass
from types import NoneType
from typing import Any, Dict, List
from dx_common.singlefile import CompiledDataWrapperFactory, SingleFile


def _validate_type(value: Any, expected_type: type, field_name: str):
    if not isinstance(value, expected_type):
        raise TypeError(
            f"Expected {expected_type} for '{field_name}', but got {type(value).__name__}"
        )


def _validate_list_type(value: List[Any], expected_item_type: type, field_name: str):
    _validate_type(value, list, field_name)
    for item in value:
        _validate_type(item, expected_item_type, f"element in '{field_name}'")


@dataclass
class MemoryInfo:
    name: str
    offset: int
    size: int
    type: str

    @classmethod
    def from_dict(cls, data: Dict[str, Any]) -> "MemoryInfo":
        name = data.get("name")
        offset = data.get("offset")
        size = data.get("size")
        type_ = data.get("type")

        _validate_type(name, str, "name")
        _validate_type(offset, int, "offset")
        _validate_type(size, int, "size")
        _validate_type(type_, str, "type")

        return cls(name=name, offset=offset, size=size, type=type_)


@dataclass
class TensorInfo:
    name: str
    dtype: str
    shape: List[int]
    name_encoded: str
    dtype_encoded: str
    shape_encoded: List[int]
    layout: str
    align_unit: int
    transpose: str
    scale: float
    bias: float
    memory: MemoryInfo

    @classmethod
    def from_dict(cls, data: Dict[str, Any]) -> "TensorInfo":
        name = data.get("name")
        dtype = data.get("dtype")
        shape = data.get("shape")
        name_encoded = data.get("name_encoded")
        dtype_encoded = data.get("dtype_encoded")
        shape_encoded = data.get("shape_encoded")
        layout = data.get("layout")
        align_unit = data.get("align_unit")
        transpose = data.get("transpose")
        scale = data.get("scale")
        bias = data.get("bias")
        memory_data = data.get("memory")

        _validate_type(name, str, "name")
        _validate_type(dtype, str, "dtype")
        _validate_list_type(shape, int, "shape")
        _validate_type(name_encoded, str, "name_encoded")
        _validate_type(dtype_encoded, str, "dtype_encoded")
        _validate_list_type(shape_encoded, int, "shape_encoded")
        _validate_type(layout, str, "layout")
        _validate_type(align_unit, int, "align_unit")
        _validate_type(transpose, (str, NoneType), "transpose")
        _validate_type(scale, (float, NoneType), "scale")
        _validate_type(bias, (float, NoneType), "bias")
        _validate_type(memory_data, dict, "memory")
        memory = MemoryInfo.from_dict(memory_data)

        return cls(
            name=name,
            dtype=dtype,
            shape=shape,
            name_encoded=name_encoded,
            dtype_encoded=dtype_encoded,
            shape_encoded=shape_encoded,
            layout=layout,
            align_unit=align_unit,
            transpose=transpose,
            scale=scale,
            bias=bias,
            memory=memory,
        )


@dataclass
class RmapInfo:
    version: Dict[str, str]
    name: str
    mode: str
    npu: Dict[str, int]
    size: int
    counts: Dict[str, int]
    memory: List[MemoryInfo]
    inputs: List[TensorInfo]
    outputs: List[TensorInfo]

    @classmethod
    def from_dict(cls, data: Dict[str, Any]) -> "RmapInfo":
        version = data.get("version")
        name = data.get("name")
        mode = data.get("mode")
        npu = data.get("npu")
        size = data.get("size")
        counts = data.get("counts")
        memory_data = data.get("memory")
        inputs_data = data.get("inputs")
        outputs_data = data.get("outputs")

        _validate_type(version, dict, "version")
        _validate_type(name, str, "name")
        _validate_type(mode, str, "mode")
        _validate_type(npu, dict, "npu")
        _validate_type(size, int, "size")
        _validate_type(counts, dict, "counts")
        _validate_list_type(memory_data, dict, "memory")
        _validate_list_type(inputs_data, dict, "inputs")
        _validate_list_type(outputs_data, dict, "outputs")

        memory = [MemoryInfo.from_dict(mem) for mem in memory_data]
        inputs = [TensorInfo.from_dict(input) for input in inputs_data]
        outputs = [TensorInfo.from_dict(output) for output in outputs_data]

        return cls(
            version=version,
            name=name,
            mode=mode,
            npu=npu,
            size=size,
            counts=counts,
            memory=memory,
            inputs=inputs,
            outputs=outputs,
        )

    def to_dict(self) -> dict:
        return asdict(self)

    @classmethod
    def from_json(cls, json_str: str) -> "RmapInfo":
        return cls.from_dict(json.loads(json_str))

    def to_json(self, indent: int = 2) -> str:
        return json.dumps(self.to_dict(), indent=indent)

    @classmethod
    def from_file(cls, file_path: str) -> "RmapInfo":
        with open(file_path, "r") as file:
            return cls.from_json(file.read())

    def to_file(self, file_path: str) -> None:
        with open(file_path, "w") as file:
            file.write(self.to_json())


def convert_rmap_info(rmap_info: str, graph_info: str) -> str:
    rmap_info = json.loads(rmap_info)
    graph_info = json.loads(graph_info)
    for graph in graph_info["graphs"]:
        if graph["name"] == "npu_0":
            npu_graph = graph
            break

    input_name, input_tensor = list(npu_graph["inputs"].items())[0]
    input_shape = input_tensor["shape"]

    version = rmap_info["version"]
    cg_version = version["rmapInfo"]
    cg_version, opt_level = cg_version.split("(")
    opt_level = opt_level = opt_level[:-1]
    name = rmap_info["model"]
    mode = rmap_info["mode"]
    npu = rmap_info["npu"]
    size = int(rmap_info["size"])
    counts = rmap_info["counts"]
    input = rmap_info["input"]

    version = dict(
        npu=version["npu"],
        rmap=version["rmap"],
        rmapInfo=cg_version,
        opt_level=opt_level,
    )

    memory_input = MemoryInfo(
        name="INPUT",
        offset=int(input["memory"].get("offset", 0)),
        size=int(input["memory"].get("size", 0)),
        type=input["memory"]["type"],
    )
    inputs = [
        TensorInfo(
            name=input_name,
            dtype=input["type"],
            shape=input_shape,
            name_encoded=input_name,
            dtype_encoded=input["type"],
            shape_encoded=input_shape,
            layout="NONE",
            align_unit=1,
            transpose=None,
            scale=None,
            bias=None,
            memory=memory_input,
        )
    ]
    memory_output_all = MemoryInfo(
        name="OUTPUT",
        offset=int(rmap_info["outputs"]["memory"].get("offset", 0)),
        size=int(rmap_info["outputs"]["memory"].get("size", 0)),
        type=rmap_info["outputs"]["memory"]["type"],
    )
    outputs = []
    for value in rmap_info["outputs"]["outputList"]["output"]:
        layout = value["format"]
        if layout not in ["PPU_YOLO", "PPU_FD", "PPU_POSE"]:
            layout = "NONE"
            
        shape = npu_graph["outputs"][value["name"]]["shape"]
        outputs.append(
            TensorInfo(
                name=value["name"],
                dtype=value["type"],
                shape=shape,
                name_encoded=value["name"],
                dtype_encoded=value["type"],
                shape_encoded=shape,
                layout=layout,
                align_unit=1,
                transpose=None,
                scale=None,
                bias=None,
                memory=MemoryInfo(
                    name="OUTPUT",
                    offset=int(value["memory"].get("offset", 0)),
                    size=int(value["memory"].get("size", 0)),
                    type=value["memory"]["type"],
                ),
            )
        )
    memory = [
        memory_input,
        memory_output_all,
    ]
    for mem in rmap_info["memorys"]["memory"]:
        memory.append(
            MemoryInfo(
                name=mem["name"],
                offset=int(mem.get("offset", 0)),
                size=int(mem.get("size", 0)),
                type="DRAM",
            )
        )
    rmap_info = RmapInfo(
        version=version,
        name=name,
        mode=mode,
        npu=npu,
        size=size,
        counts=counts,
        memory=memory,
        inputs=inputs,
        outputs=outputs,
    )
    return rmap_info.to_json()


@dataclass
class ValueInfo:
    name: str
    owner: str
    users: List[str]

    @classmethod
    def from_dict(cls, data: Dict[str, Any]) -> "ValueInfo":
        name = data.get("name")
        owner = data.get("owner")
        users = data.get("users")

        _validate_type(name, str, "name")
        _validate_type(owner, (str, NoneType), "owner")
        _validate_list_type(users, str, "users")

        return cls(name=name, owner=owner, users=users)


@dataclass
class SubGraph:
    name: str
    device: str
    inputs: List[ValueInfo]
    outputs: List[ValueInfo]

    @classmethod
    def from_dict(cls, data: Dict[str, Any]) -> "SubGraph":
        name = data.get("name")
        device = data.get("device")
        inputs_data = data.get("inputs")
        outputs_data = data.get("outputs")

        _validate_type(name, str, "name")
        _validate_type(device, str, "device")
        _validate_list_type(inputs_data, dict, "inputs")
        _validate_list_type(outputs_data, dict, "outputs")
        inputs = [ValueInfo.from_dict(item) for item in inputs_data]
        outputs = [ValueInfo.from_dict(item) for item in outputs_data]
        return cls(name=name, device=device, inputs=inputs, outputs=outputs)


@dataclass
class GraphInfo:
    offloading: bool
    inputs: List[str]
    outputs: List[str]
    toposort_order: List[str]
    graphs: List[SubGraph]

    @classmethod
    def from_dict(cls, data: Dict[str, Any]) -> "GraphInfo":
        offloading = data.get("offloading")
        inputs = data.get("inputs")
        outputs = data.get("outputs")
        toposort_order = data.get("toposort_order")
        graphs_data = data.get("graphs")

        _validate_type(offloading, bool, "offloading")
        _validate_list_type(inputs, str, "inputs")
        _validate_list_type(outputs, str, "outputs")
        _validate_list_type(toposort_order, str, "toposort_order")
        _validate_list_type(graphs_data, dict, "graphs")
        graphs = [SubGraph.from_dict(item) for item in graphs_data]
        return cls(
            offloading=offloading,
            inputs=inputs,
            outputs=outputs,
            toposort_order=toposort_order,
            graphs=graphs,
        )

    def to_dict(self) -> dict:
        return asdict(self)

    @classmethod
    def from_json(cls, json_str: str) -> "GraphInfo":
        return cls.from_dict(json.loads(json_str))

    def to_json(self) -> str:
        return json.dumps(self.to_dict(), indent=2)

    @classmethod
    def from_file(cls, file_path: str) -> "GraphInfo":
        with open(file_path, "r") as file:
            return cls.from_json(file.read())

    def to_file(self, file_path: str) -> None:
        with open(file_path, "w") as file:
            file.write(self.to_json())


def convert_graph_info(graph_info: str) -> str:
    graph_info = json.loads(graph_info)
    graphs = []
    for subgraph in graph_info["graphs"]:
        inputs = []
        for name, value in subgraph["inputs"].items():
            inputs.append(
                ValueInfo(
                    name=name,
                    owner=value["source"],
                    users=[subgraph["name"]],
                )
            )
        outputs = []
        for name, value in subgraph["outputs"].items():
            outputs.append(
                ValueInfo(
                    name=name,
                    owner=subgraph["name"],
                    users=value["next_layers"],
                )
            )
        graphs.append(
            SubGraph(
                name=subgraph["name"],
                device=subgraph["type"],
                inputs=inputs,
                outputs=outputs,
            )
        )
    graph_info = GraphInfo(
        offloading=graph_info["offloading"],
        inputs=graph_info["origin_input"],
        outputs=graph_info["origin_output"],
        toposort_order=graph_info["toposort_order"],
        graphs=graphs,
    )
    return graph_info.to_json()


def convert_interface_v6_to_v7(
    sf: SingleFile,
    converting_graph_info: bool = True,
    converting_rmap_info: bool = True,
) -> SingleFile:
    graph_info = sf.graph_info
    npu_models = sf.npu_models
    cpu_models = sf.cpu_models
    compiled_data = sf.compiled_data
    vis_npu_models = sf.vis_npu_models
    compile_config = sf.compile_config

    graph_info = json.dumps(graph_info)
    sf2 = SingleFile(version=7, hw_config="M1A_4K")
    sf2.set_compile_config(compile_config)
    sf2.set_graph_info(graph_info)
    for name, model in npu_models.items():
        sf2.add_npu_model(model, name)
    for name, model in cpu_models.items():
        sf2.add_cpu_model(model, name)
    for name, model in vis_npu_models.items():
        sf2.add_vis_npu_model(model, name)
    for hw_config, pack1 in compiled_data.items():
        for name, pack2 in pack1.items():
            rmap_info = pack2["rmap_info"]
            if converting_rmap_info:
                rmap_info = convert_rmap_info(
                    rmap_info=rmap_info,
                    graph_info=graph_info,
                )
            pack2["rmap_info"] = rmap_info
            pack2["dummy_mask"] = pack2["bitmatch"]
            compiled = CompiledDataWrapperFactory.create_wrapper(
                sf.version, pack2, False, False
            )
            sf2.add_compiled_data(hw_config, compiled, name)
    if converting_graph_info:
        graph_info = convert_graph_info(graph_info=graph_info)
    sf2.set_graph_info(graph_info=graph_info)
    return sf2


if __name__ == "__main__":
    import argparse

    parser = argparse.ArgumentParser("Singlefile Interface Converter")
    parser.add_argument("input", help="input singlefile (*.dxnn)")
    parser.add_argument("output", help="output singlefile (*.dxnn)")
    args = parser.parse_args()

    sf = SingleFile()
    sf.load(args.input)
    sf = convert_interface_v6_to_v7(sf)
    sf.save(args.output)