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

    void BridgeMove(const NetworkManager &network, std::vector<int> &path);
    double Neighborhood_one(const NetworkManager &network, std::vector<int> &path);
    double Neighborhood_two(const NetworkManager &network, std::vector<int> &path);
    double VariableNeighborhoodDescent(const NetworkManager &network, std::vector<int> &created_path);
    void VariableNeighborhoodSearch(const NetworkManager &network);
};
