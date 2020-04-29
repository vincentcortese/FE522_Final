//
// Created by Vincent on 4/29/2020.
//

#ifndef FINAL_BS_H
#define FINAL_BS_H
class bs{
private:
    double S;  // Underlying asset price
    double K;  // Strike price
    double R;  // Risk-free rate
    double t;  // Time to maturity

public:
    bs(double S, double K, double R, double t);
    double operator()(double v) const;
};

#endif //FINAL_BS_H
