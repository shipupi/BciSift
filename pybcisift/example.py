import pybcisift
import numpy as np
el = np.zeros((1,256))
el[0,23]=200
descr = pybcisift.extract(el.tolist()[0])
print(descr)
print(len(descr))



