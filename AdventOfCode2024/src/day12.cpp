#include <filesystem>
#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <vector>

using Coord = std::pair<int, int>;
using Direction = std::pair<int, int>;
using GeoData = std::pair<int, int>;

const std::vector<Direction> directions = {{0,  1},
                                           {-1, 0},
                                           {1,  0},
                                           {0,  -1}};

std::vector<std::string> readInput(const std::string &filePath) {
    if (!std::filesystem::exists(filePath)) {
        std::cerr << "File does not exist: " << filePath << std::endl;
        return {};
    }

    std::ifstream inputFile(filePath);
    if (!inputFile) {
        std::cerr << "Error opening file: " << filePath << std::endl;
        return {};
    }

    std::vector<std::string> input;
    std::string line;
    while (std::getline(inputFile, line)) {
        input.push_back(line);
    }

    inputFile.close();

    return input;
}

bool isInMap(const std::vector<std::string> &map, const Coord &coord) {
    int xSize = static_cast<int>(map.size());
    int ySize = static_cast<int>(map[0].size());

    const auto &[x, y] = coord;

    return x >= 0 && x < xSize && y >= 0 && y < ySize;
}

void DFS(const std::vector<std::string> &map, const Coord &coord,
         std::set<Coord> &region, std::set<Coord> &visited) {

    visited.insert(coord);
    region.insert(coord);

    const auto &[x, y] = coord;

    for (const auto &[dx, dy]: directions) {
        int newX = x + dx, newY = y + dy;
        if (isInMap(map, {newX, newY})) {
            if (!visited.count({newX, newY}) && map[x][y] == map[newX][newY]) {
                DFS(map, {newX, newY}, region, visited);
            }
        }
    }
}

GeoData analyseRegion(const std::vector<std::string> &map,
                      const std::set<Coord> &region) {
    int area = 0;
    int perimeter = 0;

    for (const auto &[x, y]: region) {
        area++;
        for (const auto &[dx, dy]: directions) {
            int newX = x + dx, newY = y + dy;
            if (isInMap(map, {newX, newY})) {
                if (!region.count({newX, newY})) {
                    perimeter++;
                }
            } else {
                perimeter++;
            }
        }
    }

    return {area, perimeter};
}

int solverP1(const std::vector<std::string> &map) {
    int result = 0;

    std::set<Coord> visited;
    std::vector<std::set<Coord>> regions;
    std::vector<GeoData> regionsInfo;

    for (auto i = 0; i < map.size(); i++) {
        for (auto j = 0; j < map[i].size(); j++) {
            if (!visited.count({i, j})) {
                std::set<Coord> region;
                DFS(map, {i, j}, region, visited);
                regions.push_back(region);
                regionsInfo.push_back(analyseRegion(map, region));
            }
        }
    }

    for (const auto &[area, perimeter]: regionsInfo) {
        result += (area * perimeter);
    }

    return result;
}

GeoData analyseRegionP2(const std::vector<std::string> &map,
                        const std::set<Coord> &region) {
    int area = 0;
    int fenceSize = 0;
    using FenceInfo = std::pair<Direction, int>; // {check direction, end coord(x or y)}

    // set: y or x.
    // In the sets, the number different values means the number of sides
    std::map<FenceInfo, std::set<int>> fenceInfoMap;

    for (const auto &[x, y]: region) {
        area++;
        for (const auto &direction: directions) {
            const auto &[dx, dy] = direction;
            int newX = x + dx, newY = y + dy;

            int k = dx != 0 ? newX : newY;
            int v = dx != 0 ? y : x;

            if (isInMap(map, {newX, newY})) {
                if (!region.count({newX, newY})) {
                    fenceInfoMap[{direction, k}].insert(v);
                }
            } else {
                fenceInfoMap[{direction, k}].insert(v);
            }
        }
    }

    for (const auto &[fInfo, s]: fenceInfoMap) {
        int diff = 1;
        for (int i = 1; i < s.size(); i++) {
            int currentV = *std::next(s.begin(), i);
            int lastV = *std::next(s.begin(), i - 1);
            if (std::abs(currentV - lastV) > 1) {
                diff++;
            }

        }
        fenceSize += diff;
    }

    return {area, fenceSize};
}

int solverP2(const std::vector<std::string> &map) {
    int result = 0;

    std::set<Coord> visited;
    std::vector<std::set<Coord>> regions;
    std::vector<GeoData> regionsInfo;

    for (auto i = 0; i < map.size(); i++) {
        for (auto j = 0; j < map[i].size(); j++) {
            if (!visited.count({i, j})) {
                std::set<Coord> region;
                DFS(map, {i, j}, region, visited);
                regions.push_back(region);
                regionsInfo.push_back(analyseRegionP2(map, region));
            }
        }
    }

    for (const auto &[area, value]: regionsInfo) {
        result += (area * value);
    }

    return result;
}

int main(int argc, char *argv[]) {
    const std::string &filePath = argv[1];
    auto input = readInput(filePath);

    auto resultP1 = solverP1(input);
    std::cout << "P1 result: " << resultP1 << std::endl;

    auto resultP2 = solverP2(input);
    std::cout << "P2 result: " << resultP2 << std::endl;

    return 0;
}