import numpy as np
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation

N = 100
steps = 200
alpha = 0.1

grid = np.zeros((N, N))

# multiple heat sources
grid[20:30, 20:30] = 100
grid[70:80, 70:80] = 100
grid[40:60, 75:85] = 100

fig, ax = plt.subplots()
img = ax.imshow(grid, cmap='hot', animated=True)
plt.colorbar(img)

def update(frame):
    global grid
    new = grid.copy()

    for i in range(1, N-1):
        for j in range(1, N-1):
            new[i,j] = grid[i,j] + alpha * (
                grid[i+1,j] + grid[i-1,j] +
                grid[i,j+1] + grid[i,j-1] -
                4*grid[i,j]
            )

    # keep sources hot
    new[20:30,20:30] = 100
    new[70:80,70:80] = 100
    new[40:60,75:85] = 100

    grid = new
    img.set_array(grid)
    ax.set_title(f"Step {frame}")
    return [img]

ani = FuncAnimation(fig, update, frames=steps, interval=50)
plt.show()