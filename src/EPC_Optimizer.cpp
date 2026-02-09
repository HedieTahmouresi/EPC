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

    int groupSize = 10; 
    int regroupPeriod = 5;

    std::vector<int> indices(ctx.populationSize);
    std::iota(indices.begin(), indices.end(), 0);

    for (int t = 0; t < ctx.maxIterations; ++t) {
        if (t % regroupPeriod == 0) {
            std::shuffle(indices.begin(), indices.end(), ctx.rng);
        }

        Penguin globalBest = getGlobalBest();
        std::cout << "Iter " << t << " | Global Best: " << globalBest.heat << std::endl;

        std::vector<Penguin>& birds = colony.getColony();
        std::vector<Penguin> next_generation = birds;

        for (int g = 0; g < ctx.populationSize; g += groupSize) {
            
            int localBestIdx = -1;
            double localBestHeat = (ctx.mode == OptimizationMode::Minimize) ? 1e15 : -1e15;

            int endIdx = std::min(g + groupSize, ctx.populationSize);
            for (int k = g; k < endIdx; ++k) {
                int p_idx = indices[k];
                if (isBetter(birds[p_idx].heat, localBestHeat)) {
                    localBestHeat = birds[p_idx].heat;
                    localBestIdx = p_idx;
                }
            }
            
            Penguin& localKing = birds[localBestIdx];

            for (int k = g; k < endIdx; ++k) {
                int p_idx = indices[k];
                Penguin& current = birds[p_idx];
                if (current.heat == globalBest.heat) {
                    std::vector<double> mutated_pos = current.position;
                    std::uniform_real_distribution<double> dist_u(-1.0, 1.0);
                    
                    for (double& val : mutated_pos) {
                        val += current_m * dist_u(ctx.rng);
                        if(val > ctx.upperBound) val = ctx.upperBound;
                        if(val < ctx.lowerBound) val = ctx.lowerBound;
                    }

                    if (isBetter(ctx.costFunction(mutated_pos), current.heat)) {
                        next_generation[p_idx].position = mutated_pos;
                        next_generation[p_idx].heat = ctx.costFunction(mutated_pos);
                    } else {
                        next_generation[p_idx] = current;
                    }
                    continue; 
                }

                if (p_idx == localBestIdx) {

                    next_generation[p_idx].position = EPC_Physics::computeNewPosition(
                        current, globalBest, current_mu, 
                        ctx.spiral_a, ctx.spiral_b, 
                        current_m, ctx
                    );
                    next_generation[p_idx].heat = ctx.costFunction(next_generation[p_idx].position);
                    continue;
                }


                next_generation[p_idx].position = EPC_Physics::computeNewPosition(
                    current, localKing, current_mu, 
                    ctx.spiral_a, ctx.spiral_b, 
                    current_m, ctx
                );
                next_generation[p_idx].heat = ctx.costFunction(next_generation[p_idx].position);
            }
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