#include <iostream>
#include <vector>
#include "EPC_Data.h"
#include "Cost_Functions.h" 

int main() {
    std::cout << "[System] EPC Optimizer Initializing..." << std::endl;

    ProblemContext ctx(4, 4, -10.0, 10.0, 100);
    Penguin p(ctx.dimensions);
    p.position = {2.0, 1.0, -1.0, 0.0}; 

    p.heat = Benchmarks::Sphere(p.position);

    std::cout << "[Test] Penguin Heat (Sphere): " << p.heat << std::endl;
    
    if (p.heat == 6.0) {
        std::cout << "[Success] Heat calculation matches PDF example." << std::endl;
    } else {
        std::cout << "[Error] Heat calculation incorrect." << std::endl;
    }

    return 0;
}