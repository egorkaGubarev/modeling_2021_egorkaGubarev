import matplotlib.pyplot as plt
import numpy as np
import pygame


is_running: bool = True
visualization_needed: bool = True

height: int = 800
width: int = 1500
y: int = height // 2

path: str = '../../../CLionProjects/Modeling/wk_06/Gas/cmake-build-debug/result.txt'
config_path: str = '../../../CLionProjects/Modeling/wk_06/Gas/cmake-build-debug/config.txt'

black: tuple = (0, 0, 0)
green: tuple = (0, 255, 0)
yellow: tuple = (255, 255, 0)

size: list = [width, height]


config = np.loadtxt(config_path)
data = np.loadtxt(path)


particles_str: str = config[0]
mass_1_str: str = config[1]
mass_2_str: str = config[2]
radius_str: str = config[3]
time_total_str: str = config[4]
length_str: str = config[5]
collisions_str: str = config[6]
time = data[:, 0]


particles: int = int(particles_str)
frames: int = len(time)
collisions: int = int(collisions_str)

radius: float = float(radius_str)
length: float = float(length_str)
scale: float = width / length
r_float: float = radius * scale
right_border_x_float: float = length * scale
time_total: float = float(time_total_str)
delay_s_float: float = time_total / frames
delay_ms_float: float = delay_s_float * 1000

delay_ms: int = int(delay_ms_float)
r: int = int(r_float)
right_border_x: int = int(right_border_x_float)

left_border_up: tuple = (0, 0)
right_border_up: tuple = (right_border_x, 0)
left_border_down: tuple = (0, height)
right_border_down: tuple = (right_border_x, height)
table_dimension: tuple = (particles, frames)


trajectories = np.zeros(table_dimension)
speeds = np.zeros(table_dimension)
masses = np.zeros(particles)
for particle_number in range(particles):
    index_mass: int = particle_number * 3 + 1
    index_position: int = particle_number * 3 + 2
    index_speed: int = (particle_number + 1) * 3
    mass_str: str = data[0, index_mass]
    trajectory = data[:, index_position]
    speed = data[:, index_speed]
    mass: float = float(mass_str)
    trajectories[particle_number, :] = trajectory
    speeds[particle_number, :] = speed
    masses[particle_number] = mass


colours: dict = {mass_1_str: green,
                 mass_2_str: yellow}


print("Collisions:", collisions)
print("Time:", time_total, "s")


figure, (position_plot, speed_plot) = plt.subplots(1, 2)
for particle_number in range(particles):
    trajectory = trajectories[particle_number, :]
    speed = speeds[particle_number, :]
    position_plot.plot(time, trajectory)
    speed_plot.plot(time, speed, '.')
position_plot.set_title("Trajectories")
speed_plot.set_title("Speed")
position_plot.set_xlabel("Time, s")
speed_plot.set_xlabel("Time, s")
position_plot.set_ylabel("Position, m")
speed_plot.set_ylabel("Speed, m / s")
plt.show()


if visualization_needed:
    screen = pygame.display.set_mode(size, pygame.FULLSCREEN)

    for frame_number in range(frames):
        if is_running:
            pygame.draw.line(screen, green, left_border_up, left_border_down)
            pygame.draw.line(screen, green, right_border_up, right_border_down)
            positions = trajectories[:, frame_number]
            for particle_number in range(particles):
                position: float = positions[particle_number]
                x_float: float = position * scale
                x: int = int(x_float)
                center: tuple = (x, y)
                mass: str = masses[particle_number]
                colour: tuple = colours[mass]
                pygame.draw.circle(screen, colour, center, r)
            pygame.display.update()
            screen.fill(black)
            pygame.time.delay(delay_ms)
            for event in pygame.event.get():
                if event.type == pygame.QUIT:
                    is_running = False
                    pygame.quit()
                if event.type == pygame.KEYDOWN:
                    if event.key == pygame.K_ESCAPE:
                        is_running = False
                        pygame.quit()
    pygame.quit()
