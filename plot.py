import matplotlib.pyplot as plt
import numpy as np

# Function to read data from file
def read_data(filename):
    data = np.genfromtxt(filename, delimiter=',')

    days = data[:, 0]
    opening_prices = data[:, 1]
    highest_prices = data[:, 2]
    lowest_prices = data[:, 3]
    closing_prices = data[:, 4]

    return days, opening_prices, highest_prices, lowest_prices, closing_prices

def main():
    filename = 'plot_data.txt'
    days, opening_prices, highest_prices, lowest_prices, closing_prices = read_data(filename)

    # Plotting the different price data
    plt.figure(figsize=(10, 6))

    plt.plot(days, opening_prices, marker='o', linestyle='-', color='b', label='Opening Prices')
    plt.plot(days, highest_prices, marker='o', linestyle='-', color='g', label='Highest Prices')
    plt.plot(days, lowest_prices, marker='o', linestyle='-', color='r', label='Lowest Prices')
    plt.plot(days, closing_prices, marker='o', linestyle='-', color='m', label='Closing Prices')

    plt.xlabel('Days')
    plt.ylabel('Prices')
    plt.title('AAPL Stock Prices')
    plt.legend()
    plt.grid(True)

    # Ensure x-axis shows every day
    plt.xticks(np.arange(min(days), max(days) + 1, 1))
    plt.gca().set_xticklabels(days.astype(int), rotation=45, ha='right')

    plt.show()

if __name__ == '__main__':
    main()