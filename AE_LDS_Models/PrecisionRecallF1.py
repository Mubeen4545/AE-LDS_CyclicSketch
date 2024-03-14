import matplotlib.pyplot as plt
import numpy as np

# Fixed X-Axis values
x_values = [7, 8, 9, 10]

syn_data = {
    'Precision': [0.96, 0.96, 0.98, 0.98],  # Replace with your TCP precision data
    'Recall': [0.97, 0.98, 0.98, 0.98],     # Replace with your TCP recall data
    'F1-Score': [0.96, 0.97, 0.98, 0.98]    # Replace with your TCP f1-score data
}

udp_c_data = {
    'Precision': [0.98, 0.97, 0.98, 0.98],  # Replace with your TCP-c precision data
    'Recall': [0.96, 0.97, 0.97, 0.98],     # Replace with your TCP-c recall data
    'F1-Score': [0.96, 0.97, 0.97, 0.98]    # Replace with your TCP-c f1-score data
}

udp_f_data = {
    'Precision': [0.85, 0.95, 0.96, 0.98],  # Replace with your TCP-f precision data
    'Recall': [0.93, 0.96, 0.97, 0.98],     # Replace with your TCP-f recall data
    'F1-Score': [0.88, 0.95, 0.96, 0.98]    # Replace with your TCP-f f1-score data
}

def plot_metric(ax, x_values, syn_data, udp_c_data, udp_f_data, metric):
    ax.plot(x_values, syn_data[metric][:len(x_values)], '-o', label='DT_ATC')
    ax.plot(x_values, udp_c_data[metric][:len(x_values)], '-^', label='RF_ATC')
    ax.plot(x_values, udp_f_data[metric][:len(x_values)], '-s', label='SVM_ATC')
    ax.set_xlabel('H')
    ax.set_ylabel(metric.title(), fontsize=8)
    # ax.set_title(f'({metric[0].upper()}) {metric.title()}', fontsize=8)
    ax.set_ylim([0.7, 1.0])  # Set Y-axis values from 0.70 to 1.0
    ax.set_xlim([7, 10])  # Set Y-axis values from 0.70 to 1.0
    ax.set_xticks(x_values)  # Set x-ticks as integer values
    ax.grid(True)  # Show gridlines
    ax.set_facecolor('white')  # Set background color
    ax.legend(loc='lower right', fontsize=8)  # Move legend to the lower right corner

# Set font family and size
plt.rcParams['font.family'] = 'DejaVu Sans'
plt.rcParams['font.size'] = 8

# Create subplots
fig, axes = plt.subplots(nrows=1, ncols=3, figsize=(15, 5))

# Plot precision, recall, and f1-score
plot_metric(axes[0], x_values, syn_data, udp_c_data, udp_f_data, 'Precision')
plot_metric(axes[1], x_values, syn_data, udp_c_data, udp_f_data, 'Recall')
plot_metric(axes[2], x_values, syn_data, udp_c_data, udp_f_data, 'F1-Score')

# Adjust layout and remove empty space
plt.subplots_adjust(wspace=0.3)

# Save separate PDFs for Precision, Recall, and F1-Score
fig.savefig('all_metrics.pdf', format='pdf')
plt.close(fig)

for metric in ['Precision', 'Recall', 'F1-Score']:
    fig, ax = plt.subplots(figsize=(7, 5))
    plot_metric(ax, x_values, syn_data, udp_c_data, udp_f_data, metric)
    plt.tight_layout()
    fig.savefig(f'{metric}.pdf', format='pdf')
    plt.close(fig)

# Show the plot
plt.show()
