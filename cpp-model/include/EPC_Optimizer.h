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

    bool isBetter(double val1, double val2) const;

public:
    EPC_Optimizer(const ProblemContext& context);

    void run();

    Penguin getGlobalBest() const;
};

#endif 