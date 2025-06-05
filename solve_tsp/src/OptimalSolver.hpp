#pragma once
#include "NetworkManager.hpp"
#include <algorithm>
#include <random>
#include <vector>
#include <chrono>

class OptimalSolver
{
private:
    /// The tolerance represents a threshold or acceptable margin of error
    /// for computations performed by the OptimalSolver class.
    /// It is used to control precision or convergence criteria in the solver's operations.
    double tolerance;
    /// Maximum number of iterations allowed for the solver to run.
    /// This parameter limits how many times the optimization loop will execute
    /// before terminating, regardless of whether an optimal solution has been found.
    size_t max_iteration;
    /// This parameter is used to determine when the algorithm has stabilized
    size_t mature_iteration;

public:
    OptimalSolver();
    static std::vector<int> RandomPermutation(int node_num);
    int solve(const NetworkManager &network, std::string method_solve);

    void SimulatedAnnealing(const NetworkManager &network);

    void GeneticAlgorithm(const NetworkManager &network);
};