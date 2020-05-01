#ifndef FINAL_OPTIONSTRATEGY_H
#define FINAL_OPTIONSTRATEGY_H
#include "optionPricing.h"
#include <numeric>

vector<double> pct_change(const vector<double> prices){
    vector<double> percents;
    for (int i = 1; i < prices.size(); ++i) {
        percents.push_back((prices[i]/prices[i-1])-1);
    }
    return percents;
}

double mean (const vector<double> nums){
    return accumulate( nums.begin(), nums.end(), 0.0)/ nums.size();
}

double stddev(const vector<double> nums) {
    double total = 0;
    int size = nums.size();
    for (int i = 0; i < size; ++i) {
        double mean_value = mean(nums);
        total = total + (nums.at(i) - mean_value)*(nums.at(i) - mean_value);
    }

    return sqrt(total / size);
}

//https://www.techiedelight.com/get-slice-sub-vector-from-vector-cpp/
template<typename T>
std::vector<T> slice(std::vector<T> &v, int m, int n)
{
    std::vector<T> vec(n - m + 1);
    std::copy(v.begin() + m, v.begin() + n + 1, vec.begin());
    return vec;
}

double calcvol(const vector<double> prices){
    if(prices.size() == 1) return 0;
    return stddev(pct_change(prices));
}

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
    while(input != 1 && input != 2 && input != 3 && input != 4 && input != 5 && input != 6 && input != 7 && input != 8
    && input != 9) {
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
        cout << "\nStock and option data:\n";
        for(double i = 0; i < dates.size(); ++i){
            EuropeanOption E(CALL, close_prices[i], round(close_prices[0]), .05,
                    calcvol(slice(close_prices, 0, i)), (dates.size() - i)/dates.size());
            option.push_back(E.getPrice());
        }
        for(double i = 0; i < 3; ++i){
            cout << "Date: " << dates[i] << "  " << x + " close price: " << setprecision(2) << fixed << close_prices[i]
                 << "  Option price:  $" << setprecision(2) << fixed << option[i] << endl;
        }
        cout << ".\n.\n.\n";
        for(double i = 4; i > 1; --i){
            cout << "Date: " << dates[dates.size()-i] << "  " << x + " close price: " << setprecision(2) << fixed << close_prices[dates.size()-i]
                 << "  Option price:  $" << setprecision(2) << fixed << option[dates.size()-i] << endl;
        }
        cout << "\nThis option cost: $" << option[0] << " on the day of purchase."<< endl;

        cout << "\nThe max the " + x + " call was worth was: $" << *max_element(option.begin(), option.end()) <<
             "."<< endl << "This was on date: " << dates[max_element(option.begin(), option.end()) - option.begin()]<< endl;

        cout << "The max profit was: " << (option[max_element(option.begin(), option.end()) - option.begin()] - option[0])/option[0]*100 << "%\n\n";

        if(option[dates.size() - 1] < 0.01){
            cout << "This option expired worthless." << endl;
        } else {
            cout << "You could have exercised the option and bought 100 shares at strike: " << round(close_prices[0]) <<
            ". The stock price at expiration was: " << close_prices[dates.size() - 1] << ".\n";
        }


    } else if (input == 2) {
        // Covered call
        double strike = round(close_prices[0] * 1.1);
        cout << "A covered call is when an investor owns 100 shares of the stock, and then also sells a call option. \n"
        << "This is for an investor that wants to hedge against the downside of their investment, although this will cap \n"
        << "gains at the strike price that they sold the option at. We will assume that we buy the 100 shares and sell the \n"
        << "call at the beginning of the time period (April 30, 2019). Buy-write strategies typically write the call out of the money.\n";

        cout << "\nStock and option data:\n";
        for(double i = 0; i < dates.size(); ++i){
            EuropeanOption E(CALL, close_prices[i], strike, .05,
                             calcvol(slice(close_prices, 0, i)), (dates.size() - i)/dates.size());
            option.push_back(E.getPrice());
        }
        for(double i = 0; i < 3; ++i){
            cout << "Date: " << dates[i] << "  " << x + " close price: " << setprecision(2) << fixed << close_prices[i]
                 << "  Option price:  $" << setprecision(2) << fixed << option[i] << endl;
        }
        cout << ".\n.\n.\n";
        for(double i = 4; i > 1; --i){
            cout << "Date: " << dates[dates.size()-i] << "  " << x + " close price: " << setprecision(2) << fixed << close_prices[dates.size()-i]
                 << "  Option price:  $" << setprecision(2) << fixed << option[dates.size()-i] << endl;
        }
        cout << "\nThis option was worth: $" << option[0] * 100 << " on the day of sale."<< endl;
        cout << "However, buying 100 shares cost: $" << close_prices[0] * 100 << endl;

        if(option[dates.size() - 1] < 0.01){
            cout << "This option expired worthless and we keep the premium. Which was worth: $" << option[0] * 100 << ".\n"
            << "The stock price is now: $" << close_prices[dates.size() - 1] << ". Our total P&L is: $" <<
            close_prices[dates.size() - 1] * 100 - close_prices[0] * 100 + option[0] * 100 << endl;
        } else {
            cout << "The option was exercised and we have to sell our 100 shares at strike: " << strike <<
                 ". \nThe stock price at expiration was: " << close_prices[dates.size() - 1] << ".\n";
            cout << "So we are capped to our gains from the stock and have to sell at the strike price.\n" <<
            "Our total P&L is: $" << strike  * 100 -  close_prices[0] * 100 << ", so we probably would have been better off holding the stock.";
        }

    } else if (input == 3) {
        // Bull Call Spread
    } else if (input == 4) {
        // Long Call Butterfly Spread
    } else if (input == 5){
        // Long put
        cout << "A long put position gives you the right, but not the obligation to sell the stock at a certain " << endl
             << "price by a certain date. In this case, we will buy 1 put option of " + x + ". For simplicity, we will " << endl
             << "use an at-the-money European option that was purchased a year ago, and expires on May 1st." << endl;
        cout << "\nStock and option data:\n";
        for(double i = 0; i < dates.size(); ++i){
            EuropeanOption E(PUT, close_prices[i], round(close_prices[0]), .05,
                             calcvol(slice(close_prices, 0, i)), (dates.size() - i)/dates.size());
            option.push_back(E.getPrice());
        }
        for(double i = 0; i < 3; ++i){
            cout << "Date: " << dates[i] << "  " << x + " close price: " << setprecision(2) << fixed << close_prices[i]
                 << "  Option price:  $" << setprecision(2) << fixed << option[i] << endl;
        }
        cout << ".\n.\n.\n";
        for(double i = 4; i > 1; --i){
            cout << "Date: " << dates[dates.size()-i] << "  " << x + " close price: " << setprecision(2) << fixed << close_prices[dates.size()-i]
                 << "  Option price:  $" << setprecision(2) << fixed << option[dates.size()-i] << endl;
        }
        cout << "\nThe max the " + x + " put was worth was: $" << *max_element(option.begin(), option.end()) <<
             "."<< endl << "This was on date: " << dates[max_element(option.begin(), option.end()) - option.begin()]<< endl;

        cout << "The max profit was: " << (option[max_element(option.begin(), option.end()) - option.begin()] - option[0])/option[0]*100 << "%\n\n";

        if(option[dates.size() - 1] < 0.01){
            cout << "This option expired worthless." << endl;
        } else {
            cout << "You could have exercised the option and sold 100 shares at strike: " << round(close_prices[0]) <<
                 ". The stock price at expiration was: " << close_prices[dates.size() - 1] << ".\n";
        }
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
            cout << "\nStock and option data:\n";
            for(double i = 0; i < dates.size(); ++i){
                EuropeanOption E(CALL, close_prices[i], round(close_prices[0]), .05,
                                 calcvol(slice(close_prices, 0, i)), (dates.size() - i)/dates.size());
                option.push_back(E.getPrice());
                EuropeanOption F(PUT, close_prices[i], round(close_prices[0]), .05,
                                calcvol(slice(close_prices, 0, i)), (dates.size() - i)/dates.size());
                option1.push_back(F.getPrice());
            }
            for(double i = 0; i < 3; ++i){
                cout << "Date: " << dates[i] << "  " << x + " close price: " << setprecision(2) << fixed << close_prices[i]
                        << "  Call price:  " << setprecision(2) << fixed << option[i]
                        << "  Put price:  " << setprecision(2) << fixed << option1[i]<< endl;
            }
            cout << ".\n.\n.\n";
            for(double i = 4; i > 1; --i){
                cout << "Date: " << dates[dates.size()-i] << "  " << x + " close price: " << setprecision(2) << fixed << close_prices[dates.size()-i]
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
            cout << "\nStock and option data:\n";
            for(double i = 0; i < dates.size(); ++i){
                EuropeanOption E(CALL, close_prices[i], round(close_prices[0]*1.1), .05,
                        calcvol(slice(close_prices, 0, i)), (dates.size() - i)/dates.size());
                option.push_back(E.getPrice());
                EuropeanOption F(PUT, close_prices[i], round(close_prices[0]*.9), .05,
                        calcvol(slice(close_prices, 0, i)), (dates.size() - i)/dates.size());
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
