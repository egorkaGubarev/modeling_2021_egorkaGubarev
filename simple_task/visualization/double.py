import numpy as np
import matplotlib.pyplot as plt
import pygame


path: str = "C:/Programs/Git/repositories/modeling_2021_egorkaGubarev/simple_task/calculations/solve/result/result.txt"
path_parameters: str = "C:/Programs/Git/repositories/modeling_2021_egorkaGubarev/simple_task/calculations/solve/" \
                       "parameters/parameters.txt"
figure, ((ax1, ax2), (ax3, ax4)) = plt.subplots(2, 2)
figure.tight_layout()
data = np.loadtxt(path)
time = np.array(data[:, 0])
x1 = np.array(data[:, 1])
x2 = np.array(data[:, 2])
v1 = np.array(data[:, 3])
v2 = np.array(data[:, 4])
energy = np.array(data[:, 5])
ax1.plot(time, x1)
ax1.plot(time, x2)
ax1.legend(["phi1", "phi2"])
ax1.set_title('phi(t)')
ax1.set_xlabel('t, s')
ax1.set_ylabel('phi, rad')
ax2.plot(x1, v1)
ax2.plot(x2, v2)
ax2.legend(["1", "2"])
ax2.set_title('w(phi)')
ax2.set_xlabel('phi, rad')
ax2.set_ylabel('w, rad / s')
ax3.plot(time, v1)
ax3.plot(time, v2)
ax3.legend(["w1", "w2"])
ax3.set_title('w(t)')
ax3.set_xlabel('t, s')
ax3.set_ylabel('w, rad / s')
ax4.plot(time, energy)
ax4.set_title('E(t)')
ax4.set_xlabel('t, s')
ax4.set_ylabel('E, J * s^2 / rad^2')
plt.show()
pygame.init()
width: int = 1500
height: int = 800
x_center: int = width // 2
y_center: int = height // 2
center: list = [x_center, y_center]
size: list = [width, height]
screen = pygame.display.set_mode(size, pygame.FULLSCREEN)
points: int = len(x1)
parameters = np.loadtxt(path_parameters)
m1: float = parameters[0]
m2: float = parameters[1]
l1: float = parameters[2]
l2: float = parameters[3]
delta: float = parameters[4]
delta_ms: float = delta * 1000
delta_int: int = int(delta_ms)
length: float = l1 + l2
scale: float = y_center / length
scale_int: int = int(scale)
scale_mass: int = scale_int // 10
radius_1: float = m1 * scale_mass
radius_2: float = m2 * scale_mass
radius_1_int: int = int(radius_1)
radius_2_int: int = int(radius_2)
green: tuple = (0, 255, 0)
black: tuple = (0, 0, 0)
for point_number in range(points):
    phi1: float = x1[point_number]
    phi2: float = x2[point_number]
    x_ball_1 = x_center + l1 * scale_int * np.sin(phi1)
    y_ball_1 = y_center + l1 * scale_int * np.cos(phi1)
    x_ball_2 = x_ball_1 + l2 * scale_int * np.sin(phi2)
    y_ball_2 = y_ball_1 + l2 * scale_int * np.cos(phi2)
    ball_1: list = [x_ball_1, y_ball_1]
    ball_2: list = [x_ball_2, y_ball_2]
    pygame.draw.line(screen, green, center, ball_1)
    pygame.draw.line(screen, green, ball_1, ball_2)
    pygame.draw.circle(screen, green, ball_1, radius_1_int)
    pygame.draw.circle(screen, green, ball_2, radius_2_int)
    pygame.display.update()
    screen.fill(black)
    pygame.time.delay(delta_int)
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            pygame.quit()
        if event.type == pygame.KEYDOWN:
            if event.key == pygame.K_ESCAPE:
                pygame.quit()
