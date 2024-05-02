import numpy as np
p0 = np.load('prior_0-1.npy')
p1 = np.load('prior_2-3.npy')
p=np.concatenate((p0,p1), axis=2)
p.tofile('prior_boxes.bin')