#include <iostream>
#include "EPC_Data.h" 

int main() {
    std::cout << "[System] EPC Optimizer Initializing..." << std::endl;

    ProblemContext ctx(4, 4, -10.0, 10.0, 100);

    Penguin p(ctx.dimensions);
    
    std::cout << "[Test] Penguin Created with " << p.position.size() << " dimensions." << std::endl;
    std::cout << "[Test] Problem Context: Bounds [" << ctx.lowerBound << ", " << ctx.upperBound << "]" << std::endl;

    return 0;
}