#ifndef EPC_POPULATION_H
#define EPC_POPULATION_H

#include <vector>
#include <random>
#include "EPC_Data.h"

class EPC_Population {
private:
    ProblemContext ctx;                   
    std::vector<Penguin> penguins;        
    
    
    std::mt19937 rng;

public:
    
    EPC_Population(const ProblemContext& context);

    
    void initialize();

    
    const Penguin& getPenguin(int index) const;

    
    std::vector<Penguin>& getColony();
};

#endif 