#ifndef EPC_ACCELERATOR_H
#define EPC_ACCELERATOR_H

#include "EPC_Shared.h"
#include "EPC_Processing_Element.h"
#include <systemc.h>

#define NUM_CORES 10 

SC_MODULE(EPC_Accelerator) {
    sc_in<bool> clk;           
    sc_fifo_in<Update_Job>  i_job_bus;    
    sc_fifo_out<Penguin_Packet> o_res_bus;

    sc_vector<EPC_Processing_Element> cores;
    sc_vector<sc_fifo<Update_Job>> internal_job_q;
    sc_vector<sc_fifo<Penguin_Packet>> internal_res_q;

    void dispatch_unit(); 
    void collect_unit();  

    SC_CTOR(EPC_Accelerator) 
        : cores("cores"), 
          internal_job_q("internal_job_q", NUM_CORES),
          internal_res_q("internal_res_q", NUM_CORES) 
    {
        cores.init(NUM_CORES, [](const char* name, size_t i) {
            return new EPC_Processing_Element(name, i);
        });

        for (int i = 0; i < NUM_CORES; ++i) {
            cores[i].clk(clk); 
            cores[i].i_job(internal_job_q[i]);
            cores[i].o_result(internal_res_q[i]);
        }

        SC_THREAD(dispatch_unit);
        sensitive << clk.pos(); 

        SC_THREAD(collect_unit);
        sensitive << clk.pos(); 
    }
};

#endif 