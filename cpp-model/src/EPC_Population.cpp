#include "EPC_Population.h"
#include <iostream>

EPC_Population::EPC_Population(const ProblemContext& context) 
    : ctx(context) //, rng(std::random_device{}()) // completely random
    {
    penguins.reserve(ctx.populationSize);
    }

void EPC_Population::initialize() {
    penguins.clear();

    std::uniform_real_distribution<double> random_r(0.0, 1.0);

    for (int i = 0; i < ctx.populationSize; ++i) {
        Penguin p(ctx.dimensions);
        
        for (int d = 0; d < ctx.dimensions; ++d) {
            // double r = random_r(rng); // completely random
            double r = random_r(ctx.rng); //random with seed 
            p.position[d] = ctx.lowerBound + r * (ctx.upperBound - ctx.lowerBound);
        }
        
        if (ctx.costFunction) {
            p.heat = ctx.costFunction(p.position);
        }

        penguins.push_back(p);
    }
}

const Penguin& EPC_Population::getPenguin(int index) const {
    return penguins[index];
}

std::vector<Penguin>& EPC_Population::getColony() {
    return penguins;
}