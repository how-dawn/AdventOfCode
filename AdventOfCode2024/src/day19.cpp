#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <map>
#include <ostream>
#include <string>
#include <vector>
#include <sstream>

using InputData = std::pair<std::vector<std::string>, std::vector<std::string>>;

std::vector<std::string> splitStrings(const std::string &s,
                                      const char delimiter) {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(s);
    while (std::getline(tokenStream, token, delimiter)) {
        token.erase(std::remove(token.begin(), token.end(), ' '), token.end());
        tokens.push_back(token);
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

    std::vector<std::string> patterns;
    std::vector<std::string> designs;
    std::string line;

    while (std::getline(inputFile, line)) {
        if (line.find(",") != line.npos) {
            auto strs = splitStrings(line, ',');
            patterns = std::move(strs);
        } else if (line.length() != 0) {
            designs.push_back(line);
        }
    }

    inputFile.close();

    return {patterns, designs};
}

bool dfsP1(const std::string &design, const std::vector<std::string> &patterns,
           std::map<std::string, bool> &memo) {
    if (design.empty()) {
        return true;
    }

    if (memo.count(design)) {
        return memo[design];
    }

    for (const auto p: patterns) {
        if (design.substr(0, p.size()) == p) {
            bool check = dfsP1(design.substr(p.size()), patterns, memo);
            memo[design] = check;

            if (check) {
                return true;
            }
        }
    }

    return false;
}

int solverP1(const InputData &inputData) {
    int result = 0;
    const auto &[patterns, designs] = inputData;
    std::map<std::string, bool> memo;

    for (const auto &d: designs) {
        if (dfsP1(d, patterns, memo)) {
            std::cout << "Found: " << d << std::endl;
            result++;
        }
    }
    return result;
}

int64_t dfsP2(const std::string &design,
              const std::vector<std::string> &patterns,
              std::map<std::string, int64_t> &memo) {
    if (design.empty()) {
        return 1;
    }

    if (memo.count(design)) {
        return memo[design];
    }

    int64_t total = 0;
    for (const auto p: patterns) {
        if (design.substr(0, p.size()) == p) {
            total += dfsP2(design.substr(p.size()), patterns, memo);
        }
    }
    memo[design] = total;

    return total;
}

int64_t solverP2(const InputData &inputData) {
    int64_t result = 0;
    const auto &[patterns, designs] = inputData;
    std::map<std::string, int64_t> memo;

    for (const auto &d: designs) {
        result += dfsP2(d, patterns, memo);
    }
    return result;
}

int main(int argc, char *argv[]) {
    std::string filePath = argv[1];

    auto input = readInput(filePath);
    // std::cout << "Input: " << input << std::endl;

    auto resultP1 = solverP1(input);
    std::cout << "P1 result: " << resultP1 << std::endl;

    auto resultP2 = solverP2(input);
    std::cout << "P2 result: " << resultP2 << std::endl;

    return 0;
}
