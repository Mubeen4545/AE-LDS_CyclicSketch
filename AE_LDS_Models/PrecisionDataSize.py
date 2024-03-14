import matplotlib.pyplot as plt
import numpy as np

data_volume_values = [2e4, 4e4, 6e4, 8e4, 10e4, 1e5]  # Updated X-axis values

syn_data = {'Precision': [0.89, 0.94, 0.94, 0.95, 0.97, 0.97]}  # Inserted None at the beginning
udp_c_data = {'Precision': [0.94, 0.96, 0.97, 0.97, 0.98, 0.98]}  # Inserted None at the beginning
udp_f_data = {'Precision': [0.95, 0.95, 0.97, 0.96, 0.97, 0.97]}  # Inserted None at the beginning

# Ensure all data arrays have the same length
min_length = min(len(data_volume_values), len(syn_data['Precision']), len(udp_c_data['Precision']), len(udp_f_data['Precision']))

data_volume_values = data_volume_values[:min_length]
syn_data['Precision'] = syn_data['Precision'][:min_length]
udp_c_data['Precision'] = udp_c_data['Precision'][:min_length]
udp_f_data['Precision'] = udp_f_data['Precision'][:min_length]

def plot_precision(ax, data_volume_values, syn_data, udp_c_data, udp_f_data):
    ax.plot(data_volume_values[:min_length], syn_data['Precision'], '-o', label='DT_ITC')
    ax.plot(data_volume_values[:min_length], udp_c_data['Precision'], '-^', label='RF_ITC')
    ax.plot(data_volume_values[:min_length], udp_f_data['Precision'], '-s', label='SVM_ITC')
    ax.set_xscale('linear')
    ax.set_xlabel('Data Volume')
    ax.set_ylabel('Precision', fontsize=8)
    ax.set_ylim([0.70, 1.0])  # Adjusted Y-axis range
    ax.set_xlim([2e4, 1e5])   # Adjusted X-axis range
    ax.set_xticks(data_volume_values[:min_length])
    ax.set_xticklabels([f'{int(val/1e3)}K' for val in data_volume_values[:min_length]])  # Updated tick labels
    ax.grid(True)
    ax.set_facecolor('white')
    ax.legend(loc='lower right', fontsize=8)

plt.rcParams['font.family'] = 'DejaVu Sans'
plt.rcParams['font.size'] = 8

fig, ax = plt.subplots(figsize=(7, 5))

# Check if the data lengths match before plotting
if len(set(map(len, [data_volume_values, syn_data['Precision'], udp_c_data['Precision'], udp_f_data['Precision']]))) == 1:
    plot_precision(ax, data_volume_values, syn_data, udp_c_data, udp_f_data)
    fig.savefig('precision_plot.pdf', format='pdf', bbox_inches='tight')  # Save the plot without extra whitespace
    plt.show()
else:
    print("Error: Data lengths do not match.")
