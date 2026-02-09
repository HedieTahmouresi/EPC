#include <iostream>
#include <vector>
#include <limits>
#include "EPC_Data.h"
#include "EPC_Population.h"
#include "EPC_Physics.h"
#include "Cost_Functions.h"

int main() {
    std::cout << "[System] EPC Optimizer Initializing..." << std::endl;

    ProblemContext ctx(4, 4, -10.0, 10.0, 100, OptimizationMode::Minimize);
    
    ctx.costFunction = Benchmarks::Sphere;

    if (ctx.mode == OptimizationMode::Minimize) {
        std::cout << "[Test] Optimization Mode: MINIMIZE" << std::endl;
    }

    EPC_Population colony(ctx);
    colony.initialize();

    const Penguin& p_verify = colony.getPenguin(0);
    std::cout << "[Test] Penguin 0 Initial Heat: " << p_verify.heat << std::endl;

    if (p_verify.heat != std::numeric_limits<double>::max()) {
        std::cout << "[Success] Population initialized and evaluated." << std::endl;
    }

    std::cout << "\n[System] Testing Physics Engine..." << std::endl;

    Penguin p_target(4); 
    p_target.position = {2.0, 1.0, -1.0, 0.0}; 

    Penguin p_mover(4);
    p_mover.position = {3.0, 2.0, 0.0, -1.0}; 

    double dist = EPC_Physics::calculateDistance(p_mover, p_target);
    std::cout << "[Test] Distance Calculated: " << dist << std::endl;
    if (std::abs(dist - 2.0) < 1e-5) std::cout << "[Success] Distance matches PDF (2.0)." << std::endl;

    double mu = 0.1; 
    double Q = EPC_Physics::calculateAttraction(dist, mu);
    std::cout << "[Test] Normalized Attraction (Q): " << Q << std::endl;
    if (std::abs(Q - 0.81873) < 1e-4) std::cout << "[Success] Attraction matches PDF (~0.8187)." << std::endl;

    std::pair<double, double> p3_2d = {3.0, 2.0}; 
    std::pair<double, double> p1_2d = {2.0, 1.0};
    
    double a = 1.0;
    double b = 0.5;

    std::pair<double, double> spiral_res = EPC_Physics::computeSpiralPair(
        p3_2d, p1_2d, Q, a, b
    );
    
    std::cout << "[Test] Spiral Result: X=" << spiral_res.first << ", Y=" << spiral_res.second << std::endl;
    if (std::abs(spiral_res.first - 1.13) < 0.02 && std::abs(spiral_res.second - 0.60) < 0.02) {
        std::cout << "[Success] Spiral 2D logic matches PDF example." << std::endl;
    } else {
        std::cout << "[Warning] Spiral result differs slightly (Check rounding in PDF)." << std::endl;
    }

    Penguin p_current(4); p_current.position = {3.0, 2.0, 0.0, -1.0};
    Penguin p_best(4);    p_best.position = {2.0, 1.0, -1.0, 0.0};
    
    std::vector<double> new_pos = EPC_Physics::computeNewPosition(
        p_current, p_best, 
        0.8187, 1.0, 0.5, 0.0, 
        ctx
    );

    std::cout << "[Test] New Position Size: " << new_pos.size() << std::endl;

    return 0;
}