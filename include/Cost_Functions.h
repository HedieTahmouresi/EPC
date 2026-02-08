#ifndef COST_FUNCTIONS_H
#define COST_FUNCTIONS_H

#include <vector>
#include <cmath>
#include <functional>

using CostFunction = std::function<double(const std::vector<double>&)>;

class Benchmarks {
public:
    static double Sphere(const std::vector<double>& position) {
        double sum = 0.0;
        for (double val : position) {
            sum += val * val;
        }
        return sum;
    }

    static double Rosenbrock(const std::vector<double>& position) {
        double sum = 0.0;
        size_t D = position.size();
        
        for (size_t i = 0; i < D - 1; ++i) {
            double xi = position[i];
            double next_xi = position[i+1];
            
            double term1 = 100.0 * std::pow((next_xi - (xi * xi)), 2);
            double term2 = std::pow((xi - 1.0), 2);
            
            sum += term1 + term2;
        }
        return sum;
    }
};

#endif