import numpy as np
import matplotlib.pyplot as plt

symbol_split: str = ";"
path: str = "C:/Programs/Git/repositories/modeling_2021_egorkaGubarev/simple_task/calculations/" \
            "simple_equation_euler_method.txt"

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
plt.show()
