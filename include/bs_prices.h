//
// Created by Vincent on 4/29/2020.
//

#ifndef FINAL_BS_PRICES_H
#define FINAL_BS_PRICES_H
#include <iostream>
#include <cmath>

double norm_pdf(const double x) {
    return (1.0/(pow(2*M_PI,0.5)))*exp(-0.5*x*x);
}

double norm_cdf(const double& x) {
    return erfc(-x/sqrt(2))/2;
}

double call_price(double S, double K, double R, double v, double t) {
    double d1 = (1/(v * sqrt(t))) * (log(S/K) + (R+(pow(v,2)/2)) * t);
    double d2 = d1 - v * sqrt(t);
    return S * norm_cdf(d1) - K * exp(-R * t) * norm_cdf(d2);
}

double call_vega(const double S, const double K, const double R, const double v, const double t) {
    double d1 = (1/(v * sqrt(t))) * (log(S/K) + (R+(pow(v,2)/2)) * t);
    return S * sqrt(t) * norm_pdf(d1);
}

#endif //FINAL_BS_PRICES_H
