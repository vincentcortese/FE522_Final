#ifndef FINAL_OPTIONSTRATEGY_H
#define FINAL_OPTIONSTRATEGY_H
double optionInput(){
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

    return input;
}

void optionStrategy(int input,
                    string x,
                    vector<string> dates,
                    vector<double> close_prices
){
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
        }
        cout << "Date: " << dates[0] << "  " << x + " close price: " << setprecision(2) << fixed << close_prices[0]
             << "  Option price:  " << setprecision(2) << fixed << option[0] << "\n.\n.\n.\n";
        cout << "Date: " << dates[dates.size()-1] << "  " << x + " close price: " << setprecision(2) << fixed << close_prices[dates.size()-1]
             << "  Option price:  " << setprecision(2) << fixed << option[dates.size()-1] << endl;



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

}



#endif //FINAL_OPTIONSTRATEGY_H
