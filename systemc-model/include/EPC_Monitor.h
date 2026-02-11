#ifndef EPC_MONITOR_H
#define EPC_MONITOR_H

#include <systemc.h>
#include <iostream>

SC_MODULE(EPC_Monitor) {
    sc_in<int> i_iter;
    sc_in<double> i_best_cost;
    sc_in<bool> i_done;

    void print_status();

    void check_done();

    SC_CTOR(EPC_Monitor) {
        SC_METHOD(print_status);
        sensitive << i_iter; 
        dont_initialize();

        SC_METHOD(check_done);
        sensitive << i_done;
        dont_initialize();
    }
};

#endif // EPC_MONITOR_H