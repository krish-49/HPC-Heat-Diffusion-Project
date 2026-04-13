import pandas as pd
import numpy as np
import matplotlib.pyplot as plt

# Load latest CSV
data = pd.read_csv("heatmap_hybrid.csv", header=None)

arr = data.values.astype(float)

# Normalize values for better visibility
norm = (arr - arr.min()) / (arr.max() - arr.min() + 1e-12)

# Plot
plt.figure(figsize=(8,8))
plt.imshow(norm, cmap="inferno", origin="upper")
plt.colorbar(label="Normalized Temperature")
plt.title("Hybrid Heat Diffusion (MPI + OpenMP)")
plt.xlabel("X")
plt.ylabel("Y")
plt.tight_layout()
plt.show()

# Debug values at expected hotspots
print("Value at (20,20):", arr[20,20])
print("Value at (70,70):", arr[70,70])
print("Value at (45,80):", arr[45,80])
print("Value at center (50,50):", arr[50,50])