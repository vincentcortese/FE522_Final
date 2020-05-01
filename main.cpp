#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <fstream>
#include <iomanip>
#include <sstream>
#include "include/optionPricing.h"

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
    myFile.open("../../Final/input/" + x + ".txt");
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

    // File is read so we can now start to do stuff... we have to ask the user to choose from our list of option
    // strategies





    for(int i = 0; i < dates.size(); ++i){
        EuropeanOption E(CALL, close_prices[i], 200, .025, .2, (dates.size() - i)/252);
        cout << "Price of " + x + " at close: " << close_prices[i] << ".  Option price:  " <<  E.getPrice() << endl;
    }

    // Testing the iv calc, need to call every day to adjust the option price each day
    // by including new volatility calculation, using a new spot price, and reducing the time

    /*EuropeanOption E(CALL, 100, 100, .05, .3, 30/252);
    cout << E.bisection_IV(65) << endl;*/

    AmericanOption a(CALL, 65, 60, .006, .3, 1);
    //cout << a.getPrice() << endl;
    return 0;
}
