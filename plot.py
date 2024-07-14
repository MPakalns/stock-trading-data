import matplotlib.pyplot as plt
import numpy as np

# Function to read data from file
def read_data(filename):
    data = np.genfromtxt(filename, delimiter=',')
    x = data[:, 0]
    y = data[:, 1]
    return x, y

def main():
    filename = 'plot_data.txt'
    x, y = read_data(filename)
    
    plt.plot(x, y, marker='o', linestyle='-', color='b', label='Data')
    plt.xlabel('Dates')
    plt.ylabel('Last 7 day average Opening price')
    plt.title('AAPL stock info')
    plt.legend()
    plt.grid(True)
    plt.show()

if __name__ == '__main__':
    main()