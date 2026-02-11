#include "EPC_Hardware_Math.h"
#include <cmath>
#include <systemc.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#ifndef M_E
#define M_E 2.71828182845904523536
#endif

bool Hardware_ALU::is_better(double new_raw_cost, double old_raw_cost, int sign) {
    double eff_new = new_raw_cost * sign;
    double eff_old = old_raw_cost * sign;
    return eff_new < eff_old;
}

double Hardware_ALU::compute_distance(const Penguin_Packet& p1, const Penguin_Packet& p2) {
    double sum_sq = 0.0;
    for(int i = 0; i < p1.dim_size; ++i) {
        double diff = p1.position[i] - p2.position[i];
        sum_sq += diff * diff;
    }
    return std::sqrt(sum_sq);
}

double Hardware_ALU::clamp(double val, double min, double max) {
    if (val < min) return min;
    if (val > max) return max;
    return val;
}

double Hardware_ALU::calculate_cost(const Penguin_Packet& p, CostFunctionID func_id) {
    double val = 0.0;
    
    switch(func_id) {
        case FUNC_SPHERE: {
            for (int i = 0; i < p.dim_size; ++i) {
                val += p.position[i] * p.position[i];
            }
            break;
        }

        case FUNC_ROSENBROCK: {
            for (int i = 0; i < p.dim_size - 1; ++i) {
                double xi = p.position[i];
                double xi_next = p.position[i+1];
                double t1 = 100.0 * std::pow(xi_next - xi * xi, 2);
                double t2 = std::pow(xi - 1.0, 2);
                val += t1 + t2;
            }
            break;
        }

        case FUNC_RASTRIGIN: {
            val = 10.0 * p.dim_size;
            for (int i = 0; i < p.dim_size; ++i) {
                val += (p.position[i] * p.position[i]) - (10.0 * std::cos(2.0 * M_PI * p.position[i]));
            }
            break;
        }
        
        case FUNC_ACKLEY: {
            double sum1 = 0.0, sum2 = 0.0;
            for (int i = 0; i < p.dim_size; ++i) {
                sum1 += p.position[i] * p.position[i];
                sum2 += std::cos(2.0 * M_PI * p.position[i]);
            }
            val = -20.0 * std::exp(-0.2 * std::sqrt(sum1 / p.dim_size)) 
                  - std::exp(sum2 / p.dim_size) + 20.0 + M_E;
            break;
        }

        default:
            val = 1.0e15;
            break;
    }
    return val;
}