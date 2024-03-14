import matplotlib.pyplot as plt

def plot_metrics():
    # Defining 'w' values as 2^8 and 2^9 for the x-axis
    w_values = [2**8, 2**9]

    # Create a dictionary to hold the metric values for each classifier
    metrics_data = {
        'Precision': {'RF': [0.9732, 0.9810], 'SVM': [0.9732, 0.9811]},
        'Recall': {'RF': [0.9646, 0.9805], 'SVM': [0.9646, 0.9801]},
        'F1-Score': {'RF': [0.9654, 0.9984], 'SVM': [0.9654, 1.0]}
    }

    # Plotting each metric
    for metric, values in metrics_data.items():
        plt.figure(figsize=(6, 4))  # New figure for each metric
        for classifier in values:
            plt.plot(w_values, values[classifier], marker='o', label=classifier)

        plt.title(f'{metric}')
        plt.xlabel('(w)')
        plt.ylabel(f'{metric} (%)')
        plt.xticks(w_values, [f'2^{i}' for i in range(8, 10)])  # Set the x-ticks to show 2^8 and 2^9
        plt.ylim(0.8, 1.0)  # Adjust y-axis limit for better comparison
        plt.legend()
        plt.tight_layout()
        
        # Save the figure as a PDF
        plt.savefig(f'{metric.replace(" ", "_")}.pdf')

if __name__ == "__main__":
    plot_metrics()
