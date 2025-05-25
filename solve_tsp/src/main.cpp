#include <iostream>
#include <string>
#include <sstream>
#include <math.h>

#include "NetworkManager.hpp"
#include "OptimalSolver.hpp"

int main()
{

    // 获取文件路径
    std::filesystem::path txt_path = std::filesystem::path(__FILE__).parent_path() / "../data/china31.txt";
    NetworkManager network;
    network.LoadData(txt_path);

    // for (auto it = network.NodeMap.begin(); it != network.NodeMap.end(); ++it)
    // {
    //     std::cout << "id: " << it->first << ", coordinates: (" << it->second.first << ", " << it->second.second << ")\n";
    // }
    auto solver = OptimalSolver();
    solver.solve(network, "SimulatedAnnealing");
    return 0;
}