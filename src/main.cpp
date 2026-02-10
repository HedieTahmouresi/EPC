#include <iostream>
#include "EPC_Data.h"
#include "EPC_Optimizer.h" // The Brain
#include "Cost_Functions.h"

int main() {
    std::cout << "[System] Testing Determinism..." << std::endl;

    // 1. Setup Context with FIXED SEED (e.g., 12345)
    ProblemContext ctx(10, 20, -5.12, 5.12, 5000, OptimizationMode::Minimize, 12345);
    ctx.costFunction = Benchmarks::Sphere;

    // ---------------------------------------------------------
    // RUN 1
    // ---------------------------------------------------------
    std::cout << "\n--- Run 1 ---" << std::endl;
    EPC_Optimizer optimizer1(ctx); 
    optimizer1.run();
    Penguin best1 = optimizer1.getGlobalBest();

    // ---------------------------------------------------------
    // RUN 2 (RESET)
    // ---------------------------------------------------------
    std::cout << "\n--- Run 2 (Same Seed) ---" << std::endl;
    
    // CRITICAL: Reset the RNG to the initial state
    ctx.resetRNG(); 
    
    EPC_Optimizer optimizer2(ctx);
    optimizer2.run();
    Penguin best2 = optimizer2.getGlobalBest();

    // ---------------------------------------------------------
    // COMPARISON
    // ---------------------------------------------------------
    std::cout << "\n[Test] Comparing Results..." << std::endl;
    std::cout << "Run 1 Best: " << best1.heat << std::endl;
    std::cout << "Run 2 Best: " << best2.heat << std::endl;

    if (std::abs(best1.heat - best2.heat) < 1e-9) {
        std::cout << "[Success] Algorithm is DETERMINISTIC." << std::endl;
    } else {
        std::cout << "[Fail] Output varies. RNG leak detected." << std::endl;
    }

    return 0;
}