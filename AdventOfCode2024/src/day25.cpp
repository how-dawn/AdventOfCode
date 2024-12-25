#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <set>

#include <cassert>

using Schematic = std::vector<std::string>;
const int AvailableSpace = 5;

std::vector<Schematic> readInput(const std::string &filePath) {
    if (!std::filesystem::exists(filePath)) {
        std::cerr << "File does not exist: " << filePath << std::endl;
        return {};
    }

    std::ifstream inputFile(filePath);
    if (!inputFile) {
        std::cerr << "Error opening file: " << filePath << std::endl;
        return {};
    }

    std::vector<Schematic> input;
    std::string line;
    Schematic schematic;
    while (std::getline(inputFile, line)) {
        if (line.empty()) {
            input.push_back(schematic);
            schematic.clear();
        } else {
            schematic.push_back(line);
        }
    }
    input.push_back(schematic);

    inputFile.close();

    return input;
}

std::map<int, std::vector<std::string>> preprocess(const std::vector<Schematic> &schematics) {
    auto func_ = [](const Schematic &schematic) {
        std::string res;
        size_t yMax = schematic.front().size();
        int group = -1;
        if ((schematic.front() == std::string(schematic.front().size(), '#'))) {
            group = 1;
        } else if ((schematic.back() == std::string(schematic.back().size(), '#'))) {
            group = 2;
        }
        for (size_t j = 0; j < yMax; j++) {
            int count = 0;
            bool flag = true;
            for (size_t i = 0; i < schematic.size(); i++) {
                if (schematic[i][j] == '#') {
                    count++;
                }
            }
            res += std::to_string(count - 1);
        }
        return std::make_pair(group, res);
    };

    std::map<int, std::vector<std::string>> res;
    for (const auto &schematic: schematics) {
        const auto [k, v] = func_(schematic);
        res[k].push_back(v);
    }

    assert(res.size() == 2);

    return res;
}

int cntUniqueKeys(const std::vector<std::string> &locks, const std::vector<std::string> &keys) {
    int res = 0;
    for (const auto &lock: locks) {
        for (const auto &key: keys) {
            assert(lock.size() == key.size());
            bool flag = true;
            for (auto i = 0; i < key.size(); i++) {
                if (lock[i] + key[i] - 2 * '0' > AvailableSpace) {
                    flag = false;
                    break;
                }
            }

            if (flag) {
                res++;
            }
        }
    }

    return res;
}

int solverP1(const std::vector<Schematic> &inputData) {
    const auto lockAndKeys = preprocess(inputData);
    const auto &locks = lockAndKeys.at(1);
    const auto &keys = lockAndKeys.at(2);

    return cntUniqueKeys(locks, keys);
}

int solverP2(const std::vector<Schematic> &inputData) {
    return 0;
}

int main(int argc, char *argv[]) {
    const std::string &filePath = argv[1];
    auto input = readInput(filePath);

    auto resultP1 = solverP1(input);
    std::cout << "P1 result: " << resultP1 << std::endl;

//    auto resultP2 = solverP2(input);
//    std::cout << "P2 result: " << resultP2 << std::endl;

    return 0;
}