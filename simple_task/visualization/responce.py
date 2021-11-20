import numpy as np
import matplotlib.pyplot as plt


def get_amplitude(x_list: list):
    size: int = len(x_list)
    i: int = size - 2
    is_done: bool = False
    is_max: bool = False
    is_min: bool = False
    min_x: float = 0
    max_x: float = 0
    while not is_done:
        i_last: int = i + 1
        i_next: int = i - 1
        x_current: float = x_list[i]
        x_last: float = x_list[i_last]
        x_next: float = x_list[i_next]
        if x_current < x_last and x_current < x_next and not is_min:
            min_x = x_current
            is_min = True
        if x_current > x_last and x_current > x_next and not is_max:
            max_x = x_current
            is_max = True
        if is_min and is_max:
            is_done = True
        i -= 1
    delta: float = max_x - min_x
    amplitude_local: float = delta / 2
    return amplitude_local


amount: int = 190
frequency_start: float = 0.1
frequency_step: float = 0.01
frequency_end: float = 2
symbol_split: str = ";"
path_pattern: str = "C:/Programs/Git/repositories/modeling_2021_egorkaGubarev/simple_task/calculations/" \
            "force_equation_response_heun_method_"

amplitudes = np.zeros(amount)
frequencies = np.arange(frequency_start, frequency_end, frequency_step)
for file in range(amount):
    path: str = path_pattern + str(file) + ".txt"
    data = np.loadtxt(path)
    time = data[:, 0]
    x = data[:, 1]
    amplitude: float = get_amplitude(x)
    amplitudes[file] = amplitude
plt.plot(frequencies, amplitudes)
plt.title('xm(w)')
plt.xlabel('w, rad / s')
plt.ylabel('xm, m')
plt.show()
