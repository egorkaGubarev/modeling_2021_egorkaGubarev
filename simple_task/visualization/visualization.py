import numpy as np
import matplotlib.pyplot as plt

<<<<<<< Updated upstream
path: str = "C:\\Programs\\Git\\repositories\\modeling_2021_egorkaGubarev\\simple_task\\calculations\\result.txt"
=======
numbers_in_string: int = 4
>>>>>>> Stashed changes
symbol_split: str = ";"
path: str = "C:/Programs/Git/repositories/modeling_2021_egorkaGubarev/simple_task/calculations/" \
            "simple_equation_euler_method.txt"

<<<<<<< Updated upstream
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
=======
figure, ((ax1, ax2, ax3), (ax4, ax5, ax6)) = plt.subplots(2, 3)
figure.tight_layout()
data = np.loadtxt(path)
time = data[:, 0]
x = data[:, 1]
v = data[:, 2]
energy = data[:, 3]
ax1.plot(time, x)
ax2.plot(time, v)
ax3.plot(x, v)
ax4.plot(time, energy)
ax1.set_title('x(t)')
ax1.set_xlabel('t')
ax1.set_ylabel('x')
ax2.set_title('v(t)')
ax2.set_xlabel('t')
ax2.set_ylabel('v')
ax3.set_title('v(x)')
ax3.set_xlabel('x')
ax3.set_ylabel('v')
ax4.set_title('E(t)')
ax4.set_xlabel('t')
ax4.set_ylabel('E')
>>>>>>> Stashed changes
plt.show()
