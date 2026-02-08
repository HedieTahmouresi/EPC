#include <iostream>
#include "EPC_Data.h"
#include "Cost_Functions.h"
#include "EPC_Population.h" 

int main() {
    std::cout << "[System] EPC Optimizer Initializing..." << std::endl;

    
    ProblemContext ctx(4, 10, -100.0, 100.0, 100, OptimizationMode::Minimize);
    ctx.costFunction = Benchmarks::Sphere;

    
    EPC_Population colony(ctx);
    
    
    colony.initialize();

    
    const Penguin& p1 = colony.getPenguin(0);
    std::cout << "[Test] Penguin 0 Position: [" << p1.position[0] << ", " << p1.position[1] << ", " << p1.position[2] << ", " << p1.position[3] << "]" << std::endl;
    std::cout << "[Test] Penguin 0 Initial Heat: " << p1.heat << std::endl;

    if (p1.heat != std::numeric_limits<double>::max()) {
        std::cout << "[Success] Population initialized and evaluated." << std::endl;
    }

    return 0;
}