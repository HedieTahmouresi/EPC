#include "EPC_Host.h"
#include <iostream>
#include <cstdlib>

void EPC_Host::initialize_population() {
    population.resize(population_size);
    
    std::cout << "[Host] Initializing " << population_size << " penguins..." << std::endl;

    for(int i=0; i<population_size; ++i) {
        population[i].id = i;
        population[i].dim_size = dimensions;
        
        for(int d=0; d<dimensions; ++d) {
            double r = (double)rand() / RAND_MAX;
            population[i].position[d] = lower_bound + r * (upper_bound - lower_bound);
        }
    }

    for(int i=0; i<population_size; ++i) {
        Update_Job job;
        
        job.type = JOB_EVAL_COST; 
        
        job.penguin = population[i];
        job.target  = population[i];
        
        job.m_factor = 0.0;          
        job.temperature = 0.0;       
        job.spiral_param_a = 0.0;    
        
        job.lower_bound = lower_bound;
        job.upper_bound = upper_bound;
        job.func_id = func_id;       

        o_hw_job.write(job);
    }

    for(int i=0; i<population_size; ++i) {
        Penguin_Packet p = i_hw_res.read();
        population[i] = p;
    }
    
    global_best = population[0];
}

void EPC_Host::sort_population() {
    std::sort(population.begin(), population.end(), 
        [this](const Penguin_Packet& a, const Penguin_Packet& b) {
            if (this->mode == Minimize)
                return a.cost < b.cost;
            else
                return a.cost > b.cost;
        }
    );
    
    bool new_best_found = false;
    if (this->mode == Minimize) {
        if (population[0].cost < global_best.cost) new_best_found = true;
    } else {
        if (population[0].cost > global_best.cost) new_best_found = true;
    }

    if (new_best_found) {
        global_best = population[0];
    }
}

void EPC_Host::update_parameters() {
    current_mu *= cooling_factor;
    current_m  *= m_decay; 
}

void EPC_Host::run_thread() {
    initialize_population();
    
    std::vector<int> sorted_indices(population_size);
    std::iota(sorted_indices.begin(), sorted_indices.end(), 0);

    std::vector<int> assignments(population_size, -1);

    std::cout << "[Host] Leaders: " << num_leaders 
              << " | Capacity: " << leader_capacity << std::endl;

    for (int t = 0; t < max_iterations; ++t) {
        
        std::sort(sorted_indices.begin(), sorted_indices.end(), 
            [&](int a, int b) {
                if (mode == Minimize) return population[a].cost < population[b].cost;
                else return population[a].cost > population[b].cost;
            }
        );

        int best_idx = sorted_indices[0];
        if ((mode == Minimize && population[best_idx].cost < global_best.cost) ||
            (mode == Maximize && population[best_idx].cost > global_best.cost)) {
            global_best = population[best_idx];
        }

        std::vector<int> leader_counts(num_leaders, 0);
        std::fill(assignments.begin(), assignments.end(), -1);
        std::vector<double> precalc_distances(population_size, 0.0);

        for (int i = num_leaders; i < population_size; ++i) {
            int p_idx = sorted_indices[i]; 
            int valid_leaders = 0;

            for (int l = 0; l < num_leaders; ++l) {
                if (leader_counts[l] >= leader_capacity) continue;
                
                Update_Job dist_job;
                dist_job.type = JOB_CALC_DIST;
                dist_job.penguin = population[p_idx];
                dist_job.target = population[sorted_indices[l]];
                o_hw_job.write(dist_job);
                valid_leaders++;
            }

            int best_leader_idx = -1;
            double max_attraction = -1.0;
            double chosen_distance = 0.0;

            for (int l = 0; l < valid_leaders; ++l) {
                Penguin_Packet hw_res = i_hw_res.read(); 
                double dist = hw_res.temp_distance;
                double Q = std::exp(-dist * current_mu); 

                if (Q > max_attraction) {
                    max_attraction = Q;
                    chosen_distance = dist;
                    for (int k = 0; k < num_leaders; ++k) {
                        if (sorted_indices[k] == hw_res.target_id) best_leader_idx = k;
                    }
                }
            }

            if (best_leader_idx != -1) {
                assignments[p_idx] = sorted_indices[best_leader_idx];
                precalc_distances[p_idx] = chosen_distance;
                leader_counts[best_leader_idx]++;
            } else {
                assignments[p_idx] = sorted_indices[0]; 
            }
        }

        for (int i = 0; i < population_size; ++i) {
            int p_idx = sorted_indices[i];
            Update_Job job;
            job.type = JOB_UPDATE_POS;
            job.penguin = population[p_idx]; 
            job.func_id = func_id;
            job.lower_bound = lower_bound;
            job.upper_bound = upper_bound;
            job.temperature = current_mu;
            job.m_factor = current_m;
            job.spiral_param_a = spiral_a;
            job.spiral_param_b = spiral_b;

            if (i == 0) {
                job.target = population[p_idx]; 
                job.precalc_distance = 0.0;
                o_hw_job.write(job);
            }
            else if (i < num_leaders) {
                job.target = population[sorted_indices[0]];
                job.precalc_distance = precalc_distances[p_idx];
                o_hw_job.write(job);
            }
            else {
                int leader_id = assignments[p_idx];
                job.target = population[leader_id];
                job.precalc_distance = precalc_distances[p_idx];
                o_hw_job.write(job);
            }
        }

        for (int i = 0; i < population_size; ++i) {
            Penguin_Packet res = i_hw_res.read(); 
            
            int p_idx = res.id; 

            if (p_idx == sorted_indices[0]) {
                bool improved = false;
                if (mode == Minimize && res.cost < population[p_idx].cost) improved = true;
                if (mode == Maximize && res.cost > population[p_idx].cost) improved = true;

                if (improved) {
                    population[p_idx] = res; 
                } 
            } 
            else {
                population[p_idx] = res;
            }
        }

        current_mu *= cooling_factor;
        current_m *= m_decay;

        o_iter.write(t);
        o_best_cost.write(global_best.cost);
    }
    
    std::cout << "[Host] Final Best: " << global_best.cost << std::endl;
    sc_stop();
}