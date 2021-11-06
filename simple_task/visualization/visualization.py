import numpy as np
import matplotlib.pyplot as plt


def get_decrement(x_list: list):
    amount_peaks: int = 0
    decrement_sum: float = 0
    x_max_last: float = -1
    size: int = len(x_list)
    for i in range(1, size - 1):
        i_last: int = i - 1
        i_next: int = i + 1
        x_current: float = x_list[i]
        x_last: float = x_list[i_last]
        x_next: float = x_list[i_next]
        if x_current > x_last:
            if x_current > x_next:
                if x_max_last == -1:
                    x_max_last: float = x_current
                else:
                    x_max_current: float = x_current
                    ratio: float = x_max_last / x_max_current
                    decrement_local: float = np.log(ratio)
                    decrement_sum += decrement_local
                    amount_peaks += 1
                    x_max_last: float = x_max_current
    if amount_peaks > 0:
        average: float = decrement_sum / amount_peaks
        return average
    else:
        return -1


def get_periods_amount(v_list: list):
    half_periods: int = 0
    size: int = len(v_list)
    for i in range(0, size - 1):
        i_next: int = i + 1
        v_current: float = v_list[i]
        v_next: float = v_list[i_next]
        sign: float = v_current * v_next
        if sign < 0:
            half_periods += 1
    periods_local: float = half_periods / 2
    return periods_local


is_analytical_solution_available: bool = True
symbol_split: str = ";"
path: str = "C:/Programs/Git/repositories/modeling_2021_egorkaGubarev/simple_task/calculations/" \
            "simple_friction_equation_heun_method.txt"
path_analytical: str
if is_analytical_solution_available:
    path_analytical: str = "C:/Programs/Git/repositories/modeling_2021_egorkaGubarev/simple_task/calculations/" \
                "simple_friction_equation_analytical.txt"
else:
    path_analytical: str = ''

figure, ((ax1, ax2), (ax3, ax4)) = plt.subplots(2, 2)
figure.tight_layout()
data = np.loadtxt(path)
time = data[:, 0]
x = data[:, 1]
v = data[:, 2]
energy = data[:, 3]
periods: float = get_periods_amount(v)
print("Periods:", periods)
time_total: float = time[-1]
period: float = time_total / periods
frequency: float = 2 * np.pi / period
frequency_round: float = round(frequency, 2)
print("Frequency:", frequency_round, "Hz")
decrement: float = get_decrement(x)
if decrement > 0:
    quality: float = np.pi / decrement
    quality_round: float = round(quality, 2)
    print("Quality:", quality_round)
ax1.plot(time, x)
ax2.plot(time, v)
ax3.plot(x, v)
ax4.plot(time, energy)
if is_analytical_solution_available:
    data_analytical = np.loadtxt(path_analytical)
    time_analytical = data_analytical[:, 0]
    x_analytical = data_analytical[:, 1]
    v_analytical = data_analytical[:, 2]
    energy_analytical = data_analytical[:, 3]
    ax1.plot(time_analytical, x_analytical)
    ax2.plot(time_analytical, v_analytical)
    ax3.plot(x_analytical, v_analytical)
    ax4.plot(time_analytical, energy_analytical)
ax1.set_title('x(t)')
ax1.set_xlabel('t, s')
ax1.set_ylabel('x, rad')
ax2.set_title('v(t)')
ax2.set_xlabel('t, s')
ax2.set_ylabel('v, rad / s')
ax3.set_title('v(x)')
ax3.set_xlabel('x, rad')
ax3.set_ylabel('v, rad / s')
ax4.set_title('E(t)')
ax4.set_xlabel('t, s')
ax4.set_ylabel('E, J / (kg * m^2)')
plt.show()
