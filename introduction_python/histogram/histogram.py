import numpy as np
import matplotlib.pyplot as plt

points_amount: int = 200
time_start: float = 0
time_end: float = 2 * np.pi

x_frequency: float = 1
x_max: float = 1
x_phase_start: float = 0

y_frequency: float = 2
y_max: float = 1
y_phase_start: float = np.pi / 2

time = np.linspace(time_start, time_end, points_amount)
x = x_max * np.sin(x_frequency * time + x_phase_start)
y = y_max * np.sin(y_frequency * time + y_phase_start)
figure, (histogram_left, histogram_right) = plt.subplots(1, 2)
figure.suptitle('Lissajous histogram')
histogram_left.set_title('x')
histogram_left.set_xlabel('x')
histogram_left.set_ylabel('Amount')
histogram_right.set_title('y')
histogram_right.set_xlabel('y')
histogram_right.set_ylabel('Amount')
histogram_left.hist(x)
histogram_right.hist(y)
plt.show()
