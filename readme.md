# Heat Diffusion Simulation using High Performance Computing (HPC)

## Overview

This project demonstrates a **2D Heat Diffusion Simulation** using multiple High Performance Computing approaches.
The objective is to compare execution performance and visualize heat propagation using parallel computing techniques.

The simulation models how heat spreads across a metal plate/grid over time using the numerical heat equation.

---

## Implemented Versions

### 1. Serial Version

* Traditional single-core execution
* Baseline for performance comparison

### 2. OpenMP Version

* Shared-memory parallelism
* Uses CPU threads for faster computation

### 3. MPI Version

* Distributed-memory parallelism
* Uses multiple processes with message passing

### 4. Hybrid Version

* Combines MPI + OpenMP
* Uses processes + threads together

---

## Features

* 2D Grid Heat Simulation
* Multiple Heat Sources
* CSV Export for Visualization
* Heatmap Plot Generation using Python
* Speedup Graph Comparison
* Performance Benchmarking

---

## Heat Equation Used

The simulation uses finite difference approximation:

T_new = T + α (up + down + left + right - 4T)

Where:

* **T** = Current temperature
* **α** = Diffusion coefficient
* Neighbor cells exchange heat over time

---

## Technologies Used

* C Programming
* OpenMP
* MPI (MS-MPI)
* Python
* Matplotlib
* NumPy
* CSV Data Export

---

## File Structure

```text
serial.c
omp.c
mpi.c
hybrid.c

serial.py
omp.py
mpi.py
hybrid.py

speedup_graph.py

serial_heatmap.csv
omp_heatmap.csv
mpi_heatmap.csv
heatmap_hybrid.csv
```

---

## Compilation & Run Commands

### Serial

```bash
gcc serial.c -O2 -o serial.exe
./serial.exe
```

### OpenMP

```bash
gcc omp.c -fopenmp -O2 -o omp.exe
./omp.exe
```

### MPI

```bash
gcc mpi.c -O2 -o mpi.exe -lmsmpi
mpiexec -n 4 ./mpi.exe
```

### Hybrid

```bash
gcc hybrid.c -fopenmp -O2 -o hybrid.exe -lmsmpi
mpiexec -n 4 ./hybrid.exe
```

---

## Visualization

Run Python plotting files to generate heatmaps:

```bash
python serial.py
python omp.py
python mpi.py
python hybrid.py
```

---

## Performance Metrics

The project compares:

* Execution Time
* Speedup
* Efficiency
* Scalability

---

## Sample Results

| Version | Time (sec) |
| ------- | ---------- |
| Serial  | Highest    |
| OpenMP  | Faster     |
| MPI     | Faster     |
| Hybrid  | Fastest    |

---

## Learning Outcomes

* Parallel Programming Concepts
* Shared vs Distributed Memory
* Synchronization and Communication
* Performance Optimization
* Scientific Computing Simulation

---

## Future Improvements

* GPU Acceleration using CUDA
* Real-Time GUI Dashboard
* 3D Heat Simulation
* Adaptive Mesh Refinement
* Web Visualization

---

## Author

Krish / Team HPC Project

---

## License

For Academic / Educational Use
