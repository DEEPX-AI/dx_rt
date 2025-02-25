import numpy as np
import subprocess
import re

def int8tofloat32(arr: np.ndarray) -> np.ndarray:
    """
    int8 배열을 float32 배열로 변환합니다.
    """
    return np.frombuffer(arr.tobytes(), dtype=np.float32)

def get_dxrt_info():
    # dxrt-cls -s 커맨드 실행
    result = subprocess.run(
        "dxrt-cli -s",
        shell=True,
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
        text=True
    )
    output = result.stdout

    info = {}

    # 1. Memory 속도 추출 (예: "* Memory : LPDDR5 6400 MHz, 3.92GiB")
    mem_match = re.search(r'\* Memory\s*:\s*\S+\s+(\d+)\s+MHz', output)
    if mem_match:
        info['memory_speed_MHz'] = int(mem_match.group(1))
    else:
        info['memory_speed_MHz'] = None

    # 2. PCIe 정보 추출 (예: "* PCIe   : Gen0 X2 [00:00:00]")
    pcie_match = re.search(r'\* PCIe\s*:\s*([^\[]+)\[', output)
    if pcie_match:
        info['pcie'] = pcie_match.group(1).strip()
    else:
        info['pcie'] = None

    # 3. NPU 정보 추출 (예: "NPU 0: voltage 750 mV, clock 1000 MHz, temperature 37'C")
    npus = []
    npu_pattern = re.compile(
        r'NPU\s+\d+:\s+voltage\s+(\d+)\s+mV,\s+clock\s+(\d+)\s+MHz,\s+temperature\s+(\d+)'
    )
    for match in npu_pattern.finditer(output):
        npu_info = {
            'voltage_mV': int(match.group(1)),
            'clock_MHz': int(match.group(2)),
            'temperature_C': int(match.group(3))
        }
        npus.append(npu_info)
    
    info['npus'] = npus

    return info