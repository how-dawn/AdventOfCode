#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <set>
#include <map>
#include <cassert>


using Direction = std::pair<int, int>;
using Position = std::pair<int, int>;
using PositionState = std::pair<Position, int>;

const std::vector<Direction> directions = {
        {0,  1},
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

std::map<Position, int> bfsP2(const std::vector<std::string> &map, const Position &curPos, const int maxStep) {
    std::map<Position, int> record;

    std::queue<PositionState> q;
    q.emplace(curPos, 0);
    std::set<Position> visited;
    visited.insert(curPos);

    while (!q.empty()) {
        const auto [pos, layer] = q.front();
        const auto &[x, y] = pos;
        q.pop();

        if (layer == maxStep && map[x][y] == '#') {
            continue;
        }

        if (map[x][y] != '#') {
            record[pos] = layer;
        }

        for (const auto &[dx, dy]: directions) {
            int newX = x + dx;
            int newY = y + dy;
            const Position newPos = {newX, newY};

            if (isInMap(map, {newX, newY}) && !visited.count(newPos)) {
                q.emplace(newPos, layer + 1);
                visited.insert(newPos);
            }
        }
    }

    return record;
}

int cheatDistance(const Position &p1, const Position &p2) {
    const auto &[x1, y1] = p1;
    const auto &[x2, y2] = p2;
    return std::abs(x1 - x2) + std::abs(y1 - y2);
}

int cheatP2(const std::vector<std::string> &map, const std::map<Position, int> &sPathDist, const int maxStep,
            const int threshold) {
    std::map<int, int> records;

    // This can be optimized, but I'm too lazy to change the existing code. (❍ᴥ❍ʋ)
    std::vector<PositionState> sPath(sPathDist.begin(), sPathDist.end());
    std::sort(sPath.begin(), sPath.end(), [](const PositionState &ps1, const PositionState &ps2) {
        return ps1.second < ps2.second;
    });

    for (int i = 0; i < static_cast<int>(sPath.size()); i++) {
        const auto &[pos1, dist1] = sPath[i];
        for (int j = i + 1; j < static_cast<int>(sPath.size()); j++) {
            const auto &[pos2, dist2] = sPath[j];

            const int cheatDist = cheatDistance(pos1, pos2);
            if (cheatDist > maxStep) { continue; };
            if (dist1 + cheatDist < dist2) {
                records[dist2 - dist1 - cheatDist]++;
            }
        }
    }

    int result = 0;
    for (const auto [k, v]: records) {
        if (k >= threshold) {
            result += v;
        }
    }

    return result;
}

int solverP2(const std::vector<std::string> &inputData) {
    const auto startPos = findInMap(inputData, 'S');
    const auto endPos = findInMap(inputData, 'E');

    return cheatP2(inputData, bfs(inputData, startPos, endPos), 20, 100);
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