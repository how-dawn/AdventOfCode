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
            maxJoltage = stoi(val) > maxJoltage? stoi(val): maxJoltage;
        }
    }

    return maxJoltage;
}

int solverP1(const std::vector<std::string> &iList) {
    int res = 0;

    for (const auto& bank : iList) {
        res += findMaximumJoltage(bank);
    }

    return res;
}


int main(int argc, char *argv[]) {
    const std::string filePath = argv[1];

    const auto input = readInput(filePath);

    const int resultP1 = solverP1(input);
    std::cout << "P1 result: " << resultP1 << std::endl;


    return 0;
}
