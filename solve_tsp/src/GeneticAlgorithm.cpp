#include "OptimalSolver.hpp"

/// The gen is a static member of the OptimalSolver class
/// that utilizes the std::mt19937_64 engine to generate high-quality 64-bit pseudo-random numbers,
/// suitable for applications like stochastic algorithms, simulations, and reproducible random number sequences.
static std::mt19937_64 gen{std::random_device{}()};

size_t population_size = 100;
double mutation_rate = 0.15;
double cross_over_rate = 0.9;
size_t cross_over_num = 2;
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

/// The CrossOver function performs crossover on the population
/// by selecting a specified number of parents and generating offspring.
void CrossOver(const NetworkManager &network)
{
    size_t parent_num = population_size * cross_over_rate;
    if (parent_num % 2 != 0)
    {
        parent_num -= 1; // Ensure even number of parents for crossover
    }
    std::vector<int> parents(PopulationId.begin(), PopulationId.begin() + parent_num);
    std::map<int, std::vector<double>> offspring_gene(PopulationGene);
    for (size_t i = 0; i < parent_num; i += 2)
    {
        for (size_t j = 0; j < cross_over_num; ++j)
        {
            size_t position1 = std::uniform_int_distribution<size_t>(0, network.node_num - 1)(gen);
            size_t position2 = std::uniform_int_distribution<size_t>(0, network.node_num - 1)(gen);
            std::swap(offspring_gene[parents[i]][position1], offspring_gene[parents[i + 1]][position2]);
        }
    }

    size_t current_population_size = PopulationId.size();
    for (size_t i = 0; i < offspring_gene.size(); ++i)
    {
        PopulationId.emplace_back(current_population_size + i);
        PopulationGene.emplace(current_population_size + i, offspring_gene[i]);
        FitnessMap.emplace(current_population_size + i, network.CalculatePathLength(Decoder(offspring_gene[i])));
    }
    UpdateSequence();
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

/// The Migrate function adds new individuals to the population
/// based on the specified mature degree, which determines how many new individuals to add.
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

/// The Compete function reduces the population size to the specified population_size
/// by removing individuals with the worst fitness values.
void Compete()
{
    if (PopulationId.size() <= population_size)
    {
        return;
    }

    for (size_t i = population_size; i < PopulationId.size(); ++i)
    {
        PopulationGene.erase(PopulationId[i]);
        FitnessMap.erase(PopulationId[i]);
    }
    PopulationId.erase(PopulationId.begin() + population_size, PopulationId.end());

    std::map<int, std::vector<double>> PopulationGene_;
    std::map<int, double> FitnessMap_;
    std::vector<int> indices(PopulationId.size());
    std::iota(indices.begin(), indices.end(), 0);
    std::sort(indices.begin(), indices.end(), [](int i, int j)
              { return PopulationId[i] < PopulationId[j]; });
    std::vector<int> ranks(PopulationId.size());
    for (int i = 0; i < indices.size(); ++i)
    {
        ranks[indices[i]] = i;
    }

    for (int i = 0; i < population_size; ++i)
    {
        PopulationGene_[PopulationId[i]] = PopulationGene[PopulationId[indices[i]]];
        FitnessMap_[PopulationId[i]] = FitnessMap[PopulationId[indices[i]]];
    }
    PopulationGene = std::move(PopulationGene_);
    FitnessMap = std::move(FitnessMap_);
    PopulationId = std::move(ranks);
}

void OptimalSolver::GeneticAlgorithm(const NetworkManager &network)
{
    size_t iter = 0, non_iter = 0;
    InitPack(network);
    double best_fitness = FitnessMap[PopulationId[0]];
    while (iter <= max_iteration && non_iter <= mature_iteration)
    {
        ++iter;
        CrossOver(network);
        Mutate(network);
        Migrate(network, iter / max_iteration);
        Compete();

        if (FitnessMap[PopulationId[0]] < best_fitness)
        {
            best_fitness = FitnessMap[PopulationId[0]];
            non_iter = 0; // Reset non-iteration counter if a better solution is found
        }
        else
        {
            ++non_iter; // Increment non-iteration counter if no improvement
        }
        if (iter % 10 == 0)
        {
            std::cout << "Iteration: " << iter << ", Best Fitness: " << best_fitness << std::endl;
        }
    }
}