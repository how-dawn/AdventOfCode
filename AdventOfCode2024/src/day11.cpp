#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <map>

std::vector<long long> readInput(const std::string &filePath) {
    if (!std::filesystem::exists(filePath)) {
        std::cerr << "File does not exist: " << filePath << std::endl;
        return {};
    }

    std::ifstream inputFile(filePath);
    if (!inputFile) {
        std::cerr << "Error opening file: " << filePath << std::endl;
        return {};
    }

    std::vector<long long> input;
    std::string line;
    while (std::getline(inputFile, line)) {
        std::istringstream lineStream(line);
        int num;

        // Extract integers from the line
        while (lineStream >> num) {
            input.push_back(num);
        }
    }

    return input;
}

std::vector<long long> blink(const std::vector<long long> &inputData) {
    std::vector<long long> result;
    for (const auto &v: inputData) {
        const auto strV = std::to_string(v);

        if (v == 0) {
            result.push_back(1);
        } else if (strV.length() % 2 == 0) {
            const auto left = strV.substr(0, strV.size() / 2);
            const auto right = strV.substr(strV.size() / 2);

            result.push_back(std::stoll(left));
            result.push_back(std::stoll(right));
        } else {
            result.push_back(v * 2024);
        }
    }

    return result;
}

// brute force
int solverP1(const std::vector<long long> &inputData) {
    int times = 25;
    std::vector<long long> data(inputData);
    for (int i = 0; i < times; i++) {
        data = blink(data);
    }

    return static_cast<int>(data.size());
}


// recursion with memo dict
std::map<std::pair<long long, int>, long long> cache;

long long partialBlink(const long long &v, int times) {
    if (times == 0) {
        return 1;
    }

    if (cache.count({v, times})) {
        return cache[{v, times}];
    }

    if (v == 0) {
        cache[{v, times}] = partialBlink(1, times - 1);
        return cache[{v, times}];
    }

    const auto strV = std::to_string(v);
    const auto len = strV.length();

    if (len % 2 == 0) {
        const auto left = strV.substr(0, strV.size() / 2);
        const auto right = strV.substr(strV.size() / 2);

        cache[{v, times}] = partialBlink(std::stoll(left), times - 1) + partialBlink(std::stoll(right), times - 1);
        return cache[{v, times}];
    }

    cache[{v, times}] = partialBlink(v * 2024, times - 1);
    return cache[{v, times}];

}

long long solverP2(const std::vector<long long> &inputData) {
    int times = 75;
    std::vector<long long> data(inputData);

    long long result = 0;
    for (const auto &v: data) {
        result += partialBlink(v, times);
    }

    return result;
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