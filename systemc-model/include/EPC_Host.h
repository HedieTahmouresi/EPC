#ifndef EPC_HOST_H
#define EPC_HOST_H

#include <systemc.h>
#include <vector>
#include <algorithm>
#include <numeric>
#include <cmath>
#include "EPC_Shared.h"
#include "EPC_Monitor.h"

enum OptimizationMode { Minimize = 0, Maximize = 1 };

SC_MODULE(EPC_Host) {
    sc_in<bool> clk;
    sc_fifo_out<Update_Job> o_hw_job;
    sc_fifo_in<Penguin_Packet> i_hw_res;
    
    sc_out<int> o_iter;
    sc_out<double> o_best_cost;
    sc_out<bool> o_done;

    std::vector<Penguin_Packet> population;
    Penguin_Packet global_best;
    
    int population_size;
    int dimensions;
    int max_iterations;
    OptimizationMode mode;
    CostFunctionID func_id;
    
    int num_leaders;
    int leader_capacity;

    double current_mu;      
    double current_m;
    double cooling_factor;
    double m_decay;
    double spiral_a;
    double spiral_b;
    double lower_bound;
    double upper_bound;

    void run_thread();

    void initialize_population();
    void sort_population();
    void update_parameters();
    
    double sw_distance(const Penguin_Packet& p1, const Penguin_Packet& p2) {
        double sum = 0;
        for(int i=0; i<dimensions; ++i) 
            sum += std::pow(p1.position[i] - p2.position[i], 2);
        return std::sqrt(sum);
    }

    SC_HAS_PROCESS(EPC_Host);

    EPC_Host(sc_module_name name, int pop_size, int dim, int max_iter,
             double lb, double ub, OptimizationMode opt_mode, CostFunctionID fid)
        : sc_module(name), 
          population_size(pop_size), dimensions(dim), max_iterations(max_iter),
          lower_bound(lb), upper_bound(ub), mode(opt_mode), func_id(fid) 
    {
        current_mu = 0.05; 
        current_m = 0.5;   
        cooling_factor = 0.99;
        m_decay = 0.99; 
        spiral_a = 1.0; 
        spiral_b = 0.5;

        num_leaders = std::max(1, pop_size / 10);
        leader_capacity = (pop_size / std::max(1, pop_size / 10)) + 2;
        
        SC_THREAD(run_thread);
        sensitive << clk.pos();
    }
};

#endif