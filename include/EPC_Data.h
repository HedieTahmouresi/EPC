#ifndef EPC_DATA_H
#define EPC_DATA_H

#include <vector>
#include <iostream>
#include <limits> 

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

    
    ProblemContext(int dim, int pop, double lb, double ub, int maxIter)
        : dimensions(dim), populationSize(pop), 
          lowerBound(lb), upperBound(ub), maxIterations(maxIter) {}
};

#endif 