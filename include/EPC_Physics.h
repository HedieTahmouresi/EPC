#ifndef EPC_PHYSICS_H
#define EPC_PHYSICS_H

#include "EPC_Data.h"

class EPC_Physics {
public:
    static double calculateDistance(const Penguin& p1, const Penguin& p2);

    static double calculateAttraction(double distance, double mu);

    static std::pair<double, double> computeSpiralPair(
        std::pair<double, double> current, 
        std::pair<double, double> target,  
        double Q, double a, double b
    );

    static std::vector<double> computeNewPosition(
        const Penguin& current, 
        const Penguin& best, 
        double Q, 
        double a, 
        double b,
        double mutation_m, 
        const ProblemContext& ctx
    );
};

#endif