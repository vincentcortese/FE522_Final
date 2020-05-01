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

    // File is read so we can now start to do stuff... we have to ask the user to choose from our list of option
    // strategies

    cout << "Choose an Option Strategy to test: (pick 1-9)\n";
    cout << "[1] Long Call\n"
              << "[2] Covered Call\n"
              << "[3] Bull Call Spread\n"
              << "[4] Long Call Butterfly Spread\n"
              << "[5] Long Put\n"
              << "[6] Bear Put Spreads\n"
              << "[7] Straddle/Strangle\n"
              << "[8] Protective Collar\n"
              << "[9] Iron Condor\n";

    int input;

    cin >> input;

    while(input < 1 && input > 9) {
        cout << "Invalid!\n\n";

        cout << "Choose an Option Strategy to test: (pick 1-9)\n";
        cout << "[1] Long Call\n"
             << "[2] Covered Call\n"
             << "[3] Bull Call Spread\n"
             << "[4] Long Call Butterfly Spread\n"
             << "[5] Long Put\n"
             << "[6] Bear Put Spread\n"
             << "[7] Straddle/Strangle\n"
             << "[8] Protective Collar\n"
             << "[9] Iron Condor\n";

        cin >> input;
    }

    vector <double> option, option1, option2, option3, profitability;

    if (input == 1) {
        // Long call
        cout << "A long call position gives you the right, but not the obligation to buy the stock at a certain " << endl
        << "price by a certain date. In this case, we will buy 1 call option of " + x + ". For simplicity, we will " << endl
        << "use an at-the-money European option that was purchased a year ago, and expires on May 1st." << endl;
        for(double i = 0; i < dates.size(); ++i){
            EuropeanOption E(CALL, close_prices[i], round(close_prices[0]), .05, .2, (dates.size() - i)/dates.size());
            option.push_back(E.getPrice());
            cout << "Date: " << dates[i] << "  " << x + " close price: " << setprecision(2) << fixed << close_prices[i]
            << "  Option price:  " << setprecision(2) << fixed << option[i] << endl;
        }
        cout << "\nThe max the " + x + " call was worth was: $" << *max_element(option.begin(), option.end()) <<
        "."<< endl << "This was on date: " << dates[max_element(option.begin(), option.end()) - option.begin()]<< endl;

    } else if (input == 2) {
        // Covered call
    } else if (input == 3) {
        // Bull Call Spread
    } else if (input == 4) {
        // Long Call Butterfly Spread
    } else if (input == 5){
        // Long put
        cout << "A long put position gives you the right, but not the obligation to sell the stock at a certain " << endl
             << "price by a certain date. In this case, we will buy 1 put option of " + x + ". For simplicity, we will " << endl
             << "use an at-the-money European option that was purchased a year ago, and expires on May 1st." << endl;
        for(double i = 0; i < dates.size(); ++i){
            EuropeanOption E(PUT, close_prices[i], round(close_prices[0]), .05, .2, (dates.size() - i)/dates.size());
            option.push_back(E.getPrice());
            cout << "Date: " << dates[i] << "  " << x + " close price: " << setprecision(2) << fixed << close_prices[i]
                 << "  Option price:  " << setprecision(2) << fixed << option[i] << endl;
        }
        cout << "\nThe max the " + x + " put was worth was: $" << *max_element(option.begin(), option.end()) <<
             "."<< endl << "This was on date: " << dates[max_element(option.begin(), option.end()) - option.begin()]<< endl;
    } else if (input == 6) {
        //Bear Put Spread
    } else if (input == 7){
        // Straddle/Strangle
        cout << "[1] Straddle\n"
            << "[2] Strangle\n";
        int n;
        cin >> n;
        while(n != 1 && n != 2) {
            cout << "Invalid!\n\n";

            cout << "[1] Straddle\n"
                 << "[2] Strangle\n";

            cin >> n;
        }
        if(n == 1){
            //straddle
            cout << "We purchase a Call and Put with the same strike price, this strategy is when an investor " << endl <<
            "anticipates significant price movement, but is unsure of the direction." << endl;
            for(double i = 0; i < dates.size(); ++i){
                EuropeanOption E(CALL, close_prices[i], round(close_prices[0]), .05, .2, (dates.size() - i)/dates.size());
                option.push_back(E.getPrice());
                EuropeanOption F(PUT, close_prices[i], round(close_prices[0]), .05, .2, (dates.size() - i)/dates.size());
                option1.push_back(F.getPrice());
                cout << "Date: " << dates[i] << "  " << x + " close price: " << setprecision(2) << fixed << close_prices[i]
                     << "  Call price:  " << setprecision(2) << fixed << option[i]
                     << "  Put price:  " << setprecision(2) << fixed << option1[i]<< endl;
            }
            cout << "\nThe max the " + x + " call was worth was: $" << *max_element(option.begin(), option.end()) <<
                 "."<< endl << "This was on date: " << dates[max_element(option.begin(), option.end()) - option.begin()]<< endl
                    << "The max the " + x + " put was worth was: $" << *max_element(option1.begin(), option1.end()) << "."<< endl
                    << "This was on date: " << dates[max_element(option1.begin(), option1.end()) - option1.begin()] << endl;

        } else if (n == 2){
            //strangle
            cout << "We purchase a Call and Put with different strike prices. These are typically out of the money, " << endl <<
            "strangles are usually less expensive, but require greater price movements to generate profits." << endl;
            for(double i = 0; i < dates.size(); ++i){
                EuropeanOption E(CALL, close_prices[i], round(close_prices[0]*1.1), .05, .2, (dates.size() - i)/dates.size());
                option.push_back(E.getPrice());
                EuropeanOption F(PUT, close_prices[i], round(close_prices[0]*.9), .05, .2, (dates.size() - i)/dates.size());
                option1.push_back(F.getPrice());
                cout << "Date: " << dates[i] << "  " << x + " close price: " << setprecision(2) << fixed << close_prices[i]
                     << "  Call price:  " << setprecision(2) << fixed << option[i]
                     << "  Put price:  " << setprecision(2) << fixed << option1[i]<< endl;
            }
            cout << "\nThe max the " + x + " call was worth was: $" << *max_element(option.begin(), option.end()) <<
                 "."<< endl << "This was on date: " << dates[max_element(option.begin(), option.end()) - option.begin()] << endl
                    << "The max the " + x + " put was worth was: $" << *max_element(option1.begin(), option1.end()) << "."<< endl
                    << "This was on date: " << dates[max_element(option1.begin(), option1.end()) - option1.begin()] << endl;
        }

    } else if (input == 8){
        // Protective Collar
    } else if (input == 9){
        // Iron Condor

    }





/*
  for(int i = 0; i < dates.size(); ++i){
        EuropeanOption E(PUT, close_prices[i], 200, .05, .5, (dates.size() - i)/dates.size());
        cout << "Price of " + x + " at close: " << close_prices[i] << ".  Option price:  " <<  E.getPrice() << endl;
    }
*/

    // Testing the iv calc, need to call every day to adjust the option price each day
    // by including new volatility calculation, using a new spot price, and reducing the time
    // pls fix, thx

    /*EuropeanOption E(CALL, 100, 100, .05, .3, 30/252);
    cout << E.bisection_IV(65) << endl;*/

    //AmericanOption a(CALL, 65, 60, .006, .3, 1);
    //cout << a.getPrice() << endl;
    return 0;
}

// TODO
// Figure out how to implement the bisection method in the loop so we can have new volatilities as the days go on
// Start making profitability vectors for the different strategies and output those
// Make sure risk free rate is correct
// Potentially just move all the input stuff into a function into a header, focus on next week
