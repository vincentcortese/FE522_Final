#include <iostream>
#include "include/bs.h"
#include "include/bisection.h"

using namespace std;

int main() {
    cout << "Please select a stock to test Option Strategies with." << endl;
    cout << "Choose from:  NFLX,  AAPL,  GE,  TSLA,  ROKU" << endl;


    string x;
    cin >> x;

    cout << "input/" + x;

    return 0;
}
