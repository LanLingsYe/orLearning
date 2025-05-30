#include "OptimalSolver.hpp"

std::vector<int> OptimalSolver::BridgeMove(const NetworkManager &network, const std::vector<int> &original_path)
{
    std::uniform_int_distribution<int> int_dis(1, 15);
    int breakpoint_num = network.node_num / 15;
    int position = 0;
    std::vector<int> scrambled_piece = {position};
    std::vector<int> novel_path;
    novel_path.reserve(network.node_num);
    for (int iPiece = 0; iPiece < breakpoint_num; ++iPiece)
    {
        position += int_dis(gen);
        scrambled_piece.emplace_back(position);
    }
    scrambled_piece.emplace_back(network.node_num);

    std::vector<int> piece_sequence = RandomPermutation(breakpoint_num + 1);
    for (auto &element : piece_sequence)
    {
        for (int iNode = scrambled_piece[element]; iNode < scrambled_piece[element + 1]; ++iNode)
        {
            novel_path.emplace_back(original_path[iNode]);
        }
    }
    return novel_path;
};

std::vector<int> OptimalSolver::Neighborhood_one(const NetworkManager &network, const std::vector<int> &original_path)
{
}

std::tuple<std::vector<int>, double> OptimalSolver::VariableNeighborhoodDescent(const NetworkManager &network, const std::vector<int> &path)
{
    std::vector<int> current_path = path;
    double current_cost = network.CalculatePathLength(current_path);
    size_t flag = 1;
    while (true)
    {
        switch (flag)
        {
        case 1:
            path = Neighborhood_one(const NetworkManager &network, const std::vector<int> &path);
            if (original_cost < current_cost)

                break;

        default:
            break;
        }
        ++flag;
    }
    return {created_path, network.CalculatePathLength(created_path)};
}

void OptimalSolver::VariableNeighborhoodSearch(const NetworkManager &network)
{

    size_t iter = 0, noniter = 0;
    std::vector<int> current_path = RandomPermutation(network.node_num);
    double current_cost = network.CalculatePathLength(current_path);
    double created_cost = 0;
    while (iter < max_iteration && noniter < mature_iteration)
    {
        iter++;
        noniter++;
        std::cout << "\n\t\t Algorithm VNS iterated  " << iter << "  times\n";
        std::vector<int> created_path = BridgeMove(network, current_path);
        [ created_path, created_cost ] = VariableNeighborhoodDescent(network, created_path);

        if (created_cost < current_cost)
        {
            current_path = created_path;
            current_cost = created_cost;
            noniter = 0;
        }
        std::cout << "\t\t the best solution = " << current_cost << std::endl;
    }
};