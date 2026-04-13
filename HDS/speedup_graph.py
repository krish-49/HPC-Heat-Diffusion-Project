import matplotlib.pyplot as plt

versions = ["Serial", "OMP", "MPI", "Hybrid"]
times = [12.5, 4.2, 5.1, 2.8]   # replace with your measured times

speedup = [times[0]/t for t in times]

plt.figure(figsize=(8,5))
bars = plt.bar(versions, speedup)

for b,v in zip(bars,speedup):
    plt.text(b.get_x()+b.get_width()/2, b.get_height()+0.05,
             f"{v:.2f}x", ha='center')

plt.ylabel("Speedup")
plt.title("Parallel Performance Comparison")
plt.grid(axis='y')
plt.show()