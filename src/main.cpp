#include <iostream>
#include "EPC_Data.h"
#include "EPC_Optimizer.h" // The Brain
#include "Cost_Functions.h"

int main() {
    // 1. Setup Context
    // Dim=4, Pop=30, Bounds=[-100,100], Iter=100
    ProblemContext ctx(4, 30, -100.0, 100.0, 100, OptimizationMode::Minimize);
    ctx.costFunction = Benchmarks::Sphere;

    // 2. Initialize Optimizer
    EPC_Optimizer optimizer(ctx);

    // 3. Run
    optimizer.run();

    return 0;
}