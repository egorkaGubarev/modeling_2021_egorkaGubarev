import numpy as np

size: int = 10
diagonal_main = 2 * np.ones(size, int)
diagonal_up = -np.ones(size - 1, int)
matrix_main = np.diag(diagonal_main)
matrix_up = np.diag(diagonal_up, 1)
matrix_down = matrix_up.T
matrix = matrix_main + matrix_up + matrix_down
print(matrix)
