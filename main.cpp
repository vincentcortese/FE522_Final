#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <numeric>
#include "include/optionPricing.h"
#include "include/optionStrategy.h"
using namespace std;


int main() {
    cout << "Please select a stock to test Option Strategies with." << endl;
    cout << "Choose from:  AAPL,  GE,  NFLX,  ROKU,  TSLA" << endl;

    vector<string> stocks;
    stocks.push_back("AAPL");
    stocks.push_back("GE");
    stocks.push_back("NFLX");
    stocks.push_back("ROKU");
    stocks.push_back("TSLA");
    string x;
    cin >> x;

    if(find(stocks.begin(),stocks.end(), x) != stocks.end()){

    } else {
        cerr << "Please pick from the stocks above (case sensitive)." << endl;
    }

    // Open the file now
    ifstream myFile;
    myFile.open("../input/" + x + ".txt");
    if (!myFile) {
        cerr << "File failed to open." << endl;
    }

    vector<string> dates;
    vector<double> close_prices;

    string date, name;
    double close;
    getline(myFile, name);

    while (myFile.good()){
        myFile >> date >> close;
        dates.push_back(date);
        close_prices.push_back(close);
    }

    optionStrategy(x, dates, close_prices);


    return 0;
}

// TODO
// Make sure risk free rate is correct
