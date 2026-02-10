#include "EPC_Physics.h"
#include <cmath>
#include <random>

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
    double d = std::max(distance, 1e-10);
    double raw_attraction = (std::exp(-mu * d));
    return raw_attraction;
}


std::vector<double> EPC_Physics::computeNewPosition(
    const Penguin& current, const Penguin& best, 
    double mu, double a, double b, double mutation_m,
    ProblemContext& ctx 
) {
    size_t D = ctx.dimensions;
    std::vector<double> final_position(D);
    
    double dist = calculateDistance(current, best);
    double Q = calculateAttraction(dist, mu);
    
    // double spiral_scalar = a * Q * std::cos(2.0 * M_PI * Q);
    double spiral_scalar = a * Q * std::exp(b * (Q-1)) * std::cos(2.0 * M_PI * Q);

    for (size_t d = 0; d < D; ++d) {
        double direction_vec = best.position[d] - current.position[d];
        
        final_position[d] = current.position[d] + (direction_vec * spiral_scalar);
    }

    std::uniform_real_distribution<double> dist_u(-1.0, 1.0);

    for (size_t d = 0; d < D; ++d) {
        double u = dist_u(ctx.rng);
        final_position[d] += mutation_m * u;

        if (final_position[d] > ctx.upperBound) final_position[d] = ctx.upperBound;
        if (final_position[d] < ctx.lowerBound) final_position[d] = ctx.lowerBound;
    }

    return final_position;
}