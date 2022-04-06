import matplotlib.pyplot as plt
import numpy as np
import pygame


is_running: bool = True
histograms_needed: bool = False
graphs_needed: bool = False
visualization_needed: bool = False
energy_needed: bool = True

height: int = 800
width: int = 1500
y: int = height // 2

path: str = '../../../CLionProjects/Modeling/wk_06/Gas/cmake-build-release/result.txt'
config_path: str = '../../../CLionProjects/Modeling/wk_06/Gas/cmake-build-release/config.txt'

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
energy = data[:, 1]
energy_light = data[:, 2]


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


energies = np.zeros(table_dimension)
trajectories = np.zeros(table_dimension)
speeds = np.zeros(table_dimension)
masses = np.zeros(particles)
for particle_number in range(particles):
    index_mass: int = particle_number * 4 + 3
    index_position: int = particle_number * 4 + 4
    index_speed: int = particle_number * 4 + 5
    index_energy: int = particle_number * 4 + 6
    mass_str: str = data[0, index_mass]
    trajectory = data[:, index_position]
    speed = data[:, index_speed]
    energy_of_molecule = data[:, index_energy]
    mass: float = float(mass_str)
    energies[particle_number, :] = energy_of_molecule
    trajectories[particle_number, :] = trajectory
    speeds[particle_number, :] = speed
    masses[particle_number] = mass


colours: dict = {mass_1_str: green,
                 mass_2_str: yellow}


print("Collisions:", collisions)
print("Time:", time_total, "s")

if graphs_needed:
    figure, ([position_plot, light_energy], [energy_plot, energy_of_molecule_plot]) = plt.subplots(2, 2)
    plt.tight_layout()
    energy_plot.plot(time, energy)
    light_energy.plot(time, energy_light)
    energy_plot.set_title("Energy")
    light_energy.set_title("Light energy")
    energy_plot.set_xlabel("Time, s")
    light_energy.set_xlabel("Time, s")
    energy_plot.set_ylabel("Energy, J")
    light_energy.set_ylabel("Energy, J")
    for particle_number in range(particles):
        energy = energies[particle_number, :]
        trajectory = trajectories[particle_number, :]
        speed = speeds[particle_number, :]
        energy_of_molecule_plot.plot(time, energy, '.')
        position_plot.plot(time, trajectory)
    position_plot.set_title("Trajectories")
    energy_of_molecule_plot.set_title("Energies of molecules")
    position_plot.set_xlabel("Time, s")
    energy_of_molecule_plot.set_xlabel("Time, s")
    position_plot.set_ylabel("Position, m")
    energy_of_molecule_plot.set_ylabel("Energy, J")
    plt.show()


if histograms_needed:
    histograms: int = 4
    period: int = frames // histograms
    for histogram_number in range(histograms):
        frame: int = period * histogram_number
        last_speeds = speeds[:, frame]
        light_speeds: list = []
        heavy_speeds: list = []
        for molecule_number in range(particles):
            mass: float = masses[molecule_number]
            speed: float = last_speeds[molecule_number]
            if mass == mass_1_str:
                light_speeds.append(speed)
            if mass == mass_2_str:
                heavy_speeds.append(speed)
        figure, (light_speeds_plot, heavy_speeds_plot) = plt.subplots(1, 2)
        light_speeds_plot.hist(light_speeds, 20)
        heavy_speeds_plot.hist(heavy_speeds, 20)
        light_speeds_plot.set_title("Light speeds")
        heavy_speeds_plot.set_title("Heavy speeds")
    plt.show()

if energy_needed:
    steps_to_average: int = 400
    mass_1_found: bool = False
    mass_2_found: bool = False
    index_1: int = 0
    index_2: int = 0
    for particle_number in range(particles):
        mass_str: str = masses[particle_number]
        if mass_str == mass_1_str and not mass_1_found:
            index_1 = particle_number
            mass_1_found = True
        if mass_str == mass_2_str and not mass_2_found:
            index_2 = particle_number
            mass_2_found = True
        if mass_1_found and mass_2_found:
            break
    energy_1: list = energies[index_1]
    energy_2: list = energies[index_2]
    energies_average_1: list = []
    energies_average_2: list = []
    last: int = frames - steps_to_average
    for step_number in range(last):
        energy_sum_1: float = 0
        energy_sum_2: float = 0
        for sub_step_number in range(steps_to_average):
            index: int = step_number + sub_step_number
            energy_current_1: float = energy_1[index]
            energy_current_2: float = energy_2[index]
            energy_sum_1 += energy_current_1
            energy_sum_2 += energy_current_2
        energy_average_1: float = energy_sum_1 / steps_to_average
        energy_average_2: float = energy_sum_2 / steps_to_average
        energies_average_1.append(energy_average_1)
        energies_average_2.append(energy_average_2)
    points: int = frames - steps_to_average
    time_cut: list = time[:points]
    plt.plot(time_cut, energies_average_1)
    plt.plot(time_cut, energies_average_2)
    plt.xlabel("Time, s")
    plt.ylabel("Energy, J")
    plt.title("Average energy")
    plt.legend(["Light", "Heavy"])
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
