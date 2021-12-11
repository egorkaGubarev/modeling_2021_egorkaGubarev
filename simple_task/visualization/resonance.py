import numpy as np
import matplotlib.pyplot as plt


def get_amplitude(x_list):
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


path_frequency: str = "C:/Programs/Git/repositories/modeling_2021_egorkaGubarev/simple_task/calculations/resonance/"\
    "frequency/frequency.txt"
path_friction: str = "C:/Programs/Git/repositories/modeling_2021_egorkaGubarev/simple_task/calculations/resonance/"\
    "friction/friction.txt"
path_pattern: str = "C:/Programs/Git/repositories/modeling_2021_egorkaGubarev/simple_task/calculations/resonance/"\
    "result/result_"
frequency = np.loadtxt(path_frequency)
friction = np.loadtxt(path_friction)
frequencies: int = len(frequency)
frictions: int = len(friction)
paths = np.array([path_pattern])
resonance_frequency: float = 0
for path_pattern in paths:
    resonance_frequencies = np.zeros(frictions)
    for friction_number in range(frictions):
        amplitudes = np.zeros(frequencies)
        for file in range(frequencies):
            path: str = path_pattern + str(friction_number) + '_' + str(file) + ".txt"
            data = np.loadtxt(path)
            time = np.array(data[:, 0])
            x = np.array(data[:, 1])
            amplitude: float = get_amplitude(x)
            amplitudes[file] = amplitude
        resonance_index: int = amplitudes.argmax()
        resonance_frequency: float = frequency[resonance_index]
        resonance_frequencies[friction_number] = resonance_frequency
    plt.plot(friction, resonance_frequencies)
    plt.title('frequency vs friction')
    plt.xlabel('friction, rad / s')
    plt.ylabel('frequency, rad / s')
    plt.show()
