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
    double raw_attraction = (std::exp(-mu * d)) / d;
    if (raw_attraction > 1.0) {
        return 1.0 / (1.0 + raw_attraction);
    } else {
        return raw_attraction;
    }
}

std::pair<double, double> EPC_Physics::computeSpiralPair(
    std::pair<double, double> current, 
    std::pair<double, double> target, 
    double Q, double a, double b
) {
    double theta_i = std::atan2(current.second, current.first);
    double theta_j = std::atan2(target.second, target.first);

    double term1 = (1.0 - Q) * std::exp(b * theta_i);
    double term2 = Q * std::exp(b * theta_j);
    double S = term1 + term2;

    double theta_k = (1.0 / b) * std::log(S);
    double r_k = a * S;

    return {r_k * std::cos(theta_k), r_k * std::sin(theta_k)};
}

std::vector<double> EPC_Physics::computeNewPosition(
    const Penguin& current, const Penguin& best, 
    double Q, double a, double b, double mutation_m,
    const ProblemContext& ctx
) {
    size_t D = ctx.dimensions;
    std::vector<double> final_position(D);

    for (size_t i = 0; i < D - 1; i += 2) {
        size_t j = i + 1;

        std::pair<double, double> curr_pair = {current.position[i], current.position[j]};
        std::pair<double, double> target_pair = {best.position[i], best.position[j]};
        
        std::pair<double, double> new_pair = computeSpiralPair(curr_pair, target_pair, Q, a, b);

        final_position[i] = new_pair.first;
        final_position[j] = new_pair.second;
    }

    if (D % 2 != 0) {
        size_t last = D - 1;
        final_position[last] = current.position[last] + Q * (best.position[last] - current.position[last]);
    }

    std::mt19937 rng(std::random_device{}());
    std::uniform_real_distribution<double> dist_u(-1.0, 1.0);

    for (size_t d = 0; d < D; ++d) {
        double u = dist_u(rng);
        final_position[d] += mutation_m * u;

        if (final_position[d] > ctx.upperBound) final_position[d] = ctx.upperBound;
        if (final_position[d] < ctx.lowerBound) final_position[d] = ctx.lowerBound;
    }

    return final_position;
}