import numpy as np
import matplotlib.pyplot as plt
from matplotlib.widgets import Button

# Function to read data from file
def read_data(filename):
    data = np.genfromtxt(filename, delimiter=',')

    days = data[:, 0].astype(int)
    opening_prices = data[:, 1]
    highest_prices = data[:, 2]
    lowest_prices = data[:, 3]
    closing_prices = data[:, 4]
    RSIs = data[:, 5]

    correctedRSIs = []
    last_value = 0
    index = 0
    for value in RSIs:
        if (value != 0):
            correctedRSIs.append(value)
            last_value = value
        else:
            correctedRSIs.append(last_value)
        index+=1
            


    return days, opening_prices, highest_prices, lowest_prices, closing_prices, correctedRSIs



# Main function to plot data
def main():
    filename = 'plot_data.txt'
    days, opening_prices, highest_prices, lowest_prices, closing_prices, RSIs = read_data(filename)

    # Create figure and subplots
    fig, (ax1, ax2) = plt.subplots(2, 1, figsize=(12, 10), sharex=True)

    # Plot prices on ax1
    line_opening_prices, = ax1.plot(days, opening_prices, label='Opening Prices', color='blue')
    line_highest_prices, = ax1.plot(days, highest_prices, label='Highest Prices', color='green')
    line_lowest_prices, = ax1.plot(days, lowest_prices, label='Lowest Prices', color='red')
    line_closing_prices, = ax1.plot(days, closing_prices, label='Closing Prices', color='black')

    ax1.set_ylabel('Prices')
    ax1.set_title('AAPL Stock Prices')
    ax1.legend(loc='upper left')

    # Plot RSIs on ax2 with connecting lines
    ax2.plot(days, RSIs, label='RSI', linestyle='-', color='purple')
    ax2.set_xlabel('Days')
    ax2.set_ylabel('RSI')
    ax2.legend(loc='upper right')

    # Adjust layout
    plt.xticks(rotation=45)
    plt.tight_layout()

    # Toggle button callback functions
    def toggle_line(line, visible):
        line.set_visible(visible)
        plt.draw()

    def toggle_opening_prices(event):
        toggle_line(line_opening_prices, not line_opening_prices.get_visible())

    def toggle_highest_prices(event):
        toggle_line(line_highest_prices, not line_highest_prices.get_visible())

    def toggle_lowest_prices(event):
        toggle_line(line_lowest_prices, not line_lowest_prices.get_visible())

    def toggle_closing_prices(event):
        toggle_line(line_closing_prices, not line_closing_prices.get_visible())

    # Create buttons for toggling price lines
    ax_1 = plt.axes([0.1, 0.50, 0.2, 0.030])
    button_opening_prices = Button(ax_1, 'Toggle Opening Prices')
    button_opening_prices.on_clicked(toggle_opening_prices)

    ax_2 = plt.axes([0.1, 0.55, 0.2, 0.030])
    button_highest_prices = Button(ax_2, 'Toggle Highest Prices')
    button_highest_prices.on_clicked(toggle_highest_prices)

    ax_3 = plt.axes([0.1, 0.60, 0.2, 0.030])
    button_lowest_prices = Button(ax_3, 'Toggle Lowest Prices')
    button_lowest_prices.on_clicked(toggle_lowest_prices)

    ax_4 = plt.axes([0.1, 0.65, 0.2, 0.030])
    button_closing_prices = Button(ax_4, 'Toggle Closing Prices')
    button_closing_prices.on_clicked(toggle_closing_prices)

    plt.show()

if __name__ == '__main__':
    main()
