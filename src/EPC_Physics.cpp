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
    //if (raw_attraction > 1.0) {
    //    return 1.0 / (1.0 + raw_attraction);
    //} else {
    //    return raw_attraction;
    //}
    return raw_attraction;
}

double calculateSpiralDistance(double theta_i, double theta_j, double a, double b) {
    double factor = (a / b) * std::sqrt(b * b + 1.0);
    double diff = std::abs(std::exp(b * theta_j) - std::exp(b * theta_i));
    return factor * diff;
}

std::pair<double, double> EPC_Physics::computeSpiralPair(
    std::pair<double, double> current, 
    std::pair<double, double> target, 
    double mu, double a, double b
) {
    double theta_i = std::atan2(current.second, current.first);
    double theta_j = std::atan2(target.second, target.first);

    double dist = calculateSpiralDistance(theta_i, theta_j, a, b);

    double Q = calculateAttraction(dist, mu);

    double term1 = (1.0 - Q) * std::exp(b * theta_i);
    double term2 = Q * std::exp(b * theta_j);
    double S = term1 + term2;

    double theta_k = (1.0 / b) * std::log(S);
    double r_k = a * S;

    return {r_k * std::cos(theta_k), r_k * std::sin(theta_k)};
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