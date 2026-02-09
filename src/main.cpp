#include <iostream>
#include "EPC_Data.h"
#include "EPC_Physics.h"

int main() {
    std::cout << "[System] Testing Physics Engine..." << std::endl;

    Penguin p1(4); 
    p1.position = {2.0, 1.0, -1.0, 0.0}; 

    Penguin p3(4);
    p3.position = {3.0, 2.0, 0.0, -1.0}; 

    double dist = EPC_Physics::calculateDistance(p3, p1);
    std::cout << "[Test] Distance Calculated: " << dist << std::endl;
    std::cout << "[Expected] Distance: 2.0" << std::endl;

    double mu = 0.1;
    double Q = EPC_Physics::calculateAttraction(dist, mu);
    std::cout << "[Test] Attraction (Q): " << Q << std::endl;
    std::cout << "[Expected] Attraction: ~0.8187" << std::endl;

    return 0;
}