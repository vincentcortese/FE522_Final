#ifndef FINAL_OPTIONSTRATEGY_H
#define FINAL_OPTIONSTRATEGY_H
#include "optionPricing.h"
#include <numeric>

class OptionStrategy{

protected:
    string x;
    vector<string> dates;
    vector<double> close_prices;
    vector<double> t;
    double v;
    double len;

public:
    OptionStrategy(string x_, vector<string> dates_, vector<double> close_prices_): x{x_}, dates{dates_}, close_prices{close_prices_} {

        v = calcvol(close_prices) + .1;

        len = dates.size()-1;
        for(double i = 0; i < len + 1; ++i){
            t.push_back((dates.size() - i) / dates.size());
        }

    }

    void longCall(){
        vector<double> option, profitability;

        // Long call
        cout << "\nLONG CALL:\n";
        cout << "A long call position gives you the right, but not the obligation to buy the stock at a certain "
             << endl
             << "price by a certain date. In this case, we will buy 1 call option of " + x +
                ", with a strike price specified by the user.\n";
        double k;
        cout << "\nPlease select a strike price for the option. The current price of the stock is: " << close_prices[0] << endl;
        cin >> k;

        cout << "\nStock and option data:\n";
        for (int i = 0; i < dates.size(); ++i) {
            EuropeanOption E(CALL, close_prices[i], k, .05, v, t[i]);
            option.push_back(E.getPrice());
        }

        for (int i = 0; i < 3; ++i) {
            cout << "Date: " << dates[i] << "  " << x + " close price: " << setprecision(2) << fixed
                 << close_prices[i]
                 << "  Option price:  $" << setprecision(2) << fixed << option[i] << endl;
        }
        cout << ".\n.\n.\n";
        for (int i = 4; i > 1; --i) {
            cout << "Date: " << dates[dates.size() - i] << "  " << x + " close price: " << setprecision(2) << fixed
                 << close_prices[dates.size() - i]
                 << "  Option price:  $" << setprecision(2) << fixed << option[dates.size() - i] << endl;
        }
        cout << "\nThis option cost: $" << option[0] << " on the day of purchase, with a strike of: "
             << k << endl;

        cout << "\nThe max the " + x + " call was worth was: $" << *max_element(option.begin(), option.end()) <<
             "." << endl << "This was on date: "
             << dates[max_element(option.begin(), option.end()) - option.begin()] << endl;

        cout << "The max profit was: "
             << (option[max_element(option.begin(), option.end()) - option.begin()] - option[0]) / option[0] * 100
             << "%\n\n";

        if (option[len] < 0.01) {
            cout << "This option expired worthless. We lose the initial premium of: $" << option[0] << endl;
        } else {
            cout << "At expiration, the option could be exercised and 100 shares could be purchased at strike: "
                 << round(close_prices[0]) <<
                 ". The stock price at expiration was: " << close_prices[len] << ".\n";
            cout << "Could have made: $" << close_prices[len] - k - option[0] <<
                 " per share, if you exercised and immediately sold." << endl;
        }
    }

    void coveredCall(){
        vector<double> option, profitability;
        // Covered call
        cout << "\nCOVERED CALL:\n";
        double strike = round(close_prices[0] * 1.1);
        cout
                << "A covered call is when an investor owns 100 shares of the stock, and then also sells a call option. \n"
                << "This is for an investor that wants to hedge against the downside of their investment, although this will cap \n"
                << "gains at the strike price that they sold the option at. We will assume that we buy the 100 shares and sell the \n"
                << "call at the beginning of the time period (April 30, 2019). Buy-write strategies typically write the call out of the money.\n";

        cout << "\nStock and option data:\n";
        for (int i = 0; i < dates.size(); ++i) {
            EuropeanOption E(CALL, close_prices[i], strike, .05, v, t[i]);
            option.push_back(E.getPrice());
        }
        for (int i = 0; i < 3; ++i) {
            cout << "Date: " << dates[i] << "  " << x + " close price: " << setprecision(2) << fixed
                 << close_prices[i]
                 << "  Option price:  $" << setprecision(2) << fixed << option[i] << endl;
        }
        cout << ".\n.\n.\n";
        for (int i = 4; i > 1; --i) {
            cout << "Date: " << dates[dates.size() - i] << "  " << x + " close price: " << setprecision(2) << fixed
                 << close_prices[dates.size() - i]
                 << "  Option price:  $" << setprecision(2) << fixed << option[dates.size() - i] << endl;
        }
        cout << "\nWe gain $" << option[0] * 100 << " from the sale of the option, on date: " << dates[0] << endl;
        cout << "However, buying 100 shares cost: $" << close_prices[0] * 100 << endl << endl;

        if (option[len] < 0.01) {
            cout << "This option expired worthless and we keep the premium. Which was worth: $" << option[0] * 100
                 << ".\n"
                 << "The stock price is now: $" << close_prices[len] << ". Our total P&L is: $" <<
                 close_prices[len] * 100 - close_prices[0] * 100 + option[0] * 100 << endl;
        } else {
            cout << "The option was exercised and we have to sell our 100 shares at strike: " << strike <<
                 ". \nThe stock price at expiration was: " << close_prices[len] << ".\n\n";
            cout << "We are capped to our gains from the stock and have to sell at the strike price.\n" <<
                 "Our total P&L is: $" << strike * 100 - close_prices[0] * 100
                 << ", we would have been better off holding the stock.";
        }
    }

    void bullCallSpread(){
        vector<double> option, option1, profitability;
        // Bull Call Spread
        cout << "\nBULL CALL SPREAD:\n";
        cout
                << "This is a scenario where the investor believes that a stock will have limited upside by expiration. \n"
                <<
                "The investor will buy an out-of-money call, and then sell another call that is more OTM than the one he bought. \n"
                <<
                "This increases downside protection, but will decrease the potential gains. Ideally, you would want the stock to finish \n"
                <<
                "between your long and short options to maximize your gains. This is an example of a debit spread.\n";
        cout << "\nStock and option data:\n";

        for (int i = 0; i < dates.size(); ++i) {
            EuropeanOption E(CALL, close_prices[i], round(close_prices[0] * 1.05), .05, v, t[i]);
            option.push_back(E.getPrice());
            EuropeanOption F(CALL, close_prices[i], round(close_prices[0] * 1.1), .05, v, t[i]);
            option1.push_back(F.getPrice());
        }

        for (int i = 0; i < 3; ++i) {
            cout << "Date: " << dates[i] << "  " << x + " close price: " << setprecision(2) << fixed
                 << close_prices[i]
                 << "  Long Call price: " << setprecision(2) << fixed << option[i]
                 << "  Short Call price: " << setprecision(2) << fixed << option1[i] << endl;
        }
        cout << ".\n.\n.\n";
        for (int i = 4; i > 1; --i) {
            cout << "Date: " << dates[dates.size() - i] << "  " << x + " close price: " << setprecision(2) << fixed
                 << close_prices[dates.size() - i]
                 << "  Long Call price: " << setprecision(2) << fixed << option[dates.size() - i]
                 << "  Short Call price: " << setprecision(2) << fixed << option1[dates.size() - i] << endl;
        }

        cout << "\nThe long call is worth: $" << option[0] << " on the day of purchase, with a strike price of: " <<
             round(close_prices[0] * 1.05) << endl << "The short call is worth: $" << option1[0]
             << " on the day of sale, with a strike price of: " <<
             round(close_prices[0] * 1.1) << endl
             << "\nThe net cost of the contract is: $"<< (-option[0] + option1[0])*-1 << endl << endl;

        if (option[len] < .01) {
            cout
                    << "Both options expire worthless, so you lose your initial investment but you gain the premium on the short call.\n"
                    << "Loss of: $" << (-option[0] + option1[0])*-1 << "\n";
        } else if (option1[len] > 0.01) {
            cout
                    << "This scenario leads to capped gains at expiration. We get to buy the stock at the lower strike, and sell at the higher strike. \n"
                    << "At expiration, the profits are limited to the difference in strike prices, less the cost of the options.\n"
                    << "P&L: $"
                    << round(close_prices[0] * 1.1) - round(close_prices[0] * 1.05) - option[0] + option1[0] <<
                    endl;
        } else if (option[len] > .01 && option1[len] < 0.01) {
            cout
                    << "This is the best case scenario. The option you bought is in-the-money and the option you sold expired "
                    <<
                    "worthless.\nP&L: $"
                    << close_prices[len] - round(close_prices[0] * 1.05) + option1[0] <<
                    endl;
        }
    }

    void callButterflySpread(){
        vector<double> option, option1, option2, profitability;

        // Long Call Butterfly Spread
        cout << "\nLONG CALL BUTTERFLY SPREAD:\n";
        cout
                << "This is an advanced strategy where the investor buys 1 ITM call, sells 2 ATM calls, and then buys 1 OTM call.\n"
                <<
                "All three options have the same expirations but different strike prices. This is for an investor that has a neutral\n"
                <<
                "outlook on the stock, or even slightly bullish. Profits are made when the stock finishes in between the strikes of the long calls.\n"
                << "The closer the price is to the strike of the ATM short calls, the more profitable the option becomes.\n";
        cout << "\nStock and option data:\n";

        for (int i = 0; i < dates.size(); ++i) {
            EuropeanOption E(CALL, close_prices[i], round(close_prices[0] * .9), .05, v, t[i]);
            option.push_back(E.getPrice());
            EuropeanOption F(CALL, close_prices[i], round(close_prices[0]), .05, v, t[i]);
            option1.push_back(F.getPrice());
            EuropeanOption G(CALL, close_prices[i], round(close_prices[0] * 1.1), .05, v, t[i]);
            option2.push_back(G.getPrice());
        }

        for (int i = 0; i < 3; ++i) {
            cout << "Date: " << dates[i] << "  " << x + " close price: " << setprecision(2) << fixed
                 << close_prices[i]
                 << "  Long ITM Call price: " << setprecision(2) << fixed << option[i]
                 << "  Short ATM Call price: " << setprecision(2) << fixed << option1[i]
                 << "  Long OTM Call price: " << setprecision(2) << fixed << option2[i] << endl;
        }
        cout << ".\n.\n.\n";
        for (int i = 4; i > 1; --i) {
            cout << "Date: " << dates[dates.size() - i] << "  " << x + " close price: " << setprecision(2) << fixed
                 << close_prices[dates.size() - i]
                 << "  Long ITM Call price: " << setprecision(2) << fixed << option[dates.size() - i]
                 << "  Short ATM Call price: " << setprecision(2) << fixed << option1[dates.size() - i]
                 << "  Long OTM Call price: " << setprecision(2) << fixed << option2[dates.size() - i] << endl;
        }

        cout << "\nThe long ITM call is worth: $" << option[0]
             << " on the day of purchase, with a strike price of: " <<
             round(close_prices[0] * .9) << endl << "The short ATM call is worth: $" << option1[0]
             << " on the day of sale, with a strike price of: " <<
             round(close_prices[0]) << endl << "The long OTM call is worth: $" << option2[0]
             << " on the day of purchase, with a strike price of: "
             << round(close_prices[0] * 1.1) << endl << endl;

        cout << "The net cost of the contract is: $" << -option[0] + 2 * option1[0] - option2[0] << endl << endl;

        if (close_prices[len] < round(close_prices[0] * .9) ||
            close_prices[len] > round(close_prices[0] * 1.1)) {
            cout << "The stock finished at: " << close_prices[len]
                 << ", outside the range of our long call strike prices, making us lose the initial\n" <<
                 "amount we paid for all the options. Our loss was: $" << (-option[0] + 2 * option1[0] - option2[0]) * -1
                 << endl;
        } else if (close_prices[len] < round(close_prices[0])) {
            cout << "The stock finished at: " << close_prices[len]
                 << ", near the ATM strike we originally bought it at and we have maximized our profits.\n" <<
                 "Our P&L is: $" << -option[0] + 2 * option1[0] - option2[0] + close_prices[len] -
                                    round(close_prices[0] * .9)
                 << endl;
        } else {
            cout << "The stock finished at: " << close_prices[len]
                 << ", near the ATM strike we originally bought it at and we have maximized our profits.\n" <<
                 "Our P&L is: $" << -option[0] + 2 * option1[0] - option2[0] + close_prices[len] -
                                    round(close_prices[0] * .9)
                                    - 2 * (close_prices[len] - round(close_prices[0]))
                 << endl;
        }
    }

    void longPut(){
        vector<double> option, profitability;

        // Long put
        cout << "\nLONG PUT:\n";
        cout << "A long put position gives you the right, but not the obligation to sell the stock at a certain "
             << endl
             << "price by a certain date. In this case, we will buy 1 put option of " + x +
                ", with a strike price specified by the user.\n";
        double k;
        cout << "\nPlease select a strike price for the option. The current price of the stock is: " << close_prices[0] << endl;
        cin >> k;
        cout << "\nStock and option data:\n";
        for (int i = 0; i < dates.size(); ++i) {
            EuropeanOption E(PUT, close_prices[i], k, .05, v, t[i]);
            option.push_back(E.getPrice());
        }
        for (int i = 0; i < 3; ++i) {
            cout << "Date: " << dates[i] << "  " << x + " close price: " << setprecision(2) << fixed
                 << close_prices[i]
                 << "  Option price:  $" << setprecision(2) << fixed << option[i] << endl;
        }
        cout << ".\n.\n.\n";
        for (int i = 4; i > 1; --i) {
            cout << "Date: " << dates[dates.size() - i] << "  " << x + " close price: " << setprecision(2) << fixed
                 << close_prices[dates.size() - i]
                 << "  Option price:  $" << setprecision(2) << fixed << option[dates.size() - i] << endl;
        }
        cout << "\nThis option cost: $" << option[0] << " on the day of purchase, with a strike of: "
             << k << endl;

        cout << "\nThe max the " + x + " put was worth was: $" << *max_element(option.begin(), option.end()) <<
             "." << endl << "This was on date: "
             << dates[max_element(option.begin(), option.end()) - option.begin()] << endl;

        cout << "The max profit was: "
             << (option[max_element(option.begin(), option.end()) - option.begin()] - option[0]) / option[0] * 100
             << "%\n\n";

        if (option[len] < 0.01) {
            cout << "This option expired worthless. We lose the initial premium of: $" << option[0] << endl;
        } else {
            cout << "You could have exercised the option and sold 100 shares at strike: " << k
                 <<
                 ". The stock price at expiration was: " << close_prices[len] << ".\n";
            cout << "Could have made: $" << k - close_prices[len] - option[0] <<
                 " per share, if you exercised and immediately sold." << endl;
        }
    }

    void bearPutSpread(){
        vector<double> option, option1, profitability;

        //Bear Put Spread
        cout << "\nBEAR PUT SPREAD:\n";
        cout
                << "This is a scenario where the investor believes that a stock will have limited downside by expiration. \n"
                <<
                "The investor will buy an in-the-money put, and then sell an OTM put. \n"
                <<
                "This increases downside protection, but will decrease the potential gains. Ideally you would want the stock to finish \n"
                <<
                "between your long and short options to maximize your gains.\n";
        cout << "\nStock and option data:\n";

        for (int i = 0; i < dates.size(); ++i) {
            EuropeanOption E(PUT, close_prices[i], round(close_prices[0] * 1.05), .05, v, t[i]);
            option.push_back(E.getPrice());
            EuropeanOption F(PUT, close_prices[i], round(close_prices[0] * .95), .05, v, t[i]);
            option1.push_back(F.getPrice());
        }
        for (int i = 0; i < 3; ++i) {
            cout << "Date: " << dates[i] << "  " << x + " close price: " << setprecision(2) << fixed
                 << close_prices[i]
                 << "  Long Put price: " << setprecision(2) << fixed << option[i]
                 << "  Short Put price: " << setprecision(2) << fixed << option1[i] << endl;
        }
        cout << ".\n.\n.\n";
        for (int i = 4; i > 1; --i) {
            cout << "Date: " << dates[dates.size() - i] << "  " << x + " close price: " << setprecision(2) << fixed
                 << close_prices[dates.size() - i]
                 << "  Long Put price: " << setprecision(2) << fixed << option[dates.size() - i]
                 << "  Short Put price: " << setprecision(2) << fixed << option1[dates.size() - i] << endl;
        }
        cout << "\nWe buy a put and sell a put on date: " << dates[0] << ".\nThe long put was purchased for: $"
             << option[0] <<
             " with strike: " << round(close_prices[0] * 1.05) << ".\nThe short put was sold for: $" << option1[0]
             << " with strike: "
             << round(close_prices[0] * .95) << endl
             << "\nThis net cost of the contract is: $"<< (-option[0] + option1[0])*-1 << endl << endl;

        if (option[len] < .01) {
            cout
                    << "Both options expire worthless, so you lose your initial investment but you gain the premium on the short call.\n"
                    << "Loss: $" << (-option[0] + option1[0])*-1 << "\n";
        } else if (option1[len] > 0.01) {
            cout
                    << "This scenario leads to capped gains. We get to exercise our put, however, we have to cover the short put as well. \n"
                    << "P&L: $"
                    << round(close_prices[0] * 1.05) - round(close_prices[0] * .95) - option[0] + option1[0] <<
                    endl;
            cout << "At expiration, the profits are limited to the difference in strike prices.\n";
        } else if (option[len] > .01 && option1[len] < 0.01) {
            cout
                    << "This is the best case scenario. The option you bought is in-the-money and the option you sold expired"
                    <<
                    "worthless.\nP&L: $"
                    << round(close_prices[0] * 1.05) - close_prices[len] + option1[0] - option[0]
                    << endl;
        }
    }

    void straddleStrangle(){
        vector<double> option, option1, profitability;

        // Straddle/Strangle
        cout << "[1] Straddle\n"
             << "[2] Strangle\n";
        int n;
        cin >> n;
        while (n != 1 && n != 2) {
            cout << "Invalid!\n\n";
            cout << "[1] Straddle\n"
                 << "[2] Strangle\n";
            cin >> n;
        }
        if (n == 1) {
            //straddle
            cout << "\nSTRADDLE:\n";
            cout << "We purchase a call and put with the same strike price, this strategy is for when an investor "
                 << endl <<
                 "anticipates significant price movement, but is unsure of the direction." << endl;
            cout << "\nStock and option data:\n";
            for (int i = 0; i < dates.size(); ++i) {
                EuropeanOption E(CALL, close_prices[i], round(close_prices[0]), .05, v, t[i]);
                option.push_back(E.getPrice());
                EuropeanOption F(PUT, close_prices[i], round(close_prices[0]), .05, v, t[i]);
                option1.push_back(F.getPrice());
            }
            for (int i = 0; i < 3; ++i) {
                cout << "Date: " << dates[i] << "  " << x + " close price: " << setprecision(2) << fixed
                     << close_prices[i]
                     << "  Call price:  " << setprecision(2) << fixed << option[i]
                     << "  Put price:  " << setprecision(2) << fixed << option1[i] << endl;
            }
            cout << ".\n.\n.\n";
            for (int i = 4; i > 1; --i) {
                cout << "Date: " << dates[dates.size() - i] << "  " << x + " close price: " << setprecision(2)
                     << fixed << close_prices[dates.size() - i]
                     << "  Call price:  " << setprecision(2) << fixed << option[dates.size() - i]
                     << "  Put price:  " << setprecision(2) << fixed << option1[dates.size() - i] << endl;
            }
            cout << "\nIt cost: $" <<  option[0] + option1[0] << " to enter the positions.\n";
            cout << "\nThe max the " + x + " call was worth was: $" << *max_element(option.begin(), option.end()) <<
                 "." << endl << "This was on date: "
                 << dates[max_element(option.begin(), option.end()) - option.begin()] << endl
                 << "The max the " + x + " put was worth was: $" << *max_element(option1.begin(), option1.end())
                 << "." << endl
                 << "This was on date: " << dates[max_element(option1.begin(), option1.end()) - option1.begin()]
                 << endl;

            cout << "\nBy expiration, the call was worth: $" << option[len]
                 << " and the put was worth: $"
                 << option1[len] << ".\n" << "If we held until expiration, we have a total P&L of: $"
                 << option[len] + option1[len] - option[0] - option1[0] << endl;


        } else if (n == 2) {
            //strangle
            cout << "\nSTRANGLE:\n";
            cout
                    << "We purchase a Call and Put with different strike prices. These are typically out of the money, "
                    << endl <<
                    "strangles are usually less expensive, but require greater price movements to generate profits."
                    << endl;
            cout << "\nStock and option data:\n";
            for (int i = 0; i < dates.size(); ++i) {
                EuropeanOption E(CALL, close_prices[i], round(close_prices[0] * 1.1), .05, v, t[i]);
                option.push_back(E.getPrice());
                EuropeanOption F(PUT, close_prices[i], round(close_prices[0] * .9), .05, v, t[i]);
                option1.push_back(F.getPrice());
            }
            for (int i = 0; i < 3; ++i) {
                cout << "Date: " << dates[i] << "  " << x + " close price: " << setprecision(2) << fixed
                     << close_prices[i]
                     << "  Call price:  " << setprecision(2) << fixed << option[i]
                     << "  Put price:  " << setprecision(2) << fixed << option1[i] << endl;
            }
            cout << ".\n.\n.\n";
            for (int i = 4; i > 1; --i) {
                cout << "Date: " << dates[dates.size() - i] << "  " << x + " close price: " << setprecision(2)
                     << fixed << close_prices[dates.size() - i]
                     << "  Call price:  " << setprecision(2) << fixed << option[dates.size() - i]
                     << "  Put price:  " << setprecision(2) << fixed << option1[dates.size() - i] << endl;
            }

            cout << "\nIt cost: $" <<  option[0] + option1[0] << " to enter the positions.\n";

            cout << "\nThe max the " + x + " call was worth was: $" << *max_element(option.begin(), option.end()) <<
                 "." << endl << "This was on date: "
                 << dates[max_element(option.begin(), option.end()) - option.begin()] << endl
                 << "The max the " + x + " put was worth was: $" << *max_element(option1.begin(), option1.end())
                 << "." << endl
                 << "This was on date: " << dates[max_element(option1.begin(), option1.end()) - option1.begin()]
                 << endl;

            cout << "\nBy expiration, the call was worth: $" << option[len]
                 << " and the put was worth: $"
                 << option1[len] << ".\n" << "If we held until expiration, we have a total P&L of: $"
                 << option[len] + option1[len] - option[0] - option1[0] << endl;
        }
    }

    void protectiveCollar(){
        vector<double> option, option1, profitability;

        // Protective Collar
        cout << "\nPROTECTIVE COLLAR:\n";
        cout
                << "The protective collar is created by buying a put option (typically otm) and then also writing a call option (typically otm).\n"
                << "The basic objective of the collar is to hedge downside risk, the call should have a slightly higher strike than the put. This is\n"
                << "essentially a covered call, with a put to further protect the downside, if the stock crashes.\n";
        cout << "\nStock and option data:\n";
        for (int i = 0; i < dates.size(); ++i) {
            EuropeanOption E(CALL, close_prices[i], round(close_prices[0] * 1.05), .05, v, t[i]);
            option.push_back(E.getPrice());
            EuropeanOption F(PUT, close_prices[i], round(close_prices[0] * .95), .05, v, t[i]);
            option1.push_back(F.getPrice());
        }
        for (int i = 0; i < 3; ++i) {
            cout << "Date: " << dates[i] << "  " << x + " close price: " << setprecision(2) << fixed
                 << close_prices[i]
                 << "  Call price: $" << setprecision(2) << fixed << option[i]
                 << "  Put price: $" << setprecision(2) << fixed << option1[i] << endl;
        }
        cout << ".\n.\n.\n";
        for (int i = 4; i > 1; --i) {
            cout << "Date: " << dates[dates.size() - i] << "  " << x + " close price: " << setprecision(2) << fixed
                 << close_prices[dates.size() - i]
                 << "  Call price: $" << setprecision(2) << fixed << option[dates.size() - i]
                 << "  Put price: $" << setprecision(2) << fixed << option1[dates.size() - i] << endl;
        }
        cout << "\nWe gain $" << option[0] * 100 << " from the sale of the call option, on date: " << dates[0]
             << endl;
        cout << "However, buying 100 shares cost: $" << close_prices[0] * 100 << ", and the put cost: $"
             << option1[0]
             << ", with strike: " << round(close_prices[0] * .95) << endl << endl;

        if (option[len] < 0.01) {
            cout
                    << "This option expired worthless and we keep the premium and make money from the put. The premium was worth: $"
                    << option[0] * 100 << ".\n"
                    << "The stock price is now: $" << close_prices[len] << ". Our total P&L is: $" <<
                    close_prices[len] * 100 - close_prices[0] * 100 + option[0] * 100 - option1[0] * 100
                    + (round(close_prices[0] * .95) - close_prices[len]) * 100
                    << ". This demonstrates the capped losses." << endl;
        } else {
            cout << "The option was exercised and we have to sell our 100 shares at strike: "
                 << round(close_prices[0] * 1.05) <<
                 ". \nThe stock price at expiration was: " << close_prices[len] << ".\n\n";
            cout
                    << "We are capped to our gains from the stock and have to sell at the strike price, while also taking a loss from our put.\n"
                    <<
                    "Our total P&L is: $"
                    << round(close_prices[0] * 1.05) * 100 - close_prices[0] * 100 - option1[0] * 100
                    << ", we would have been better off holding the stock.";
        }
    }

    void ironCondor(){
        vector<double> option, option1, option2, option3, profitability;

        // Iron Condor
        cout << "\nIRON CONDOR:\n";
        cout
                << "This is another advanced option strategy, consisting of 2 calls(long and short) and 2 puts(long and short).\n"
                <<
                "The goal is to profit from low volatility. Hoping that the stock finishes in between the strike prices of the short\n"
                <<
                "positions. The max profit is from the credit received from the short positions, these are barely OTM, and the\n"
                <<
                "long positions, which are referred to as wings, are more OTM. The investor receives credit from the short (barely OTM) \n"
                <<
                "options, which is more than he pays for the further OTM options.\n";

        cout << "\nStock and option data:\n";
        for (int i = 0; i < dates.size(); ++i) {
            EuropeanOption E(CALL, close_prices[i], round(close_prices[0] * 1.025), .05, v, t[i]); // sell slightly OTM CALL
            option.push_back(E.getPrice());
            EuropeanOption F(CALL, close_prices[i], round(close_prices[0] * 1.05), .05, v, t[i]); // buy call more OTM
            option1.push_back(F.getPrice());
            EuropeanOption G(PUT, close_prices[i], round(close_prices[0] * .975), .05, v, t[i]); // sell put slightly OTM
            option2.push_back(G.getPrice());
            EuropeanOption H(PUT, close_prices[i], round(close_prices[0] * .95), .05, v, t[i]); // but put more OTM
            option3.push_back(H.getPrice());
        }

        for (int i = 0; i < 3; ++i) {
            cout << "Date: " << dates[i] << "  " << x + " close price: " << setprecision(2) << fixed
                 << close_prices[i]
                 << "  Short Call price: " << setprecision(2) << fixed << option[i]
                 << "  Long Call price: " << setprecision(2) << fixed << option1[i]
                 << "  Short Put price: " << setprecision(2) << fixed << option2[i]
                 << "  Long Put price: " << setprecision(2) << fixed << option3[i] << endl;
        }
        cout << ".\n.\n.\n";
        for (int i = 4; i > 1; --i) {
            cout << "Date: " << dates[dates.size() - i] << "  " << x + " close price: " << setprecision(2) << fixed
                 << close_prices[dates.size() - i]
                 << "  Short Call price: " << setprecision(2) << fixed << option[dates.size() - i]
                 << "  Long Call price: " << setprecision(2) << fixed << option1[dates.size() - i]
                 << "  Short Put price: " << setprecision(2) << fixed << option2[dates.size() - i]
                 << "  Long Put price: " << setprecision(2) << fixed << option3[dates.size() - i] << endl;
        }

        cout << "\nThe short call is worth: $" << option[0] << " on the day of sale, with a strike price of: " <<
             round(close_prices[0] * 1.025) << endl << "The long call is worth: $" << option1[0]
             << " on the day of purchase, with a strike price of: " <<
             round(close_prices[0] * 1.05) << endl << "The short put is worth: $" << option2[0]
             << " on the day of sale, with a strike price of: "
             << round(close_prices[0] * .975) << endl << "The long put is worth: $" << option3[0]
             << " on the day of sale, with a strike price of: "
             << round(close_prices[0] * .95) << endl << endl;

        cout << "The net credit of the contract is: $" << option[0] - option1[0] + option2[0] - option3[0] << endl
             << endl;

        if (close_prices[len] > round(close_prices[0] * 1.05)) {
            cout << "The stock finished at: " << close_prices[len]
                 << ", above the range of our wing strike prices, making us lose the max limit.\n" <<
                 "The loss was: $" << (option[0] - option1[0] + option2[0] - option3[0]
                                       + (close_prices[len] - round(close_prices[0] * 1.05))
                                       - (close_prices[len] - round(close_prices[0] * 1.025)))*-1
                 << "" << endl;

        } else if (close_prices[len] < round(close_prices[0] * .95)) {
            cout << "The stock finished at: " << close_prices[len]
                 << ", below the range of our wing strike prices, making us lose the max limit.\n";

            cout << "The loss was: $" << (option[0] - option1[0] + option2[0] - option3[0]
                                          + (round(close_prices[0] * .95) - close_prices[len])
                                          - ((round(close_prices[0]) * .975) - close_prices[len]))
                 << "\n";
        } else {
            cout << "The stock finished at: " << close_prices[len]
                 << ", which is in between our short call and puts. This would yield\n" <<
                 "the max profit, which is equal to the original credit: $"
                 << option[0] - option1[0] + option2[0] - option3[0] << "\n" <<
                 "All the options expired worthless, so we just keep the initial credit.\n";
        }
    }

private:

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

    double calcvol(const vector<double> prices){
        if(prices.size() == 1) return 0;
        return stddev(pct_change(prices));
    }

};

//TODO
// error check all the strategies
// make sure outputs are cleaned up, descriptive and concise
// Also, touch up the profit and loss from the straddle/strangle


#endif //FINAL_OPTIONSTRATEGY_H
