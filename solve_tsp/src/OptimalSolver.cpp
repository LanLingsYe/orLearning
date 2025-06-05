#include "OptimalSolver.hpp"

/// The gen is a static member of the OptimalSolver class
/// that utilizes the std::mt19937_64 engine to generate high-quality 64-bit pseudo-random numbers,
/// suitable for applications like stochastic algorithms, simulations, and reproducible random number sequences.
static std::mt19937_64 gen{std::random_device{}()};

OptimalSolver::OptimalSolver()
{
    this->tolerance = 5e-4;
    this->max_iteration = 1000;
    this->mature_iteration = 40;
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

    // 结束计时
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "代码执行时间: " << duration.count() << " 微秒" << std::endl;
    return 0;
};