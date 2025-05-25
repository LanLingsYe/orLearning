#pragma once
#include "NetworkManager.hpp"
#include <algorithm>
#include <random>
#include <vector>
#include <chrono>

class OptimalSolver
{
private:
    double tolerance;
    size_t max_iteration;
    double temperature_start;
    double temperature_end;
    double q;

public:
    OptimalSolver();
    std::vector<int> RandomPermutation(int node_num);
    void SimulatedAnnealing(const NetworkManager &network);
    int solve(const NetworkManager &network, std::string method_solve);
};
