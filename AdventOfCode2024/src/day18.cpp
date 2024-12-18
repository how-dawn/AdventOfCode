#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <queue>
#include <set>
#include <string>
#include <vector>
#include <sstream>

using Position = std::pair<int, int>;
using Direction = std::pair<int, int>;

const std::vector<Direction> directions = {{0,  1},
                                           {-1, 0},
                                           {1,  0},
                                           {0,  -1}};

std::vector<int> splitIntegers(const std::string &s, const char delimiter) {
    std::vector<int> tokens;
    std::string token;
    std::istringstream tokenStream(s);
    while (std::getline(tokenStream, token, delimiter)) {
        tokens.push_back(std::stoi(token));
    }
    return tokens;
}

std::vector<Position> readInput(const std::string &filePath) {
    std::vector<Position> positions;

    if (!std::filesystem::exists(filePath)) {
        std::cerr << "File does not exist: " << filePath << std::endl;
        return {};
    }

    std::ifstream inputFile(filePath);
    if (!inputFile) {
        std::cerr << "Error opening file: " << filePath << std::endl;
        return {};
    }

    std::string line;
    while (std::getline(inputFile, line)) {
        auto nums = splitIntegers(line, ',');
        positions.emplace_back(nums[0], nums[1]);
    }

    inputFile.close();

    return positions;
}

int bfs(const std::set<Position> &positions, const Position &startPos,
        const Position &endPos) {
    using PosData = std::pair<Position, int>;
    const auto &[dstX, dstY] = endPos;

    std::queue<PosData> q;
    q.emplace(startPos, 0);
    std::set<Position> visited;
    visited.insert(startPos);

    while (!q.empty()) {
        auto posData = q.front();
        q.pop();
        const auto &[pos, len] = posData;
        const auto &[x, y] = pos;

        if (pos == endPos) {
            return len;
        }

        for (const auto &[dx, dy]: directions) {
            int newX = x + dx, newY = y + dy;
            const bool isInMap =
                    newX >= 0 && newX <= dstX && newY >= 0 && newY <= dstY;

            const bool isBlocked = positions.count({newX, newY});
            const bool isVisited = visited.count({newX, newY});

            if (isInMap && !isBlocked && !isVisited) {
                q.push({{newX, newY}, len + 1});
                visited.insert({newX, newY});
            }
        }
    }
    return -1;
}

int solverP1(const std::vector<Position> &positions, const int blocks,
             const Position &startPos, const Position &endPos) {
    std::set<Position> blocked(positions.begin(), positions.begin() + blocks);
    return bfs(blocked, startPos, endPos);
}

Position solverP2(const std::vector<Position> &positions,
                  const Position &startPos, const Position &endPos) {
    for (auto i = 1; i < positions.size(); i++) {
        std::set<Position> blocked(positions.begin(), positions.begin() + i);
        if (bfs(blocked, startPos, endPos) == -1) {
            return positions[i - 1];
        }
    }
    return {-1, -1};
}

int main(int argc, char *argv[]) {
    std::string filePath = argv[1];

    auto input = readInput(filePath);

    // test
    auto testResultP1 = solverP1(input, 12, {0, 0}, {6, 6});
    std::cout << "Test P1 result: " << testResultP1 << std::endl;
    auto testResultP2 = solverP2(input, {0, 0}, {6, 6});
    const auto &[xTest, yTest] = testResultP2;
    std::cout << "Test P2 result: " << xTest << "," << yTest << std::endl;


    // question
    auto resultP1 = solverP1(input, 1024, {0, 0}, {70, 70});
    std::cout << "P1 result: " << resultP1 << std::endl;
    auto resultP2 = solverP2(input, {0, 0}, {70, 70});
    const auto &[x, y] = resultP2;
    std::cout << "P2 result: " << x << "," << y << std::endl;


    return 0;
}