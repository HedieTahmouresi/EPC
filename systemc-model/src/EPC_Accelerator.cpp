#include "EPC_Accelerator.h"

void EPC_Accelerator::dispatch_unit() {
    int core_idx = 0;
    while(true) {
        Update_Job job = i_job_bus.read();
        
        wait(1); 
        
        internal_job_q[core_idx].write(job);
        core_idx = (core_idx + 1) % NUM_CORES;
    }
}

void EPC_Accelerator::collect_unit() {
    while(true) {
        bool collected_data = false;

        for (int i = 0; i < NUM_CORES; ++i) {
            Penguin_Packet res;
            
            if (internal_res_q[i].nb_read(res)) {
                
                wait(1); 
                
                o_res_bus.write(res);
                collected_data = true;
                
                break;
            }
        }

        if (!collected_data) {
            wait(1);
        }
    }
}