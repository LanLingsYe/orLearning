#include "OptimalSolver.hpp"

void OptimalSolver::BridgeMove(const NetworkManager &network, const std::vector<int> original_path)
{
    std::uniform_real_distribution<double> dis(0.0, 1.0);
    size_t breakpoint_num = network.node_num / 20 + 1;
    std::vector<int> scrambled_piece;
    for (size_t iPiece = 0; iPiece < breakpoint_num; ++iPiece)
    {
        scrambled_piece.push_back(dis(gen));
    }
};

void OptimalSolver::VariableNeighborhoodSearch(const NetworkManager &network)
{

    size_t iter = 0, noniter = 0;
    std::vector<int> current_path = RandomPermutation(network.node_num);
    while (iter <= max_iteration && noniter <= mature_iteration)
    {
        iter++;
        noniter++;
        std::cout << "\n\t\t Algorithm VNS iterated  " << iter << "  times\n";
    }
};