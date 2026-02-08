#include <iostream>
#include <vector>
#include "EPC_Data.h"
#include "Cost_Functions.h" 

int main() {
    std::cout << "[System] EPC Optimizer Initializing..." << std::endl;

    ProblemContext ctx(4, 4, -10.0, 10.0, 100);
    
    ctx.costFunction = Benchmarks::Sphere;

    Penguin p(ctx.dimensions);
    p.position = {2.0, 1.0, -1.0, 0.0}; 
    
    p.heat = ctx.costFunction(p.position);

    std::cout << "[Test] Context-Bound Heat Calculation: " << p.heat << std::endl;

    if (p.heat == 6.0) {
        std::cout << "[Success] Strategy pattern implemented correctly." << std::endl;
    }

    return 0;
}