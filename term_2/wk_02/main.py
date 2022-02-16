import numpy as np
import matplotlib.pyplot as plt

path: str = "../../CLionProjects/wk_02/cmake-build-debug/result.txt"

data = np.loadtxt(path, unpack=True)
tests = data[0]
probability = data[1]
correct = data[2]
plt.plot(tests, probability)
plt.plot(tests, correct)
plt.xlabel("Tests")
plt.ylabel("Probability of full stacks")
plt.legend(["Numeric", "Analytic"])
plt.show()
