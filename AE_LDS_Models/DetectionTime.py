import matplotlib.pyplot as plt
import numpy as np

# Data for the histograms with updated attack instances and detection times
attacks = {
    'Slowloris': {'color': 'red', 'instances': 207, 'detection_time': 0.11},
    'Slow POST': {'color': 'orange', 'instances': 1159, 'detection_time': 0.18},
    'Slow READ': {'color': 'purple', 'instances': 205, 'detection_time': 0.14},
    'Slow COMM': {'color': 'blue', 'instances': 230, 'detection_time': 0.15},
    'Slow NEXT': {'color': 'green', 'instances': 499, 'detection_time': 0.13},
    'Apache RANGE': {'color': 'brown', 'instances': 260, 'detection_time': 0.12},
    'Slow DROP': {'color': 'pink', 'instances': 295, 'detection_time': 0.16},
    'Slow REQ': {'color': 'gray', 'instances': 178, 'detection_time': 0.10},
}

# Function to create a histogram
def plot_histogram(ax, attack, color, instances, detection_time):
    ax.bar(detection_time, instances, width=0.04, color=color)  # Increase bar width
    ax.set_xlim(0, 0.5)
    ax.set_ylim(0, 4000)  # Adjust the y-axis limit for better visualization
    ax.set_title(attack, fontdict={'family': 'DejaVu Sans', 'size': 9})
    ax.set_xlabel('Time(s)', fontdict={'family': 'DejaVu Sans', 'size': 9})
    ax.set_ylabel('Density/Instances', fontdict={'family': 'DejaVu Sans', 'size': 9})
    ax.legend([attack], prop={'family': 'DejaVu Sans', 'size': 9})

# Plot all histograms together
fig, axes = plt.subplots(1, len(attacks), figsize=(20, 5))

# If we have only one row of plots, axes is a 1D numpy array
# Ensure that axes is iterable
if not isinstance(axes, np.ndarray):
    axes = np.array([axes])

for ax, (attack, data) in zip(axes.flatten(), attacks.items()):
    plot_histogram(ax, attack, data['color'], data['instances'], data['detection_time'])

plt.tight_layout()
plt.show()

# Optionally save the figure to a file
# fig.savefig('attacks_histograms.png')
