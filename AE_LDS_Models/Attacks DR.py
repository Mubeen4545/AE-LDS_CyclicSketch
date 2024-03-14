import numpy as np
import matplotlib.pyplot as plt

# Define the font properties
font = {'family': 'DejaVu Sans', 'size': 8}
plt.rc('font', **font)

# Data for the line plot with three classifiers
categories = ['Benign', 'Slowloris', 'RUDY', 'Slow READ', 'Slow COMM', 'Slow NEXT', 'Apache RANGE', 'Slow DROP', 'Slow REQ']
dt_values = [0, 0.93, 1, 0.95, 1, 0.99, 1, 0.90, 0.99]  # DT_ATC
rf_values = [0, 0.92, 1, 0.96, 1, 0.99, 1, 0.90, 0.99]  # RF_ATC
svm_values = [0, 0.91, 1, 0.97, 0.99, 0.99, 1, 0.77, 0.98]  # SVM_ATC

# Create figure and axis objects
fig, ax = plt.subplots()

# Plotting the line charts for each classifier with specified markers
ax.plot(categories, dt_values, marker='o', linestyle='-', color='blue', label='DT_ATC')
ax.plot(categories, rf_values, marker='^', linestyle='-', color='orange', label='RF_ATC')
ax.plot(categories, svm_values, marker='s', linestyle='-', color='green', label='SVM_ATC')

# Add gridlines
ax.grid(True)

# Set labels for axes
ax.set_xlabel('Attack Type')
ax.set_ylabel('Accuracy (%)')

# Add legend in the lower right corner
ax.legend(loc='lower right')

# Rotate x-axis labels for better readability
plt.xticks(rotation=45)

# Set the y-axis limits
plt.ylim(0, 1.2)

# Set the y-axis ticks
ytick_values = np.arange(0, 1.05, 0.20)  # Generate values from 0 to 1 inclusive, with a step of 0.05
ytick_labels = ['{:.0%}'.format(x) for x in ytick_values]  # Format each tick value as a percentage
plt.yticks(ytick_values, ytick_labels)

# Save the plot as a PDF with a tight layout
plt.tight_layout()
plt.savefig('Classifiers_Comparison_Accuracy_Markers_Lower_Right.pdf', format='pdf')

# Close the figure to prevent it from displaying
plt.close()
