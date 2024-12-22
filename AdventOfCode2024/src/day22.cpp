#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <functional>
#include <numeric>


std::vector<uint64_t> readInput(const std::string &filePath) {
    if (!std::filesystem::exists(filePath)) {
        std::cerr << "File does not exist: " << filePath << std::endl;
        return {};
    }

    std::ifstream inputFile(filePath);
    if (!inputFile) {
        std::cerr << "Error opening file: " << filePath << std::endl;
        return {};
    }

    std::vector<uint64_t> input;
    std::string line;
    while (std::getline(inputFile, line)) {
        input.push_back(std::stoll(line));
    }

    inputFile.close();

    return input;
}

uint64_t mix(const uint64_t &secret_number, const uint64_t &num) {
    return secret_number ^ num;
}

uint64_t prune(const uint64_t &secret_number) {
    return secret_number % 16777216;
}

uint64_t process(const uint64_t &secret_number) {
    auto result = mix(secret_number, secret_number * 64);
    result = prune(result);

    result = mix(result, result / 32);
    result = prune(result);

    result = prune(mix(result, result * 2048));

    return result;
}

uint64_t solverP1(const std::vector<uint64_t> &inputData, const int steps) {
    std::vector<uint64_t> result;
    for (const auto &val_: inputData) {
        auto val = val_;
        for (int i = 0; i < steps; i++) {
            val = process(val);
        }
        result.push_back(val);
    }

    return std::accumulate(result.begin(), result.end(), 0ULL);
}



//uint64_t solverP2(const std::vector<uint64_t> &inputData) {
//    return 0;
//}

int main(int argc, char *argv[]) {
    const std::string &filePath = argv[1];
    auto input = readInput(filePath);

    auto resultP1 = solverP1(input, 2000);
    std::cout << "P1 result: " << resultP1 << std::endl;

//    auto resultP2 = solverP2(input);
//    std::cout << "P2 result: " << resultP2 << std::endl;

    return 0;
}