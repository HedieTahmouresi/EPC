#include <systemc.h>
#include <iostream>
#include "EPC_Shared.h"
#include "EPC_Host.h"
#include "EPC_Accelerator.h"
#include "EPC_Monitor.h"

int sc_main(int argc, char* argv[]) {
    const int POPULATION_SIZE = 20;
    const int DIMENSIONS = 20; //10;
    const int MAX_ITERATIONS = 2000;  
    
    const double LOWER_BOUND = -5.12;
    const double UPPER_BOUND = 5.12;
    
    const CostFunctionID BENCHMARK = FUNC_SPHERE;
    const OptimizationMode MODE = Minimize; //Maximize;

    // const int POPULATION_SIZE = 20;
    // const int DIMENSIONS = 10;
    // const int MAX_ITERATIONS = 2000;  
    
    // const double LOWER_BOUND = -5.12;
    // const double UPPER_BOUND = 5.12;
    
    // const CostFunctionID BENCHMARK = FUNC_ROSENBROCK;
    // const OptimizationMode MODE = Minimize;

    
    // const int POPULATION_SIZE = 20;
    // const int DIMENSIONS = 15;
    // const int MAX_ITERATIONS = 100;  
    
    // const double LOWER_BOUND = -10.0;
    // const double UPPER_BOUND = 10.0;
    
    // const CostFunctionID BENCHMARK = FUNC_RASTRIGIN;
    // const OptimizationMode MODE = Maximize;

    // const int POPULATION_SIZE = 20;
    // const int DIMENSIONS = 15;
    // const int MAX_ITERATIONS = 500;  
    
    // const double LOWER_BOUND = -10.0;
    // const double UPPER_BOUND = 10.0;
    
    // const CostFunctionID BENCHMARK = FUNC_ACKLEY;
    // const OptimizationMode MODE = Minimize;

    sc_fifo<Update_Job>      bus_host_to_acc("BUS_H2A", 128); 
    sc_fifo<Penguin_Packet>  bus_acc_to_host("BUS_A2H", 128);

    sc_signal<int>    sig_iter;
    sc_signal<double> sig_best_cost;
    sc_signal<bool>   sig_done;

    EPC_Host host("Host_CPU", 
                  POPULATION_SIZE, DIMENSIONS, MAX_ITERATIONS,
                  LOWER_BOUND, UPPER_BOUND, MODE, BENCHMARK);
    
    EPC_Accelerator accel("EPC_Accelerator");

    EPC_Monitor monitor("System_Monitor");

    sc_clock system_clk("system_clk", 10, SC_NS);
    
    accel.clk(system_clk); 
    host.clk(system_clk);

    host.o_hw_job(bus_host_to_acc);
    host.i_hw_res(bus_acc_to_host);
    host.o_iter(sig_iter);           
    host.o_best_cost(sig_best_cost);  
    host.o_done(sig_done);

    accel.i_job_bus(bus_host_to_acc);
    accel.o_res_bus(bus_acc_to_host);

    monitor.i_iter(sig_iter);
    monitor.i_best_cost(sig_best_cost);
    monitor.i_done(sig_done);

    sc_trace_file* tf = sc_create_vcd_trace_file("EPC_Trace");
    
    std::cout << "=============================================" << std::endl;
    std::cout << "   EPC CO-DESIGN SIMULATION (SystemC)        " << std::endl;
    std::cout << "=============================================" << std::endl;
    std::cout << " Bench: " << BENCHMARK << " | Dim: " << DIMENSIONS << std::endl;
    std::cout << " HW Cores: " << NUM_CORES << std::endl;
    std::cout << "=============================================" << std::endl;

    sc_start();

    sc_close_vcd_trace_file(tf);
    std::cout << "Simulation Finished." << std::endl;

    return 0;
}