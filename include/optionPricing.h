#ifndef FINAL_OPTIONPRICING_H
#define FINAL_OPTIONPRICING_H
#include <iostream>
#include <cmath>
#include <vector>
#include <iomanip>
using namespace std;
enum OptionType {
    CALL,
    PUT
};

class Option {
protected:
    OptionType o;
    double s, k, r, v, t;
protected:
    Option(OptionType o1, double s1, double k1, double r1, double v1, double t1)
            : o{o1}, s{s1}, k{k1}, r{r1}, v{v1}, t{t1} {}
public:
    virtual double getPrice(){
        return(0.0);
    }
    double getSpot() {return s;}
    double getK() { return k;}
    double getR() { return  r;}
    double getSigma() {return v;}
    double getT() {return t;}

    void setSpot(double s0) {s = s0;}
    void setR(double r0) {r = r0;}
    void setSigma(double v0) {v = v0;}
    void setT(double t0) {t = t0;}


    double getDelta(double s_bump, Option &option){
        double p = option.getPrice();
        double S = option.getSpot();
        option.s += s_bump;
        double l = option.getPrice();
        option.setSpot(S);
        return((l-p) / s_bump);
    }
    double getRho(double r_bump, Option &option){
        double p = option.getPrice();
        double R = option.getR();
        option.r += r_bump;
        double l = option.getPrice();
        option.setR(R);
        return((l-p) / (252 * r_bump));
    }
    double getVega(double v_bump, Option &option){
        double p = option.getPrice();
        double V = option.getSigma();
        option.v += v_bump;
        double l = option.getPrice();
        option.setSigma(V);
        return((l-p) / (252 * v_bump));
    }
    double getTheta(double t_bump, Option &option){
        double p = option.getPrice();
        double T = option.getT();
        option.t -= t_bump;
        double l = option.getPrice();
        option.setT(T);
        return((l-p) / (252* t_bump));
    }
};

void report(Option &option){
    cout << "Delta: " << option.getDelta(.01, option) << endl;
    cout << "Rho: " << option.getRho(0.01, option) << endl;
    cout << "Vega: " << option.getVega(.01, option) << endl;
    cout << "Theta: " << option.getTheta(.01, option) << endl << endl;
}

class EuropeanOption: public Option{
public:
    EuropeanOption(OptionType o1, double s1, double k1, double r1, double v1, double t1)
            : Option {o1 , s1, k1, r1, v1, t1}
    {
    }
    double norm_cdf(const double& x) {
        double k = 1.0/(1.0 + 0.2316419*x);
        double k_sum = k*(0.319381530 + k*(-0.356563782 + k*(1.781477937 + k*(-1.821255978 + 1.330274429*k))));

        if (x >= 0.0) {
            return (1.0 - (1.0/(pow(2*M_PI,0.5)))*exp(-0.5*x*x) * k_sum);
        } else {
            return 1.0 - norm_cdf(-x);
        }
    }
    double getPrice() {
        double d1 = (1/(v * sqrt(t))) * (log(s/k) + (r + (pow(v,2)/2)) * t);
        double d2 = d1 - v * sqrt(t);
        if(o == CALL){
            return s * norm_cdf(d1) - k*exp(-r*t) * norm_cdf(d2);
        } else if (o == PUT) {
            return -s * norm_cdf(-d1) + k*exp(-r*t) * norm_cdf(-d2);
        } else {
            cerr << "Please enter valid option type." << endl;
        }
    }

    double f(double vol, double price_given){
        double price_calc;
        double d1 = (1/(vol * sqrt(t))) * ((log2(s/k))+(r + pow(vol,2)/2)*t);
        double d2 = d1 - (vol * sqrt(t));
        if (o == CALL){
            price_calc = (norm_cdf(d1)*s) - (norm_cdf(d2) * k * exp(-1*r*t));
        }
        else if (o == PUT){
            price_calc = (norm_cdf(-1*d2) * k *  exp(-1*r*t)) - (norm_cdf(-1*d1)*s);
        }
        return price_calc - price_given;
    }

    double bisection(double a, double b, double TOL, int NMAX, double price){
        double c;
        int n = 0;
        while (n<=NMAX){
            c=(a+b)/2.0;
            if ((f(c, price)==0) || (abs((b-a)/2.0) < TOL)){
                return c;
            }
            n+=1;
            if((f(a,price)>0 && f(c,price)>0) || (f(a,price)<0 && f(c,price)<0)){
                a=c;
            }
            else{
                b=c;
            }
        }
    }

    double bisection_IV(double price) {
        double bisection_vol=bisection(0.000001, 0.99999, 0.001, 200, price);
        return bisection_vol;
    }
};

class AmericanOption: public Option{
public:
    AmericanOption(OptionType o1, double s1, double k1, double r1, double v1, double t1)
            : Option {o1 , s1, k1, r1, v1, t1}
    {
    }

    double getPrice() {
        int n = 3;
        double dt,u,d,q;
        dt = t/n;
        u = exp(v*sqrt(dt));
        d = exp(-v*sqrt(dt));
        q = (exp(r*dt) - d) / (u-d);

        vector<vector<double>> stockTree(n+1,vector<double>(n+1));
        for(int i = 0; i <= n; ++i){
            for(int j = 0; j <= i; ++j){
                stockTree[i][j] = s * pow(u, j) * pow(d,i - j);
                //cout << i << " " << j << " " << stockTree[i][j] << endl;
            }
        }
        vector<vector<double>> valueTree(n + 1,vector<double>(n + 1));
        for(int j = 0; j <= n; ++j){
            if(o == CALL) {
                valueTree[n][j] = max(stockTree[n][j] - k, 0.);
            } else if (o == PUT) {
                valueTree[n][j] = max(k - stockTree[n][j], 0.);
            } else {
                cerr << "Please enter valid option type.";
                exit(0);
            }
        }
        for(int i = n - 1 ; i >= 0; --i){
            for(int j = 0; j <= i; ++j){
                valueTree[i][j] = exp(-r * dt) * (q * valueTree[i+1][j+1] + (1-q) * valueTree[i + 1][j]);
            }
        }
        return valueTree[0][0];
    }
};



#endif //FINAL_OPTIONPRICING_H
