#include "OptimalSolver.hpp"

void OptimalSolver::BridgeMove(const NetworkManager &network, std::vector<int> &path)
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

    std::vector<int> piece_sequence = RandomPermutation(breakpoint_num + 1);
    for (auto &element : piece_sequence)
    {
        for (int iNode = scrambled_piece[element]; iNode < scrambled_piece[element + 1]; ++iNode)
        {
            path.emplace_back(path[iNode]);
        }
    }
    return;
};

double OptimalSolver::Neighborhood_one(const NetworkManager &network, std::vector<int> &path)
{

    size_t counter = 0;
    std::vector<int> new_path = path;
    double cost = network.CalculatePathLength(path);
    while (counter <= mature_iteration)
    {
        ++counter;
        std::uniform_int_distribution<int> int_dis(0, network.node_num - 1);
        size_t first_index = int_dis(gen);
        size_t second_index = int_dis(gen);
        if (first_index < second_index)
        {
            std::reverse(new_path.begin() + first_index, new_path.begin() + second_index);
        }
        else if (first_index > second_index)
        {
            std::reverse(new_path.begin() + second_index, new_path.begin() + first_index);
        }
        else
        {
            continue;
        }

        double new_cost = network.CalculatePathLength(new_path);
        if (new_cost < cost)
        {
            path = new_path;
            cost = new_cost;
            counter = 0;
        }
    }
    return cost;
}

double OptimalSolver::Neighborhood_two(const NetworkManager &network, std::vector<int> &path)
{
}

double OptimalSolver::VariableNeighborhoodDescent(const NetworkManager &network, std::vector<int> &path)
{
    std::vector<int> best_path = path;
    double best_cost = network.CalculatePathLength(best_path);
    double cost;
    size_t flag = 1;
    size_t counter = 0;
    while (counter <= mature_iteration)
    {
        counter++;
        switch (flag)
        {
        case 1:
            cost = Neighborhood_one(network, path);
            if (cost < best_cost)
            {
                best_path = path;
                best_cost = cost;
                flag = 0;
                counter = 0;
            }
            break;
        case 2:
            cost = Neighborhood_two(network, path);
            if (cost < best_cost)
            {
                best_path = path;
                best_cost = cost;
                flag = 0;
                counter = 0;
            }
            break;

        default:
            break;
        }
        ++flag;
    }
    return best_cost;
}

void OptimalSolver::VariableNeighborhoodSearch(const NetworkManager &network)
{

    size_t iter = 0, noniter = 0;
    std::vector<int> best_path = RandomPermutation(network.node_num);
    std::vector<int> current_path = best_path;
    double best_cost = network.CalculatePathLength(best_path);
    double current_cost;
    while (iter < max_iteration && noniter < mature_iteration)
    {
        iter++;
        noniter++;
        std::cout << "\n\t\t Algorithm VNS iterated  " << iter << "  times\n";
        BridgeMove(network, current_path);
        current_cost = VariableNeighborhoodDescent(network, current_path);

        if (current_cost < best_cost)
        {
            best_path = current_path;
            best_cost = current_cost;
            noniter = 0;
        }
        std::cout << "\t\t the best solution = " << current_cost << std::endl;
    }
};