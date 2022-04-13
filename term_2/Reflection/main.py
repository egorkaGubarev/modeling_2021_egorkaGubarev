import numpy as np
import pygame

height: int = 800
width: int = 1500
size: list = [width, height]
is_running: bool = True

path_data: str = '../../../CLionProjects/Modeling/Reflection/cmake-build-debug/result.txt'
path_config: str = '../../../CLionProjects/Modeling/Reflection/cmake-build-debug/config.txt'

config = np.loadtxt(path_config)
x1 = config[:, 0]
y1 = config[:, 1]
x2 = config[:, 2]
y2 = config[:, 3]
x_list = np.append(x1, x2)
x_min: float = min(x_list)
x_max: float = max(x_list)
x_delta: float = x_max - x_min
y = np.append(y1, y2)
y_min: float = min(y)
y_max: float = max(y)
y_delta: float = y_max - y_min
scale_x: float = width / x_delta
scale_y: float = height / y_delta
scale: float = min(scale_x, scale_y)
data = np.loadtxt(path_data)
frames: list = []
(frames_amount, _) = data.shape
for frame_number in range(frames_amount):
    frame = data[frame_number, :]
    frames.append(frame)
(segments_amount, _) = config.shape
segments: list = []
for segment_number in range(segments_amount):
    segment = config[segment_number, :]
    segments.append(segment)
screen = pygame.display.set_mode(size, pygame.FULLSCREEN)
black: tuple = (0, 0, 0)
green: tuple = (0, 255, 0)
yellow: tuple = (255, 255, 0)
delay_ms: int = 800
for frame in frames:
    if is_running:
        for segment in segments:
            x1_: float = segment[0]
            y1_: float = segment[1]
            x2_: float = segment[2]
            y2_: float = segment[3]
            x1_s: float = x1_ * scale
            x2_s: float = x2_ * scale
            y1_s: float = height - y1_ * scale
            y2_s: float = height - y2_ * scale
            start: list = [x1_s, y1_s]
            end: list = [x2_s, y2_s]
            pygame.draw.line(screen, green, start, end)
        x1_: float = frame[0]
        y1_: float = frame[1]
        x2_: float = frame[2]
        y2_: float = frame[3]
        x1_s: float = x1_ * scale
        x2_s: float = x2_ * scale
        y1_s: float = height - y1_ * scale
        y2_s: float = height - y2_ * scale
        start: list = [x1_s, y1_s]
        end: list = [x2_s, y2_s]
        pygame.draw.line(screen, yellow, start, end)
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
