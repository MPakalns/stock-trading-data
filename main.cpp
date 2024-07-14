#include <iostream>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

int compareDates(string dateA, string dateB)
{
    if (stoi(dateA.substr(0, 4)) == stoi(dateB.substr(0, 4)))
    {
        if (stoi(dateA.substr(5, 2)) == stoi(dateB.substr(5, 2)))
        {
            if (stoi(dateA.substr(8, 2)) == stoi(dateB.substr(8, 2))) return 0;
            else if (stoi(dateA.substr(8, 2)) < stoi(dateB.substr(8, 2))) return -1;
            else if (stoi(dateA.substr(8, 2)) > stoi(dateB.substr(8, 2))) return 1;
        }

        else if (stoi(dateA.substr(5, 2)) < stoi(dateB.substr(5, 2))) return -1;
        else if (stoi(dateA.substr(5, 2)) > stoi(dateB.substr(5, 2))) return 1;
    }

    else if (stoi(dateA.substr(0, 4)) < stoi(dateB.substr())) return -1;
    else if (stoi(dateA.substr(0, 4)) > stoi(dateB.substr())) return 1;
}

// Function to read data from a CSV file based on a time frame
vector<vector<string>> readCSVbyDate(string filename, string dateFrom, string dateTo) 
{
    // 2D array with strings, each array is data (string) from a date
    vector<vector<string>> allData;

    ifstream file(filename);
    if (!file.is_open()) 
    {
        cerr << "ERROR opening file: " << filename << "!" << endl;
        return allData;
    }

    string line;
    string field;
    int lineIndex = 0;

    while (getline(file, line)) 
    {
        if (lineIndex != 0)
        {
            vector<string> row;
            stringstream ss(line);  // To go through string line

            // Partition the line string in fields
            while (getline(ss, field, ','))
            {
                row.push_back(field);
            }

            if (compareDates(row[0], dateFrom) == 1 && compareDates(row[0], dateTo) == -1)
            {
                allData.push_back(row);
            }
        }
        lineIndex++;
    }

    file.close();
    return allData;
}

// Function to write data to a text file
void writeDataToFile(string filename, vector<int> x, vector<double> y) 
{
    ofstream file(filename);

    if (!file.is_open()) {
        cerr << "ERROR opening file: " << filename << "!" << endl;
        return;
    }

    for (int i = 0; i < x.size(); i++) 
    {
        file << x[i] << "," << y[i] << "\n";
    }

    file.close();
}


int main()
{
    string filenameIn = "AAPL.csv";
    vector<vector<string>> data = readCSVbyDate(filenameIn, "1980-12-12", "1980-12-22");

    int dayCounter = 1;

    const int OPENING_PRICE = 0;
    const int HIGHEST_PRICE = 1;
    const int LOWEST_PRICE = 2;
    const int CLOSING_PRICE = 3;

    double last7dayOpeningPrices[7] = {};

    std::vector<int> x = {};
    std::vector<double> y = {};

    // Fill the plot file with data
    for (const auto& row : data) 
    {   
        cout << row[0] << endl;
        if (dayCounter > 7)
        {
            double last7dayOpeningPricesAverage = 0.00;
            for (int i = 0; i < 7; i++)
            {
                last7dayOpeningPricesAverage += last7dayOpeningPrices[i];
            }
            last7dayOpeningPricesAverage /= 7.0;

            x.push_back(dayCounter);
            y.push_back(last7dayOpeningPricesAverage);

            for (int i = 0; i < 6; i++)
            {
                last7dayOpeningPrices[i] = last7dayOpeningPrices[i + 1];
            }
            last7dayOpeningPrices[6] = std::stod(row[1]);
        }

        else
        {
            last7dayOpeningPrices[dayCounter-1] = std::stod(row[1]);
        }

        dayCounter++;
    }

    std::string filenameOut = "plot_data.txt";

    writeDataToFile(filenameOut, x, y);


}
