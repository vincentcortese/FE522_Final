//
// Created by Vincent on 4/29/2020.
//

#ifndef FINAL_BISECTION_H
#define FINAL_BISECTION_H

#include <cmath>
template<typename T>
double bisection(double y_target, double a, double b, double epsilon, T g) {
    double m = (a + b) / 2;
    double y = g(m);
    do {
        if (y < y_target) {
            a = m;
        }

        if (y > y_target) {
            b = m;
        }

        m = (a + b) / 2;
        y = g(m);
    } while (fabs(y-y_target) > epsilon);

    return m;
}

#endif //FINAL_BISECTION_H
