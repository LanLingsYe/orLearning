#include "OptimalSolver.hpp"

std::mt19937_64 OptimalSolver::gen{std::random_device{}()};

OptimalSolver::OptimalSolver()
{
    // 通用设定参数
    this->tolerance = 5e-4;
    this->max_iteration = 1000;
    this->mature_iteration = 40;
    // 模拟退火算法相关参数
    this->temperature_start = 50000;
    this->temperature_end = 1e-4;
    this->q = 0.98;
};

std::vector<int> OptimalSolver::RandomPermutation(int node_num)
{
    std::vector<int> seq(node_num);
    std::iota(seq.begin(), seq.end(), 0);

    std::shuffle(seq.begin(), seq.end(), gen);
    return seq;
};

int OptimalSolver::solve(const NetworkManager &network, std::string method_solve)
{
    // 开始计时
    auto start = std::chrono::high_resolution_clock::now();

    if (method_solve == "SA")
    {
        SimulatedAnnealing(network);
    }

    if (method_solve == "VNS")
    {
        VariableNeighborhoodSearch(network);
    }

    // 结束计时
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "代码执行时间: " << duration.count() << " 微秒" << std::endl;
    return 0;
};