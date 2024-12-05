#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>

using OrderRule = std::pair<int, int>;

struct InputData {
    std::vector<OrderRule> rules;
    std::vector<std::vector<int>> updates;
};

std::vector<int> splitIntegers(const std::string &s, const char delimiter) {
    std::vector<int> tokens;
    std::string token;
    std::istringstream tokenStream(s);
    while (std::getline(tokenStream, token, delimiter)) {
        tokens.push_back(std::stoi(token));
    }
    return tokens;
}

InputData readInput(const std::string &filePath) {
    InputData inputData;

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
        if (line.find("|") != line.npos) {
            auto orders = splitIntegers(line, '|');
            inputData.rules.push_back({orders[0], orders[1]});
        } else if (line.find(",") != line.npos) {
            auto update = splitIntegers(line, ',');
            inputData.updates.push_back(update);
        }
    }

    inputFile.close();

    return inputData;
}

int solverP1(const InputData &inputData) {
    int result = 0;

    const auto &rules = inputData.rules;
    const auto &updates = inputData.updates;

    for (const auto &update : updates) {
        int flag = true;
        for (const auto &[a, b] : rules) {
            auto itA = std::find(update.begin(), update.end(), a);
            auto itB = std::find(update.begin(), update.end(), b);
            if (itA != update.end() && itB != update.end()) {
                // std::cout << a << ", " << b << std::endl;
                int dis = std::distance(itA, itB);
                if (dis < 0) {
                    flag = false;
                    break;
                }
            }
        }

        if (flag) {
            result += update[update.size() / 2];
        }
    }

    return result;
}

std::vector<int> reorder(std::vector<int> update,
                         std::vector<OrderRule> rules) {
    std::vector<int> ret(update.begin(), update.end());

    for (int j = 0; j < ret.size() - 1; ++j) {
        for (int i = 0; i < ret.size() - j - 1; ++i) {
            int x = ret[i], y = ret[i + 1];
            for (const auto &rule : rules) {
                const auto &[a, b] = rule;
                if (y == a && x == b) {
                    std::swap(ret[i], ret[i + 1]);
                    // for (const auto v : ret) {
                    //   std::cout << v << " ";
                    // }
                    // std::cout << std::endl;
                }
            }
        }
    }
    return ret;
}

int solverP2(const InputData &inputData) {
    int result = 0;

    std::vector<std::vector<int>> falseUpdate;

    const auto &rules = inputData.rules;
    const auto &updates = inputData.updates;

    for (const auto &update : updates) {
        int flag = true;
        for (const auto &[a, b] : rules) {
            auto itA = std::find(update.begin(), update.end(), a);
            auto itB = std::find(update.begin(), update.end(), b);
            if (itA != update.end() && itB != update.end()) {
                int dis = std::distance(itA, itB);
                if (dis < 0) {
                    flag = false;
                    break;
                }
            }
        }

        if (!flag) {
            result += reorder(update, rules)[update.size() / 2];
        }
    }

    return result;
}

int main(int argc, char *argv[]) {
    std::string filePath = argv[1];

    auto input = readInput(filePath);
    // std::cout << "Input: " << input << std::endl;

    int resultP1 = solverP1(input);
    std::cout << "P1 result: " << resultP1 << std::endl;

    int resultP2 = solverP2(input);
    std::cout << "P2 result: " << resultP2 << std::endl;

    return 0;
}
