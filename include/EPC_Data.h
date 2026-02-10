#ifndef EPC_DATA_H
#define EPC_DATA_H

#include <vector>
#include <iostream>
#include <limits> 
#include <functional> 
#include <random>

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

    int numLeaders;      
    int leaderCapacity;

    OptimizationMode mode;
    CostFunction costFunction;

    double spiral_a;       
    double spiral_b;       
    double initial_mu;     
    double initial_m;      
    double cooling_factor; 

    int seed; // random with seed 
    std::mt19937 rng; // random with seed

    ProblemContext(int dim, int pop, double lb, double ub, int maxIter, 
                   OptimizationMode m = OptimizationMode::Minimize, int rndSeed = 42) 
        : dimensions(dim), populationSize(pop), 
          lowerBound(lb), upperBound(ub), maxIterations(maxIter), mode(m),
          numLeaders(std::max(1, pop / 10)), 
          leaderCapacity((pop / std::max(1, pop / 10)) + 2), 
          spiral_a(1.0), spiral_b(0.5), 
          initial_mu(0.05), initial_m(0.5), cooling_factor(0.99),
          seed(rndSeed) { 
              rng.seed(seed);
          }// random with seed
    // this whole method is for random with seed 
    void resetRNG() {
        rng.seed(seed);
    }
};

#endif 