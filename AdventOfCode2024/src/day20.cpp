#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <set>
#include <map>
#include <climits>
#include <cassert>


using Direction = std::pair<int, int>;
using Position = std::pair<int, int>;

const std::vector<Direction> directions = {
        {0,  1},
        {-1, 0},
        {1,  0},
        {0,  -1}};

const std::vector<Direction> cheatDirections = {
        {0,  2},
        {-2, 0},
        {2,  0},
        {0,  -2}};


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

Position findInMap(const std::vector<std::string> &map, const char &c) {
    for (int i = static_cast<int>(map.size() - 1); i >= 0; i--) {
        for (int j = 0; j < static_cast<int>(map[i].size()); j++) {
            if (map[i][j] == c)
                return {i, j};
        }
    }
    return {-1, -1};
}


bool isInMap(const std::vector<std::string> &map, const Position &coord) {
    int xSize = static_cast<int>(map.size());
    int ySize = static_cast<int>(map[0].size());

    const auto &[x, y] = coord;

    return x >= 0 && x < xSize && y >= 0 && y < ySize;
}

std::map<Position, int> bfs(const std::vector<std::string> &map, const Position &startPos, const Position &endPos) {
    std::queue<Position> q;
    q.push(startPos);

    std::map<Position, int> dist;
    dist[startPos] = 1;

    while (!q.empty()) {
        const auto pos = q.front();
        q.pop();

        const auto &[x, y] = pos;

        if (pos == endPos) {
            break;
        }
        for (const auto &[dx, dy]: directions) {
            const auto newX = x + dx, newY = y + dy;
            const Position newPos = {newX, newY};

            if (isInMap(map, newPos) && !dist.count(newPos) && map[newX][newY] != '#') {
                q.push(newPos);
                dist[newPos] = dist[pos] + 1;
            }

        }
    }

    return dist;
}

int cheat(const std::vector<std::string> &map, const std::map<Position, int> &sPathDist) {
    std::map<int, int> records;

    for (const auto &[pos, dist]: sPathDist) {
        const auto &[x, y] = pos;
        for (const auto &[dx, dy]: directions) {
            int newX = x + dx;
            int newY = y + dy;

            if (map[newX][newY] == '#') {
                const Position cheatPos = {newX + dx, newY + dy};
                if (sPathDist.count(cheatPos) && dist + 2 < sPathDist.at(cheatPos)) {
                    int saveLen = sPathDist.at(cheatPos) - dist - 2;
                    records[saveLen]++;
                }
            }
        }
    }

    int result = 0;
    for (const auto [k, v]: records) {
        if (k >= 100) {
            result += v;
        }
    }

    return result;
}

int solverP1(const std::vector<std::string> &inputData) {
    const auto startPos = findInMap(inputData, 'S');
    const auto endPos = findInMap(inputData, 'E');

    return cheat(inputData, bfs(inputData, startPos, endPos));
}

//int solverP2(const std::vector<std::string> &inputData) {
//    return 0;
//}

int main(int argc, char *argv[]) {
    const std::string &filePath = argv[1];
    auto input = readInput(filePath);

    auto resultP1 = solverP1(input);
    std::cout << "P1 result: " << resultP1 << std::endl;

//    auto resultP2 = solverP2(input);
//    std::cout << "P2 result: " << resultP2 << std::endl;

    return 0;
}