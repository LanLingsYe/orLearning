#include "OptimalSolver.hpp"

void OptimalSolver::SimulatedAnnealing(const NetworkManager &network)
{
    std::uniform_real_distribution<double> dis(0.0, 1.0);
    double T = temperature_start;
    size_t counter = 0;
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