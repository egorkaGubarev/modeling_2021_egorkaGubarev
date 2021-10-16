import numpy as np
import matplotlib.pyplot as plt

data_sets_amount: int = 8
strings_amount_in_data_set = 5
path: str = "C:/Programs/Git/repositories/modeling_2021_egorkaGubarev/simple_task/calculations/result.txt"
symbol_split: str = ";"

figure, ((ax1, ax2, ax3), (ax4, ax5, ax6)) = plt.subplots(2, 3)
energy_delta_list: list = []
time_step_list: list = []
with open(path, 'r') as file:
    for data_set_number in range(data_sets_amount):
        data_list: list = []
        for string_number in range(strings_amount_in_data_set):
            string: str = file.readline()
            array = np.array(string.split(symbol_split))
            data_size: int = array.size - 1
            array_data = array[0:data_size]
            numbers: list = list(map(float, array_data))
            data_list.append(numbers)
        x: list = data_list[0]
        v: list = data_list[1]
        energy: list = data_list[2]
        time: list = data_list[3]
        time_step: list = data_list[4]
        time_step_ln: float = np.log(time_step)
        energy_start: float = energy[0]
        energy_end: float = energy[-1]
        energy_delta: float = energy_end - energy_start
        energy_delta_ln: float = np.log(energy_delta)
        energy_delta_list.append(energy_delta_ln)
        time_step_list.append(time_step_ln)
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
ax4.set_label('E(t)')
ax4.set_xlabel('t')
ax4.set_ylabel('E')
ax5.plot(time_step_list, energy_delta_list)
ax5.set_label('ErrorE(t)')
ax5.set_xlabel('ln(t)')
ax5.set_ylabel('ln(ErrorE)')
plt.show()
