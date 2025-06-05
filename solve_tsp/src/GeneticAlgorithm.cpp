#include "OptimalSolver.hpp"

/// The gen is a static member of the OptimalSolver class
/// that utilizes the std::mt19937_64 engine to generate high-quality 64-bit pseudo-random numbers,
/// suitable for applications like stochastic algorithms, simulations, and reproducible random number sequences.
static std::mt19937_64 gen{std::random_device{}()};

size_t population_size = 100;
double mutation_rate = 0.1;
double crossover_rate = 0.9;
std::vector<int> PopulationId;
std::map<int, std::vector<int>> PopulationMap;
std::map<int, double> FitnessMap;

void BridgeMove(const NetworkManager &network, std::vector<int> &path)
{
    std::uniform_int_distribution<int> int_dis(1, 15);
    int breakpoint_num = network.node_num / 15;
    int position = 0;
    std::vector<int> scrambled_piece = {position};
    path.clear();
    for (int iPiece = 0; iPiece < breakpoint_num; ++iPiece)
    {
        position += int_dis(gen);
        scrambled_piece.emplace_back(position);
    }
    scrambled_piece.emplace_back(network.node_num);

    std::vector<int> piece_sequence = OptimalSolver::RandomPermutation(breakpoint_num + 1);
    for (auto &element : piece_sequence)
    {
        for (int iNode = scrambled_piece[element]; iNode < scrambled_piece[element + 1]; ++iNode)
        {
            path.emplace_back(path[iNode]);
        }
    }
    return;
};

void InitPack(const NetworkManager &network)
{
    for (size_t i = 0; i < population_size; ++i)
    {
        PopulationId.emplace_back(i);
        PopulationMap.emplace(i, OptimalSolver::RandomPermutation(network.node_num));
        FitnessMap.emplace(i, network.CalculatePathLength(PopulationMap[i]));
    }
}

void OptimalSolver::GeneticAlgorithm(const NetworkManager &network)
{
    size_t iter = 0, non_iter = 0;
    while (iter <= max_iteration && non_iter <= mature_iteration)
    {
        iter++;
        non_iter++;
        std::vector<int> current_path = RandomPermutation(network.node_num);
        double current_value = network.CalculatePathLength(current_path);
        std::vector<int> new_path = current_path;
        BridgeMove(network, new_path);
        double new_value = network.CalculatePathLength(new_path);
        if (new_value < current_value)
        {
            current_path = new_path;
            current_value = new_value;
            non_iter = 0;
        }
        std::cout << "当前迭代次数: " << iter << ", 最优路径长度: " << current_value << std::endl;
    }
    {
        std::vector<std::vector<int>> population(population_size);
        for (size_t i = 0; i < population_size; ++i)
        {
            population[i] = RandomPermutation(network.node_num);
        }

        std::vector<double> fitness(population_size);
        for (size_t i = 0; i < population_size; ++i)
        {
            fitness[i] = network.CalculatePathLength(population[i]);
        }

        std::vector<std::vector<int>> new_population;
        while (new_population.size() < population_size)
        {
            std::uniform_int_distribution<int> int_dis(0, population_size - 1);
            int parent1_idx = int_dis(gen);
            int parent2_idx = int_dis(gen);

            if (gen() / static_cast<double>(gen.max()) < crossover_rate)
            {
                std::vector<int> child1, child2;
                // Crossover logic here
                // ...
                new_population.push_back(child1);
                new_population.push_back(child2);
            }
            else
            {
                new_population.push_back(population[parent1_idx]);
                new_population.push_back(population[parent2_idx]);
            }
        }

        for (auto &individual : new_population)
        {
            if (gen() / static_cast<double>(gen.max()) < mutation_rate)
            {
                BridgeMove(network, individual);
            }
        }

        population = new_population;
        counter++;
    }
}