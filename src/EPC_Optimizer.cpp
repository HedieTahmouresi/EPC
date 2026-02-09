#include "EPC_Optimizer.h"
#include "EPC_Physics.h"
#include <iostream>
#include <algorithm> 

EPC_Optimizer::EPC_Optimizer(const ProblemContext& context)
    : ctx(context), colony(context) {
    
    current_mu = 0.05; 
    current_m = 0.5;
}

bool EPC_Optimizer::isBetter(double val1, double val2) const {
    if (ctx.mode == OptimizationMode::Minimize) {
        return val1 < val2;
    } else {
        return val1 > val2;
    }
}

void EPC_Optimizer::run() {
    std::cout << "[Optimizer] Starting Optimization..." << std::endl;
    
    colony.initialize();

    for (int t = 0; t < ctx.maxIterations; ++t) {
        
        Penguin bestPenguin = getGlobalBest();

        std::cout << "Iter " << t << " | Best Heat: " << bestPenguin.heat << std::endl;
        

        std::vector<Penguin>& birds = colony.getColony();
        std::vector<Penguin> next_generation = birds;

        for (int i = 0; i < ctx.populationSize; ++i) {
            Penguin& current = birds[i];

            if (current.heat == bestPenguin.heat) continue;

            double dist = EPC_Physics::calculateDistance(current, bestPenguin);
            double Q = EPC_Physics::calculateAttraction(dist, current_mu);

            next_generation[i].position = EPC_Physics::computeNewPosition(
                current, bestPenguin, Q, 
                ctx.spiral_a, ctx.spiral_b, 
                current_m, ctx
            );

            next_generation[i].heat = ctx.costFunction(next_generation[i].position);
        }

        birds = next_generation;

        current_mu *= ctx.cooling_factor;
        current_m  *= ctx.cooling_factor;
    }

    Penguin finalBest = getGlobalBest();
    std::cout << "[Optimizer] Finished. Best Cost: " << finalBest.heat << std::endl;
    std::cout << "[Optimizer] Best Position: [ ";
    for(double val : finalBest.position) std::cout << val << " ";
    std::cout << "]" << std::endl;
}

Penguin EPC_Optimizer::getGlobalBest() const {
    
    EPC_Population& mutable_colony = const_cast<EPC_Population&>(colony);
    const auto& birds = mutable_colony.getColony();

    int bestIdx = 0;
    double bestHeat = birds[0].heat;

    for (size_t i = 1; i < birds.size(); ++i) {
        if (isBetter(birds[i].heat, bestHeat)) {
            bestHeat = birds[i].heat;
            bestIdx = i;
        }
    }
    return birds[bestIdx];
}