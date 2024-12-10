#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <set>

using Coord = std::pair<int, int>;
using Direction = std::pair<int, int>;
using PathInfo = std::pair<Coord, Direction>;

const std::vector<Direction> directions = {{0,  1},
                                           {-1, 0},
                                           {1,  0},
                                           {0,  -1}};

std::vector<std::vector<int>> readInput(const std::string &filePath) {
    std::vector<std::vector<int>> input;

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
        std::vector<int> row;
        for (const auto v: line) {
            row.push_back(v - '0');
        }

        input.push_back(row);
    }

    inputFile.close();
    return input;
}

bool isInMap(const std::vector<std::vector<int>> &map, const Coord &coord) {
    int xSize = static_cast<int>(map.size());
    int ySize = static_cast<int>(map[0].size());

    const auto &[x, y] = coord;

    return x >= 0 && x < xSize && y >= 0 && y < ySize;
}

int bfs(const std::vector<std::vector<int>> &map, const Coord &initCoord) {
    int result = 0;

    std::queue<Coord> q;
    std::set<Coord> visited;
    q.push(initCoord);
    visited.insert(initCoord);

    while (!q.empty()) {
        const auto &[x, y] = q.front();
        q.pop();
        const int val = map[x][y];
        if (val == 9) {
            result++;
        }

        for (const auto &[dX, dY]: directions) {
            Coord newCoord = {x + dX, y + dY};
            if (!isInMap(map, newCoord)) {
                continue;
            }
            const int newVal = map[x + dX][y + dY];

            if (!visited.count(newCoord) && (newVal == val + 1)) {
                q.push(newCoord);
                visited.insert(newCoord);
            }
        }
    }

    return result;

}

int solverP1(const std::vector<std::vector<int>> &inputData) {
    int result = 0;
    for (auto i = 0; i < inputData.size(); i++) {
        for (auto j = 0; j < inputData.size(); j++) {
            const int val = inputData[i][j];
            if (val == 0) {
                result += bfs(inputData, {i, j});
            }
        }
    }
    return result;
}


int main(int argc, char *argv[]) {
    const std::string &filePath = argv[1];
    auto input = readInput(filePath);

    auto resultP1 = solverP1(input);
    std::cout << "P1 result: " << resultP1 << std::endl;


    return 0;
}
