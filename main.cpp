#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cassert>

using namespace std;

// Determine if dateA is >, < or = to dateB
int compareDates(string dateA, string dateB)
{
    // Compare year
    int yearA = stoi(dateA.substr(0, 4));
    int yearB = stoi(dateB.substr(0, 4));
    if (yearA != yearB) return (yearA < yearB) ? -1 : 1;

    // Compare month
    int monthA = stoi(dateA.substr(5, 2));
    int monthB = stoi(dateB.substr(5, 2));
    if (monthA != monthB) return (monthA < monthB) ? -1 : 1;

    // Compare day
    int dayA = stoi(dateA.substr(8, 2));
    int dayB = stoi(dateB.substr(8, 2));
    return (dayA < dayB) ? -1 : (dayA > dayB) ? 1 : 0;
}

// Function to read data from a CSV file based on a time frame
vector<vector<string>> readCSVbyDate(string filename, string dateFrom, string dateTo)
{
    vector<vector<string>> allData;

    ifstream file(filename);
    if (!file.is_open())
    {
        cerr << "ERROR opening file: " << filename << "!" << endl;
        return allData;
    }

    string line;
    int lineIndex = 0;

    while (getline(file, line))
    {
        if (lineIndex != 0)
        {
            vector<string> row;
            stringstream ss(line);
            string field;

            // Partition the line string into fields
            while (getline(ss, field, ','))
            {
                row.push_back(field);
            }

            // Ensure row has at least the required number of columns
            if (row.size() < 5)
            {
                cerr << "ERROR: Incomplete row at line " << lineIndex + 1 << " in file: " << filename << endl;
                continue;
            }

            // Check if the date is within the specified range
            if (compareDates(row[0], dateFrom) >= 0 && compareDates(row[0], dateTo) <= 0)
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
void writeDataToFile(string filename, vector<int> days, vector<double> openingPrices, vector<double> highestPrices, vector<double> lowestPrices, vector<double> closingPrices, vector<double> allRSIs)
{
    ofstream file(filename);

    if (!file.is_open())
    {
        cerr << "ERROR opening file: " << filename << "!" << endl;
        return;
    }

    for (size_t i = 0; i < days.size(); i++)
    {
        file << days[i] << "," << openingPrices[i] << "," << highestPrices[i] << "," << lowestPrices[i] << "," << closingPrices[i] << "," << allRSIs[i] << "\n";
    }

    file.close();
}

// Function to check if value is made up of only digits
bool isDigits(string str)
{
    for (int i = 0; i < str.length(); i++)
    {
        if (str[i] < 48 || str[i] > 57) { return false; }
    }

    return true;
}

// Function to check specific date format
bool isYYYY_DD_MM(string str)
{
    if (isDigits(str.substr(0, 4)) && str.substr(4, 1) == "-" && isDigits(str.substr(5, 2)) && str.substr(7, 1) == "-" && isDigits(str.substr(8, 2)))
    {
        return true;
    }

    return false;
}

// Function to calculate relative strength index - RSI
vector<double> calculateRSI(vector<double> closingPrices)
{
    vector<double> listOfRSIs;
    double gains = 0.0; double losses = 0.0;
    int gainsDays = 0; int lossesDays = 0;
    for (int i = 1; i < closingPrices.size(); i++)
    {
        double value = closingPrices[i] - closingPrices[i - 1];
        if (value > 0) { gains += value; gainsDays++; }
        else if (value < 0) { losses -= value; lossesDays++; }

        // Average gains and losses per 14 day period
        if (i % 14 == 0)
        {
            double gainsAverage = gains / gainsDays;
            double lossesAverage = losses / lossesDays;
            double relativeStrength = gainsAverage / lossesAverage;
            double rsi = 100 - (100 / (1 + relativeStrength));
            listOfRSIs.push_back(rsi);
            gains = 0.0;
            losses = 0.0;
            gainsDays = 0;
            lossesDays = 0;
        }
    }

    return listOfRSIs;
}

int main()
{
    // User input and validation
    string userInput;
    bool correctInput = false;
    string date1, date2;
    cout << "Please enter a date period (2000-01-01 - 2024-01-01): " << endl;
    while (!correctInput)
    {
        cout << "Enter date1: "; cin >> userInput;
        if (isYYYY_DD_MM(userInput) && compareDates(userInput, "2000-01-01") == 1) { date1 = userInput; correctInput = true; }
    }
    correctInput = false;
    while (!correctInput)
    {
        cout << "Enter date2: "; cin >> userInput;
        if (isYYYY_DD_MM(userInput) && compareDates(userInput, "2024-01-01") == -1) { date2 = userInput; correctInput = true; }
    }

    // Apple stock data
    string filenameIn = "AAPL.csv";
    vector<vector<string>> data = readCSVbyDate(filenameIn, date1, date2);

    int dayCounter = 1;

    // Constants for data accessing
    const int DATE = 0;
    const int OPENING_PRICE = 1;
    const int HIGHEST_PRICE = 2;
    const int LOWEST_PRICE = 3;
    const int CLOSING_PRICE = 4;

    // Data sets
    vector<int> days;
    vector<double> openingPrices;
    vector<double> highestPrices;
    vector<double> lowestPrices;
    vector<double> closingPrices;

    // Fill the plot file with data
    for (const auto& row : data)
    {
        days.push_back(dayCounter);
        openingPrices.push_back(stod(row[OPENING_PRICE]));
        highestPrices.push_back(stod(row[HIGHEST_PRICE]));
        lowestPrices.push_back(stod(row[LOWEST_PRICE]));
        closingPrices.push_back(stod(row[CLOSING_PRICE]));

        dayCounter++;
    }

    // Ensure all vectors are the same length
    assert(days.size() == openingPrices.size());
    assert(days.size() == highestPrices.size());
    assert(days.size() == lowestPrices.size());
    assert(days.size() == closingPrices.size());

    vector<double> RSIsper14 = calculateRSI(closingPrices);
    vector<double> RSIsAll(days.size(), 0); // Initialize with zeros

    // Populate RSI values at correct intervals
    for (int i = 0; i < RSIsper14.size(); i++) 
    {
        int index = (i + 1) * 14 - 1;
        if (index < RSIsAll.size()) {
            RSIsAll[index] = RSIsper14[i];
        }
        else 
        {
            cout << "Warning: Index " << index << " out of range for RSIsAll." << endl;
        }
    }

    // Ensure RSIsAll is the same length as the other vectors
    assert(days.size() == RSIsAll.size());

    // Write the data to the output file
    string filenameOut = "plot_data.txt";
    writeDataToFile(filenameOut, days, openingPrices, highestPrices, lowestPrices, closingPrices, RSIsAll);

    return 0;
}
