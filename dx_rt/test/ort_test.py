import os, sys
import onnxruntime as ort
import numpy as np
import cv2

# os.system("cd /home/yr/git/dxnn/app/test_ort")

if len(sys.argv)==1:
    onnxFileName = '/home/yr/git/dxrt/test/data/yolov5s_512/cpu_t.onnx'
else:
    onnxFileName = sys.argv[1]
session = ort.InferenceSession(onnxFileName)

inputs = {}
inputInfos = session.get_inputs()
outputInfos = session.get_outputs()
for inputinfo in inputInfos:
    print(inputinfo)
    input = np.ndarray(inputinfo.shape, dtype=np.float32)    
    inputs[inputinfo.name] = input
for outputinfo in outputInfos:
    print(outputinfo)

outputs = session.run(None, inputs)
# print(input)

# ort_inputs = {session.get_inputs()[0].name: img[None, :, :, :]}

# tmp = session.get_inputs()

# output = session.run(None, ort_inputs)
for output in outputs:
    print(output.shape)
print("output type is ", type(outputs))