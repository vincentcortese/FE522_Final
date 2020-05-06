#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <fstream>
#include "include/optionStrategy.h"
using namespace std;


double optionInput(){
    cout << "\n\nChoose an Option Strategy to test: (pick 1-9)\n";
    cout << "[1] Long Call\n"
         << "[2] Covered Call\n"
         << "[3] Bull Call Spread\n"
         << "[4] Long Call Butterfly Spread\n"
         << "[5] Long Put\n"
         << "[6] Bear Put Spread\n"
         << "[7] Straddle/Strangle\n"
         << "[8] Protective Collar\n"
         << "[9] Iron Condor\n"
         << "[0] Exit Code\n";

    int input;

    cin >> input;

    return input;
}

void optionStrategy(string x,
                    vector<string> dates,
                    vector<double> close_prices
) {
    OptionStrategy strategies(x, dates, close_prices);
    while (true) {
        int input = optionInput();
        if (input == 0){
            break;
        }
        while (input != 1 && input != 2 && input != 3 && input != 4 && input != 5 && input != 6 && input != 7 &&
               input != 8 && input != 9) {
            cout << "Invalid!\n\n";

            input = optionInput();
        }

        if (input == 1) {
            strategies.longCall();
        } else if (input == 2) {
            strategies.coveredCall();
        } else if (input == 3) {
            strategies.bullCallSpread();
        } else if (input == 4) {
            strategies.callButterflySpread();
        } else if (input == 5) {
            strategies.longPut();
        } else if (input == 6) {
            strategies.bearPutSpread();
        } else if (input == 7) {
            strategies.straddleStrangle();
        } else if (input == 8) {
            strategies.protectiveCollar();
        } else if (input == 9) {
            strategies.ironCondor();
        }
    }
}



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
