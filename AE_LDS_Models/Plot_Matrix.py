import matplotlib.pyplot as plt

def plot_metrics():
    # Defining 'w' values as 2^8 and 2^11 for the x-axis
    w_values = [2**8, 2**11]

    # Create a dictionary to hold the metric values for each classifier
    metrics_data = {
        'Precision': {'RF': [0.9732, 0.9810], 'SVM': [0.9832, 0.9911]},
        'Recall': {'RF': [0.9646, 0.9811], 'SVM': [0.9546, 0.9701]},
        'F1-Score': {'RF': [0.9654, 0.9705], 'SVM': [0.9654, 0.9901]}
    }

    # Plotting each metric
    for metric, values in metrics_data.items():
        plt.figure(figsize=(6, 4))  # New figure for each metric
        for classifier in values:
            plt.plot(w_values, values[classifier], marker='o', label=classifier)

        plt.title(f'{metric}')
        plt.xlabel('(w)')
        plt.ylabel(f'{metric} (%)')
        plt.xticks(w_values, [f'2^{8}', f'2^{11}'])  # Set the x-ticks to show 2^8 and 2^11
        plt.ylim(0.8, 1.0)  # Adjust y-axis limit for better comparison
        plt.legend()
        plt.tight_layout()
        
        # Save the figure as a PDF
        plt.savefig(f'{metric.replace(" ", "_")}.pdf')

if __name__ == "__main__":
    plot_metrics()