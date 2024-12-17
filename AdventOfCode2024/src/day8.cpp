
#include <filesystem>
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <set>
#include <vector>

using Coord = std::pair<int, int>;


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

std::map<char, std::set<Coord>> preprocessor(const std::vector<std::string> &inputData) {
    std::map<char, std::set<Coord>> inputMap;

    for (size_t i = 0; i < inputData.size(); i++) {
        for (size_t j = 0; j < inputData[i].size(); j++) {
            if (const auto c = inputData[i][j]; c != '.') {
                inputMap[c].insert({i, j});
            }
        }
    }

    return inputMap;
}


bool isInMap(const std::vector<std::string> &inputData, const Coord &coord) {
    const auto &[x, y] = coord;
    auto xSize = inputData.size();
    auto ySize = inputData[0].size();

    return x >= 0 && x < xSize && y >= 0 && y < ySize;
}

std::set<Coord>
findAntiNodesP1(const std::vector<std::string> &inputData, const char c, const std::set<Coord> &coords) {
    std::set<Coord> result;

    // pair the antennas
    auto cSize = static_cast<int>(coords.size());

    for (int i = 0; i < cSize; i++) {
        for (int j = i + 1; j < cSize; j++) {
            const auto &[cAX, cAY] = *std::next(coords.begin(), i);
            const auto &[cBX, cBY] = *std::next(coords.begin(), j);

            std::set<Coord> connectionSet;
            connectionSet.insert({cAX, cAY});
            connectionSet.insert({cBX, cBY});

            int dX1 = cAX - cBX, dY1 = cAY - cBY;
            auto aX = cAX + dX1, aY = cAY + dY1;
            while (isInMap(inputData, {aX, aY}) && inputData[aX][aY] == c) {
                connectionSet.insert({aX, aY});
                aX += dX1;
                aY += dY1;
            }

            int dX2 = cBX - cAX, dY2 = cBY - cAY;
            auto bX = cBX + dX2, bY = cBY + dY2;
            while (isInMap(inputData, {bX, bY}) && inputData[bX][bY] == c) {
                connectionSet.insert({bX, bY});

                bX += dX2;
                bY += dY2;
            }

            const auto &[firstAntennaX, firstAntennaY] = *connectionSet.begin();
            const auto &[lastAntennaX, lastAntennaY] = *std::prev(connectionSet.end(), 1);

            const int newAX = firstAntennaX + dX1, newAY = firstAntennaY + dY1;
            if (isInMap(inputData, {newAX, newAY}) && inputData[newAX][newAY] != c) {
                result.insert({newAX, newAY});
            }

            const int newBX = lastAntennaX + dX2, newBY = lastAntennaY + dY2;
            if (isInMap(inputData, {newBX, newBY}) && inputData[newBX][newBY] != c) {
                result.insert({newBX, newBY});
            }
        }
    }

    return result;
}


int solverP1(const std::vector<std::string> &inputData) {
    std::set<Coord> result;

    const auto inputMap = preprocessor(inputData);

    for (const auto &[c, cSet]: inputMap) {
        const auto &antiNodes = findAntiNodesP1(inputData, c, cSet);
        result.insert(antiNodes.begin(), antiNodes.end());
    }

    return static_cast<int>(result.size());
}


std::set<Coord>
findAntiNodesP2(const std::vector<std::string> &inputData, const char c, const std::set<Coord> &coords) {
    std::set<Coord> result;

    // pair the antennas
    auto cSize = static_cast<int>(coords.size());

    for (int i = 0; i < cSize; i++) {
        for (int j = i + 1; j < cSize; j++) {
            const auto &[cAX, cAY] = *std::next(coords.begin(), i);
            const auto &[cBX, cBY] = *std::next(coords.begin(), j);


            int dX1 = cAX - cBX, dY1 = cAY - cBY;
            auto aX = cAX + dX1, aY = cAY + dY1;
            while (isInMap(inputData, {aX, aY})) {
                if (inputData[aX][aY] != c) {
                    result.insert({aX, aY});
                }
                aX += dX1;
                aY += dY1;
            }

            int dX2 = cBX - cAX, dY2 = cBY - cAY;
            auto bX = cBX + dX2, bY = cBY + dY2;
            while (isInMap(inputData, {bX, bY})) {
                if (inputData[bX][bY] != c) {
                    result.insert({bX, bY});
                }
                bX += dX2;
                bY += dY2;
            }
        }
    }

    return result;
}

int solverP2(const std::vector<std::string> &inputData) {
    std::set<Coord> result;

    const auto inputMap = preprocessor(inputData);

    for (const auto &[c, cSet]: inputMap) {
        const auto &antiNodes = findAntiNodesP2(inputData, c, cSet);
        result.insert(cSet.begin(), cSet.end());
        result.insert(antiNodes.begin(), antiNodes.end());
    }

    return static_cast<int>(result.size());

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