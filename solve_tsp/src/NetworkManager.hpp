#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <filesystem>
#include <math.h>
#include <map>

class NetworkManager
{
public:
    size_t node_num = 0;
    std::vector<int> node_id = {};
    std::map<int, std::pair<double, double>> NodeMap;
    int LoadData(std::filesystem::path data_path);
    double CalculateDistance(int former_id, int latter_id) const;
    double CalculatePathLength(const std::vector<int> &path_sequence) const;
};