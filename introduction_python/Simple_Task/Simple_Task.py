import matplotlib.pyplot as plt
import numpy as np

R: float = 6371000
b_start: float = 0
time_end: float = 100
time_step: float = 0.1
k: int = 1


def get_sequence(a_start: float, b_start_local: float, time_end_local: float, time_step_local: float, k_local: int):
    iterations_amount = int(time_end_local / time_step_local) + 2
    a = np.empty(iterations_amount)
    b = np.empty(iterations_amount)
    a[0]: float = a_start
    b[0]: float = b_start_local
    iteration: int = 1
    time: float = 0
    while time < time_end_local:
        a[iteration]: float = a[iteration - 1] + b[iteration - 1] * time_step_local
        b[iteration]: float = b[iteration - 1] - a[iteration - 1] * k_local * time_step_local
        time: float = time + time_step_local
        iteration: int = iteration + 1
    plot: tuple = (a, b)
    return plot


(x, y) = get_sequence(R, b_start, time_end, time_step, k)
plt.plot(x, y)
plt.show()
