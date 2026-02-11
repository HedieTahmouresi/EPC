#ifndef EPC_SHARED_H
#define EPC_SHARED_H

#include <systemc.h>
#include <iostream>
#include <iomanip>

#define MAX_DIMENSIONS 1000

enum CostFunctionID {
    FUNC_SPHERE = 0,
    FUNC_ROSENBROCK = 1,
    FUNC_RASTRIGIN = 2,
    FUNC_ACKLEY = 3
};

enum JobType {
    JOB_EVAL_COST,
    JOB_CALC_DIST,
    JOB_UPDATE_POS
};

struct Penguin_Packet {
    int target_id;
    double temp_distance;

    int id;
    double position[MAX_DIMENSIONS];
    double cost;
    int dim_size;

    Penguin_Packet() : id(-1), cost(0.0), dim_size(0) {
        for(int i=0; i<MAX_DIMENSIONS; ++i) position[i] = 0.0;
    }

    bool operator == (const Penguin_Packet& other) const {
        return (id == other.id && cost == other.cost);
    }

    friend std::ostream& operator << (std::ostream& os, const Penguin_Packet& p) {
        os << "[ID:" << p.id << " Cost:" << p.cost << "]";
        return os;
    }
};

struct Update_Job {
    JobType type;
    double precalc_distance;

    Penguin_Packet penguin;
    Penguin_Packet target;
    
    double temperature;
    double m_factor;
    double binary_r;
    
    double lower_bound;
    double upper_bound;
    double spiral_param_a;
    double spiral_param_b;
    
    CostFunctionID func_id;

    bool operator == (const Update_Job& other) const { return this == &other; }
    friend std::ostream& operator << (std::ostream& os, const Update_Job& j) {
        os << "Job(P:" << j.penguin.id << " -> T:" << j.target.id << ")";
        return os;
    }
};

#endif