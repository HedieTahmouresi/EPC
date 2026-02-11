#include "EPC_Processing_Element.h"
#include <cmath>

void EPC_Processing_Element::process_thread() {
    while (true) {
        Update_Job job = i_job.read();
        Penguin_Packet result = job.penguin;
        result.target_id = job.target.id; 

        if (job.type == JOB_EVAL_COST) {
            wait(job.penguin.dim_size * 2); 
            result.cost = Hardware_ALU::calculate_cost(result, job.func_id);
            o_result.write(result);
            continue;
        }

        if (job.type == JOB_CALC_DIST) {
            wait(job.penguin.dim_size * 2); 
            result.temp_distance = Hardware_ALU::compute_distance(job.penguin, job.target);
            o_result.write(result);
            continue;
        }

        if (job.type == JOB_UPDATE_POS) {
            double dist = job.precalc_distance; 

            if (dist < 1.0e-9 || job.target.id == job.penguin.id) { 
                wait(job.penguin.dim_size * 1); 
                for (int d = 0; d < job.penguin.dim_size; ++d) {
                    double u = dist_u(rng); 
                    result.position[d] += job.m_factor * u;
                    result.position[d] = Hardware_ALU::clamp(result.position[d], job.lower_bound, job.upper_bound);
                }
            } 
            else {
                wait(20); 
                double Q = std::exp(-dist * job.temperature);
                Q = Hardware_ALU::clamp(Q, 0.0, 1.0);

                double term1 = job.spiral_param_a * Q;
                double term2 = std::exp(job.spiral_param_b * (Q - 1.0));
                double term3 = std::cos(2.0 * M_PI * Q);
                double spiral_scalar = term1 * term2 * term3;

                wait(job.penguin.dim_size * 3); 
                for (int d = 0; d < job.penguin.dim_size; ++d) {
                    double direction_vec = job.target.position[d] - job.penguin.position[d];
                    result.position[d] += (direction_vec * spiral_scalar) + (job.m_factor * dist_u(rng));
                    result.position[d] = Hardware_ALU::clamp(result.position[d], job.lower_bound, job.upper_bound);
                }
            }

            wait(job.penguin.dim_size * 2); 
            result.cost = Hardware_ALU::calculate_cost(result, job.func_id);
            o_result.write(result);
        }
    }
}