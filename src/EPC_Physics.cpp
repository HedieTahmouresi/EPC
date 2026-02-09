#include "EPC_Physics.h"
#include <cmath> 

double EPC_Physics::calculateDistance(const Penguin& p1, const Penguin& p2) {
    double sumSquaredDiff = 0.0;
    
    size_t D = p1.position.size();
    for (size_t d = 0; d < D; ++d) {
        double diff = p1.position[d] - p2.position[d];
        sumSquaredDiff += (diff * diff);
    }
    
    return std::sqrt(sumSquaredDiff);
}

double EPC_Physics::calculateAttraction(double distance, double mu) {
    return std::exp(-mu * distance);
}