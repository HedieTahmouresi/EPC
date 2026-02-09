#ifndef EPC_DATA_H
#define EPC_DATA_H

#include <vector>
#include <iostream>
#include <limits> 
#include <functional> 

using CostFunction = std::function<double(const std::vector<double>&)>;

enum class OptimizationMode {
    Minimize,
    Maximize
};

struct Penguin {
    std::vector<double> position; 
    double heat;               

    Penguin(int dimensions) {
        position.resize(dimensions, 0.0);
        heat = std::numeric_limits<double>::max(); 
    }
};

struct ProblemContext {
    int dimensions;
    int populationSize;
    double lowerBound;
    double upperBound;
    int maxIterations;
    
    OptimizationMode mode;
    CostFunction costFunction;

    double spiral_a;       
    double spiral_b;       
    double initial_mu;     
    double initial_m;      
    double cooling_factor; 

    ProblemContext(int dim, int pop, double lb, double ub, int maxIter, 
                   OptimizationMode m = OptimizationMode::Minimize)
        : dimensions(dim), populationSize(pop), 
          lowerBound(lb), upperBound(ub), maxIterations(maxIter), mode(m),
          spiral_a(1.0), spiral_b(0.5), 
          initial_mu(0.05), initial_m(0.5), cooling_factor(0.99) {}
};

#endif 