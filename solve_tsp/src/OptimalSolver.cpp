#include "OptimalSolver.hpp"
OptimalSolver::OptimalSolver()
{
    // 通用设定参数
    this->tolerance = 5e-4;
    this->max_iteration = 100000;
    // 模拟退火算法相关参数
    this->temperature_start = 50000;
    this->temperature_end = 1e-4;
    this->q = 0.98;
};

std::vector<int> OptimalSolver::RandomPermutation(int node_num)
{
    std::vector<int> seq(node_num);
    std::iota(seq.begin(), seq.end(), 0);

    std::random_device rd;
    std::mt19937 g(rd());

    std::shuffle(seq.begin(), seq.end(), g);
    return seq;
};

void OptimalSolver::SimulatedAnnealing(const NetworkManager &network)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dis(0.0, 1.0);

    double T = temperature_start;
    int counter = 0;
    std::vector<int> current_path = RandomPermutation(network.node_num);
    double current_value = network.CalculatePathLength(current_path);
    while (T >= temperature_end && counter <= max_iteration)
    {
        std::vector<int> new_path = RandomPermutation(network.node_num);
        double new_value = network.CalculatePathLength(new_path);
        double delta_value = new_value - current_value;
        if (delta_value >= 0)
        {
            double r = dis(gen);
            if (r <= exp(-delta_value / T))
            {

                current_path = new_path;
                current_value = new_value;
            }
        }
        else
        {
            current_path = new_path;
            current_value = new_value;
        }
        T *= q;
        ++counter;
        std::cout << "最优路径长度: " << current_value << std::endl;
    }
};

int OptimalSolver::solve(const NetworkManager &network, std::string method_solve)
{
    // 开始计时
    auto start = std::chrono::high_resolution_clock::now();

    if (method_solve == "SimulatedAnnealing")
    {
        SimulatedAnnealing(network);
    }

    // 结束计时
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "代码执行时间: " << duration.count() << " 微秒" << std::endl;
    return 0;
};