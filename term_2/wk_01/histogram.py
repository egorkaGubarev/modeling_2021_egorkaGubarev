import numpy as np
import matplotlib.pyplot as plt

name: str = "result.txt"
data = np.loadtxt(name)
uniform = data[0]
normal = data[1]
parabolic = data[2]
plt.hist(parabolic)
plt.show()
