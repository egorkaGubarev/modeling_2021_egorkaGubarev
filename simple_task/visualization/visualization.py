import numpy as np
import matplotlib.pyplot as plt

path: str = "C:\\Programs\\Git\\repositories\\modeling_2021_egorkaGubarev\\simple_task\\calculations\\result.txt"
symbol_split: str = ";"

with open(path, 'r') as file:
    a_str: str = file.readline()
    b_str: str = file.readline()
a = np.array(a_str.split(symbol_split))
b = np.array(b_str.split(symbol_split))
a = a[0:a.size - 1]
b = b[0:b.size - 1]
a = list(map(float, a))
b = list(map(float, b))
plt.plot(a, b)
plt.xlabel('a')
plt.ylabel('b')
plt.show()
