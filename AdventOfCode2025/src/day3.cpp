#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>


std::vector<std::string> readInput(const std::string &filePath) {
    std::vector<std::string> list;

    if (!std::filesystem::exists(filePath)) {
        std::cerr << "File does not exist: " << filePath << std::endl;
        return list;
    }

    std::ifstream inputFile(filePath);
    if (!inputFile) {
        std::cerr << "Error opening file: " << filePath << std::endl;
        return list;
    }

    std::string line;

    while (std::getline(inputFile, line)) {
        list.push_back(line);
    }

    inputFile.close();
    return list;
}

int findMaximumJoltage(const std::string &bank) {
    int maxJoltage = 0;
    for (int i = 0; i < bank.length(); i++) {
        for (int j = i + 1; j < bank.length(); j++) {
            std::string val = std::string({bank[i]}) + std::string({bank[j]});
            maxJoltage = stoi(val) > maxJoltage ? stoi(val) : maxJoltage;
        }
    }

    return maxJoltage;
}

int solverP1(const std::vector<std::string> &iList) {
    int res = 0;

    for (const auto &bank: iList) {
        res += findMaximumJoltage(bank);
    }

    return res;
}

int64_t findMaximumJoltageP2(const std::string &bank) {
    std::string val;

    int last = -1;

    for (int i = 0; i < 12; i++) {
        char cur = '0';
        const int left = 12 - i;
        const auto bound = bank.size() - left;
        for (int j = last + 1; j <= bound; j++) {
            if (bank[j] > cur) {
                cur = bank[j];
                last = j;
            }
        }
        val += cur;
    }

    // std::cout << "[DEBUG] " << bank << " -> " << val << std::endl;
    return std::stoll(val);
}


int64_t solverP2(const std::vector<std::string> &iList) {
    int64_t res = 0;

    for (const auto &bank: iList) {
        res += findMaximumJoltageP2(bank);
    }

    return res;
}


int main(int argc, char *argv[]) {
    const std::string filePath = argv[1];

    const auto input = readInput(filePath);

    // const auto resultP1 = solverP1(input);
    // std::cout << "P1 result: " << resultP1 << std::endl;

    const auto resultP2 = solverP2(input);
    std::cout << "P1 result: " << resultP2 << std::endl;


    return 0;
}
