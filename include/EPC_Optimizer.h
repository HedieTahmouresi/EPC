#ifndef EPC_OPTIMIZER_H
#define EPC_OPTIMIZER_H

#include "EPC_Data.h"
#include "EPC_Population.h"

class EPC_Optimizer {
private:
    ProblemContext ctx;
    EPC_Population colony;

    double current_mu;  
    double current_m;   

    const double spiral_a = 1.0;
    const double spiral_b = 0.5;
    const double cooling_factor = 0.99; 

public:
    EPC_Optimizer(const ProblemContext& context);

    void run();

    Penguin getGlobalBest() const;
};

#endif 