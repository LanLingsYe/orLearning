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
    size_t mature_iteration;
    double temperature_start;
    double temperature_end;
    double q;
    // 类似与随机数种子，给整个类使用
    static std::mt19937_64 gen;

public:
    OptimalSolver();
    std::vector<int> RandomPermutation(int node_num);
    int solve(const NetworkManager &network, std::string method_solve);

    void SimulatedAnnealing(const NetworkManager &network);

    void VariableNeighborhoodSearch(const NetworkManager &network);
    std::vector<int> BridgeMove(const NetworkManager &network, const std::vector<int> &original_path);
    std::vector<int> Neighborhood_one(const NetworkManager &network, const std::vector<int> &original_path);
    std::tuple<std::vector<int>, double> VariableNeighborhoodDescent(const NetworkManager &network, const std::vector<int> &created_path);
};
