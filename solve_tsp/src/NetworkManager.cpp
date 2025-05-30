#include "NetworkManager.hpp"

int NetworkManager::LoadData(std::filesystem::path data_path)
{

    std::ifstream fin(data_path);
    if (!fin.is_open())
    {
        std::cerr << "Error: Failed to open file " << data_path << std::endl;
        return -1;
    }

    std::string line;
    int id = 0;
    while (std::getline(fin, line))
    {
        std::istringstream iss(line);
        std::string name;
        double lon, lat;
        char comma;

        if (getline(iss, name, ',') && (iss >> lon >> comma >> lat))
        {
            NodeMap.emplace(id, std::make_pair(lon, lat));
            node_id.emplace_back(id);
        }
        id++;
    }
    node_num = node_id.size();
    return 0;
}

double NetworkManager::CalculateDistance(int former_id, int latter_id) const
{
    const auto &[x1, y1] = NodeMap.at(former_id);
    const auto &[x2, y2] = NodeMap.at(latter_id);
    return std::sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
};

double NetworkManager::CalculatePathLength(const std::vector<int> &path_sequence) const
{
    if (path_sequence.size() <= 1)
    {
        return 0.0;
    }

    double path_length = CalculateDistance(path_sequence[0], path_sequence[path_sequence.size() - 1]);
    for (size_t i = 0; i < path_sequence.size() - 1; ++i)
    {
        path_length += CalculateDistance(path_sequence[i], path_sequence[i + 1]);
    }

    return path_length;
};