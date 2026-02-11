#ifndef EPC_HARDWARE_MATH_H
#define EPC_HARDWARE_MATH_H

#include "EPC_Shared.h"

class Hardware_ALU {
public:
    static bool is_better(double new_raw_cost, double old_raw_cost, int sign);

    static double compute_distance(const Penguin_Packet& p1, const Penguin_Packet& p2);

    static double calculate_cost(const Penguin_Packet& p, CostFunctionID func_id);
    
    static double clamp(double val, double min, double max);
};

#endif