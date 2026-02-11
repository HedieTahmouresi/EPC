#ifndef EPC_PROCESSING_ELEMENT_H
#define EPC_PROCESSING_ELEMENT_H

#include "EPC_Shared.h"
#include "EPC_Hardware_Math.h"
#include <systemc.h>
#include <random>

SC_MODULE(EPC_Processing_Element) {
    sc_in<bool> clk;
    sc_fifo_in<Update_Job>  i_job;        
    sc_fifo_out<Penguin_Packet> o_result; 

    std::mt19937 rng;
    std::uniform_real_distribution<double> dist_u;

    void process_thread();

    SC_HAS_PROCESS(EPC_Processing_Element);

    EPC_Processing_Element(sc_module_name name, int core_id) 
        : sc_module(name), dist_u(-1.0, 1.0) {
        
        rng.seed(12345 + core_id); 
        
        SC_THREAD(process_thread);
        sensitive << clk.pos();
    }
};

#endif