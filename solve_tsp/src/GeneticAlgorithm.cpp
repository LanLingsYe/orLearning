#include "OptimalSolver.hpp"

/// The gen is a static member of the OptimalSolver class
/// that utilizes the std::mt19937_64 engine to generate high-quality 64-bit pseudo-random numbers,
/// suitable for applications like stochastic algorithms, simulations, and reproducible random number sequences.
static std::mt19937_64 gen{std::random_device{}()};

size_t population_size = 100;
double mutation_rate = 0.15;
double crossover_rate = 0.9;
std::vector<int> PopulationId;
std::map<int, std::vector<double>> PopulationGene;
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

/// The UpdateSequence function updates the PopulationId array
/// by sorting elements from the FitnessMap based on their fitness values.
/// It ensures that the PopulationId reflects the order of individuals
/// from the best to the worst fitness, allowing for selection in genetic algorithms.
void UpdateSequence()
{
    std::vector<std::pair<int, double>> tmp_vec(FitnessMap.begin(), FitnessMap.end());
    std::sort(tmp_vec.begin(), tmp_vec.end(),
              [](const auto &a, const auto &b)
              { return a.second < b.second; });
    for (size_t i = 0; i < population_size; ++i)
    {
        PopulationId[i] = tmp_vec[i].first;
    }
}

/// The GenerateGene function creates a random gene of specified size.
/// It fills the gene with random double values between 0.0 and 1.0,
std::vector<double> GenerateGene(int gene_size)
{
    std::vector<double> gene;
    gene.reserve(gene_size);
    std::uniform_real_distribution<double> dis(0.0, 1.0);
    for (int i = 0; i < gene_size; ++i)
    {
        gene.emplace_back(dis(gen));
    }
    return gene;
}

/// The Decoder function represents a permutation of indices
/// based on the ascending order of the input values.
std::vector<int> Decoder(const std::vector<double> &gene)
{
    std::vector<int> path;
    path.reserve(gene.size());
    std::vector<int> indices(gene.size());
    std::iota(indices.begin(), indices.end(), 0);
    std::sort(indices.begin(), indices.end(),
              [&gene](int i, int j)
              { return gene[i] < gene[j]; });
    for (int i = 0; i < indices.size(); ++i)
    {
        path[indices[i]] = i;
    }
    return path;
}

/// The UpdateFitness function calculates the fitness of each individual in the population
/// by evaluating the path length of the decoded gene.
void UpdateFitness(const NetworkManager &network)
{
    for (size_t i = 0; i < population_size; ++i)
    {
        FitnessMap[PopulationId[i]] = network.CalculatePathLength(Decoder(PopulationGene[PopulationId[i]]));
    }
}

/// The InitPack function initializes the population for the genetic algorithm.
void InitPack(const NetworkManager &network)
{
    for (size_t i = PopulationId.size(); i < PopulationId.size() + population_size; ++i)
    {
        PopulationId.emplace_back(i);
        PopulationGene.emplace(i, GenerateGene(network.node_num));
        FitnessMap.emplace(i, network.CalculatePathLength(Decoder(PopulationGene[i])));
    }
    UpdateSequence();
}

void Mutate(const NetworkManager &network)
{
    size_t mutate_num = network.node_num / 50 + 1;
    std::uniform_real_distribution<double> dis(0.0, 1.0);
    std::uniform_int_distribution<int> int_dis(0, network.node_num - 1);
    for (size_t i = 0; i < population_size; ++i)
    {
        if (dis(gen) <= mutation_rate)
        {
            for (size_t j = 0; j < mutate_num; ++j)
            {
                PopulationGene[PopulationId[i]][int_dis(gen)] = dis(gen);
            }
        }
        else
        {
            continue;
        }
    }
    UpdateFitness(network);
    UpdateSequence();
}

void Migrate(const NetworkManager &network, size_t mature_degree)
{
    size_t migrate_num = mature_degree * population_size;
    for (size_t i = PopulationId.size(); i < PopulationId.size() + migrate_num; ++i)
    {
        PopulationId.emplace_back(i);
        PopulationGene.emplace(i, GenerateGene(network.node_num));
        FitnessMap.emplace(i, network.CalculatePathLength(Decoder(PopulationGene[i])));
    }
    UpdateSequence();
}

void Compete()
{
    if (PopulationId.size() <= population_size)
    {
        return;
    }
}

void OptimalSolver::GeneticAlgorithm(const NetworkManager &network)
{
    size_t iter = 0, non_iter = 0;
    InitPack(network);
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