import matplotlib.pyplot as plt
import numpy as np


def get_lissajous_figure(x_max: float, y_max: float, x_frequency: float, y_frequency, x_phase: float, y_phase):
    points_amount: int = 200
    time_start: float = 0
    divider: int = np.gcd(x_frequency, y_frequency)
    x_frequency_norm: float = x_frequency / divider
    y_frequency_norm: float = y_frequency / divider
    time = np.linspace(time_start, 2 * np.pi, points_amount)
    x = x_max * np.sin(x_frequency_norm * time + x_phase)
    y = y_max * np.sin(y_frequency_norm * time + y_phase)
    plot: tuple = (x, y)
    return plot


x_1_frequency: float = 3
x_1_max: float = 1
x_1_phase: float = 0
y_1_frequency: float = 7
y_1_max: float = 1
y_1_phase: float = np.pi / 2

plot_1: tuple = get_lissajous_figure(x_1_max, y_1_max, x_1_frequency, y_1_frequency, x_1_phase, y_1_phase)
(x_1, y_1) = plot_1
plt.suptitle('Lissajous curve')
plt.plot(x_1, y_1)
plt.show()
