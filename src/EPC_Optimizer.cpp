#include "EPC_Optimizer.h"
#include "EPC_Physics.h"
#include <iostream>
#include <algorithm> 
#include <vector>
#include <numeric>


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
    std::cout << "[Optimizer] Starting Hierarchical Optimization..." << std::endl;
    std::cout << "[Config] Leaders: " << ctx.numLeaders 
              << " | Capacity: " << ctx.leaderCapacity << std::endl;
    
    colony.initialize();

    std::vector<int> sorted_indices(ctx.populationSize);
    std::iota(sorted_indices.begin(), sorted_indices.end(), 0);

    std::vector<int> assignments(ctx.populationSize, -1);

    for (int t = 0; t < ctx.maxIterations; ++t) {
        
        std::vector<Penguin>& birds = colony.getColony();
        std::vector<Penguin> next_generation = birds;

        std::sort(sorted_indices.begin(), sorted_indices.end(), [&](int a, int b) {
            return isBetter(birds[a].heat, birds[b].heat);
        });

        int globalBestIdx = sorted_indices[0];
        Penguin& globalBest = birds[globalBestIdx];
        
        std::cout << "Iter " << t << " | Global Best: " << globalBest.heat << std::endl;

        std::vector<int> leaderCounts(ctx.numLeaders, 0);
        
        std::fill(assignments.begin(), assignments.end(), -1);

        for (int i = ctx.numLeaders; i < ctx.populationSize; ++i) {
            int p_idx = sorted_indices[i];
            Penguin& p = birds[p_idx];

            int bestLeaderIdx = -1;
            double maxAttraction = -1.0;

            for (int l = 0; l < ctx.numLeaders; ++l) {
                if (leaderCounts[l] >= ctx.leaderCapacity) continue;

                int leader_real_idx = sorted_indices[l];
                double dist = EPC_Physics::calculateDistance(p, birds[leader_real_idx]);
                
                double Q = EPC_Physics::calculateAttraction(dist, current_mu);

                if (Q > maxAttraction) {
                    maxAttraction = Q;
                    bestLeaderIdx = l;
                }
            }

            if (bestLeaderIdx != -1) {
                assignments[p_idx] = sorted_indices[bestLeaderIdx];
                leaderCounts[bestLeaderIdx]++;
            } else {
                assignments[p_idx] = sorted_indices[0];
            }
        }

        for (int i = 0; i < ctx.populationSize; ++i) {
            int p_idx = sorted_indices[i];
            Penguin& current = birds[p_idx];

            if (i == 0) {
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
            }
            else if (i < ctx.numLeaders) {
                next_generation[p_idx].position = EPC_Physics::computeNewPosition(
                    current, globalBest, current_mu, 
                    ctx.spiral_a, ctx.spiral_b, 
                    current_m, ctx
                );
                next_generation[p_idx].heat = ctx.costFunction(next_generation[p_idx].position);
            }
            else {
                int leader_idx = assignments[p_idx];
                Penguin& myLeader = birds[leader_idx];

                next_generation[p_idx].position = EPC_Physics::computeNewPosition(
                    current, myLeader, current_mu, 
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