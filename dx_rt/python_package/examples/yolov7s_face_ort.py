import cv2
import numpy as np
import onnxruntime as ort

# Load model
model_path = "/dxrt/cpuoffloading/YOLOv7s_Face"
onnx_file = model_path+"/YOLOv7s_Face.onnx"
dxnn_file = model_path+"/graph.dxnn"
input_image = model_path+"/input.png"
output_image = model_path+"/output.png"

sess = ort.InferenceSession(onnx_file)

# Load image
image = cv2.imread(input_image)
image = cv2.resize(image, (640, 640))
view = image.copy()

# Preprocess image
x = cv2.cvtColor(image, cv2.COLOR_BGR2RGB)
x = x.astype(np.float32) / 255.0
x = np.transpose(x, [2, 0, 1])
x = np.expand_dims(x, axis=0)
print(x.shape)  # (1, 3, 640, 640)

# Run model
out = sess.run(None, {sess.get_inputs()[0].name: x})[0]
out = out[0]
print(out.shape)  # (25200, 21)

# Postprocess
conf_thres = 0.5
kpt_thres = 0.5
color = (0, 255, 0)
thickness = 1
for o in out:
    conf = o[4]
    if conf > conf_thres:
        # draw a box
        x, y, w, h = o[:4].astype(int)
        x1 = x - w // 2
        y1 = y - h // 2
        x2 = x + w // 2
        y2 = y + h // 2
        image = cv2.rectangle(view, (x1, y1), (x2, y2), color, thickness)

        # draw keypoints
        kpts = o[6:]
        print(kpts.shape)  # (15,)
        kpts = kpts.reshape(5, 3)
        for kpt in kpts:
            kc = kpt[2]
            if kc > kpt_thres:
                kx, ky = kpt[:2].astype(int)
                cv2.circle(image, (kx, ky), 3, color, thickness)

# Save the result image
cv2.imwrite(output_image, view)
