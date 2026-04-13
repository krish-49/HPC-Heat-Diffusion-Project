import pandas as pd
import matplotlib.pyplot as plt
import numpy as np

data = pd.read_csv("mpi_heatmap.csv", header=None)

arr = data.values.astype(float)

norm = (arr - arr.min()) / (arr.max() - arr.min() + 1e-12)

plt.imshow(norm, cmap="hot")
plt.colorbar()
plt.title("MPI Heat Diffusion")
plt.show()