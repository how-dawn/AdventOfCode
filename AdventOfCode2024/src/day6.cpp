#include <filesystem>
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <utility>
#include <set>
#include <vector>

using Coord = std::pair<int, int>;
using Direction = std::pair<int, int>;
using PathInfo = std::pair<Coord, Direction>;

const Direction startDirection = {-1, 0};
const std::map<Direction, Direction> directMapping = {
        {{0,  1},  {1,  0}},
        {{1,  0},  {0,  -1}},
        {{0,  -1}, {-1, 0}},
        {{-1, 0},  {0,  1}}};

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

Coord findStartCoord(const std::vector<std::string> &map) {
    for (int i = 0; i < map.size(); i++) {
        for (int j = 0; j < map[i].size(); j++) {
            if (map[i][j] == '^') {
                return {i, j};
            }
        }
    }

    return {-1, -1};
}

void dfsP1(const std::vector<std::string> &map, const Coord &coord,
           const Direction &direction, std::set<Coord> &visited,
           std::vector<PathInfo> &path) {

    if (!isInMap(map, coord)) {
        return;
    }

    const auto &[x, y] = coord;
    const auto &[dx, dy] = direction;
    const auto &curSign = map[x][y];

    if (curSign == '#') {
        const auto &newDirection = directMapping.at(direction);
        const auto &[newDX, newDY] = newDirection;
        const Coord newCoord = {x - dx + newDX, y - dy + newDY};
        dfsP1(map, newCoord, newDirection, visited, path);
    } else {
        path.emplace_back(coord, direction);
        visited.insert(coord);
        const Coord newCoord = {x + dx, y + dy};
        dfsP1(map, newCoord, direction, visited, path);
    }
}

int solverP1(const std::vector<std::string> &map) {
    const auto &startCoord = findStartCoord(map);
    std::vector<PathInfo> path;
    std::set<Coord> visited;
    dfsP1(map, startCoord, startDirection, visited, path);

    return static_cast<int>(visited.size());
}

bool dfsP2(const std::vector<std::string> &map, std::set<PathInfo> &visited, const Coord &coord,
           const Direction &direction) {

    if (!isInMap(map, coord)) {
        return false;
    }

    if (visited.count({coord, direction})) {
        return true;
    }

    const auto &[x, y] = coord;
    const auto &[dx, dy] = direction;
    const char &curSign = map[x][y];

    if (curSign == '#') {
        const auto &newDirection = directMapping.at(direction);
        const auto &[newDX, newDY] = newDirection;
        const Coord newCoord = {x - dx + newDX, y - dy + newDY};
        return dfsP2(map, visited, newCoord, newDirection);
    } else {
        visited.insert({coord, direction});
        const Coord newCoord = {x + dx, y + dy};
        return dfsP2(map, visited, newCoord, direction);
    }
}

int solverP2(std::vector<std::string> &map) {
    const auto &startCoord = findStartCoord(map);
    std::vector<PathInfo> path;
    std::set<Coord> visited;
    dfsP1(map, startCoord, {-1, 0}, visited, path);

    std::set<Coord> blocks;

    for (const auto &[coord, direction]: path) {
        const auto &[x, y] = coord;
        const auto &[dx, dy] = direction;

        if (coord == startCoord) continue;

        const auto v = map[x][y];
        map[x][y] = '#';
        {
            std::set<PathInfo> visitedP2;
            if (dfsP2(map, visitedP2, startCoord, startDirection)) {
                blocks.insert({x, y});
            }
        }
        map[x][y] = v;
    }

    return static_cast<int>(blocks.size());
}

int main(int argc, char *argv[]) {
    std::string filePath = argv[1];

    auto input = readInput(filePath);

    int resultP1 = solverP1(input);
    std::cout << "P1 result: " << resultP1 << std::endl;

    int resultP2 = solverP2(input);
    std::cout << "P2 result: " << resultP2 << std::endl;

    return 0;
}
