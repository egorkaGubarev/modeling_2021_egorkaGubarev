import numpy as np
import matplotlib.pyplot as plt


def get_decrement(x_list):
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


def get_periods_amount(v_list):
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


symbol_split: str = ";"
path: str = "C:/Programs/Git/repositories/modeling_2021_egorkaGubarev/simple_task/calculations/solve/result/result.txt"
path_analytic: str = "C:/Programs/Git/repositories/modeling_2021_egorkaGubarev/simple_task/calculations/analytic/" \
                     "result/result.txt"
figure, ((ax1, ax2), (ax3, ax4)) = plt.subplots(2, 2)
figure.tight_layout()
data = np.loadtxt(path)
time = np.array(data[:, 0])
x = np.array(data[:, 1])
v = np.array(data[:, 2])
energy = np.array(data[:, 3])
force = np.array(data[:, 4])
data_analytic = np.loadtxt(path_analytic)
time_analytic = np.array(data_analytic[:, 0])
x_analytic = np.array(data_analytic[:, 1])
v_analytic = np.array(data_analytic[:, 2])
energy_analytic = np.array(data_analytic[:, 3])
periods: float = get_periods_amount(v)
print("Periods:", periods)
time_total: float = time[-1]
if periods > 0:
    period: float = time_total / periods
    period_round: float = round(period, 2)
    print("Period:", period_round, "s")
    frequency: float = 2 * np.pi / period
    frequency_round: float = round(frequency, 2)
    print("Frequency:", frequency_round, "Hz")
    decrement: float = get_decrement(x)
    if decrement > 0:
        decrement_round: float = round(decrement, 2)
        print("Decrement:", decrement_round)
        quality: float = np.pi / decrement
        quality_round: float = round(quality, 2)
        print("Quality:", quality_round)
ax1.plot(time, x)
ax1.plot(time_analytic, x_analytic)
ax1.plot(time, force)
ax1.legend(["Pendulum", "Analytic", "Force, N / kg"])
ax2.plot(x, v)
ax2.plot(x_analytic, v_analytic)
ax2.legend(["Pendulum", "Analytic"])
ax3.plot(time, v)
ax3.plot(time_analytic, v_analytic)
ax3.plot(time, force)
ax3.legend(["Pendulum", "Analytic", "Force, N / kg"])
ax4.plot(time, energy)
ax4.plot(time_analytic, energy_analytic)
ax4.legend(["Pendulum", "Analytic"])
ax1.set_title('x(t)')
ax1.set_xlabel('t, s')
ax1.set_ylabel('x, m')
ax2.set_title('v(x)')
ax2.set_xlabel('x, m')
ax2.set_ylabel('v, m / s')
ax3.set_title('v(t)')
ax3.set_xlabel('t, s')
ax3.set_ylabel('v, m / s')
ax4.set_title('E(t)')
ax4.set_xlabel('t, s')
ax4.set_ylabel('E, J * s^2 / m^2')
plt.show()
