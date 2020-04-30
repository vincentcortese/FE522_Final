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

/*    // Open the file now
    ifstream myFile("../../Final/input/" + x + ".csv");
    if (!myFile) {
        cerr << "File failed to open." << endl;
    }
    vector<Stock> Data;
    Stock temp;
    while (getline(myFile, temp.date, ','))
    {
        getline(myFile, temp.open);
        getline(myFile, temp.high);
        getline(myFile, temp.low);
        getline(myFile, temp.close);
        getline(myFile, temp.adjClose);
        getline(myFile, temp.volume);
        Data.push_back(temp);
    }

    for(int i = 0; i < Data.size(); ++i){
        cout << Data[i] << endl;
    }*/

    // Testing the iv calc, need to call every day to adjust the option price each day
    // by including new volatility calculation, using a new spot price, and reducing the time

    /*EuropeanOption E(CALL, 100, 100, .05, .3, 30/252);
    cout << E.bisection_IV(65) << endl;*/

    AmericanOption a(CALL, 65, 60, .006, .3, 1);
    cout << a.getPrice() << endl;
    return 0;
}
