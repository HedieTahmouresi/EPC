#ifndef EPC_PHYSICS_H
#define EPC_PHYSICS_H

#include "EPC_Data.h"

class EPC_Physics {
public:
    static double calculateDistance(const Penguin& p1, const Penguin& p2);

    static double calculateAttraction(double distance, double mu);
};

#endif