#include "EPC_Monitor.h"

void EPC_Monitor::print_status() {
    int iter = i_iter.read();
        if (iter > 0 && iter % 10 == 0) {
            std::cout << "[Monitor] Iteration: " << iter 
                      << " | Current Best Cost: " << i_best_cost.read() << std::endl;
        }
}

void EPC_Monitor::check_done() {
    if (i_done.read() == true) {
        std::cout << "=============================================" << std::endl;
        std::cout << "[Monitor] Simulation Complete!" << std::endl;
        std::cout << "[Monitor] Final Global Best: " << i_best_cost.read() << std::endl;
        std::cout << "=============================================" << std::endl;
        sc_stop(); 
    }
}