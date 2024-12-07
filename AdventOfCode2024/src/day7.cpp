#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>

struct InputData {
    std::vector<long long> targets;
    std::vector<std::vector<long long>> valuesVec;
};

std::vector<long long> splitIntegers(const std::string &s, const char delimiter) {
    std::vector<long long> tokens;
    std::string token;
    std::istringstream tokenStream(s);
    while (std::getline(tokenStream, token, delimiter)) {
        tokens.push_back(std::stoll(token));
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

    std::string line;
    while (std::getline(inputFile, line)) {
        auto splitPos = line.find(": ");

        auto targetStr = line.substr(0, splitPos);
        long long target = std::stoll(targetStr);

        inputData.targets.push_back(target);

        auto valueStr = line.substr(splitPos + 2);
        auto values = splitIntegers(valueStr, ' ');

        inputData.valuesVec.push_back(values);
    }

    inputFile.close();

    return inputData;
}


bool dfsP1(int index, const long long &target, const std::vector<long long> &values, const long long curValue) {
    if (index == values.size()) {
        return curValue == target;;
    }

    if (curValue > target) {
        return false;
    }

    long long newValueAdd = curValue + values[index];
    long long newValueMul = curValue * values[index];

    return dfsP1(index + 1, target, values, newValueMul) || dfsP1(index + 1, target, values, newValueAdd);
}

long long solverP1(const InputData &inputData) {
    long long result = 0;

    const auto &targets = inputData.targets;
    const auto &valuesVec = inputData.valuesVec;

    for (int i = 0; i < targets.size(); i++) {
        const long long target = targets[i];
        const auto &values = valuesVec[i];
        if (dfsP1(1, target, values, values[0])) {
            result += target;
        }
    }

    return result;
}

long long concatValue(const long long &a, const long long &b) {
    return std::stoll(std::to_string(a) + std::to_string(b));
}

bool dfsP2(int index, const long long &target, const std::vector<long long> &values, const long long curValue) {
    if (index == values.size()) {
        return curValue == target;
    }

    if (curValue > target) {
        return false;
    }

    long long newValueAdd = curValue + values[index];
    long long newValueMul = curValue * values[index];
    long long newValueConcat = concatValue(curValue, values[index]);

    return dfsP2(index + 1, target, values, newValueConcat) || dfsP2(index + 1, target, values, newValueMul) ||
           dfsP2(index + 1, target, values, newValueAdd);

}

long long solverP2(const InputData &inputData) {
    long long result = 0;

    const auto &targets = inputData.targets;
    const auto &valuesVec = inputData.valuesVec;

    for (int i = 0; i < targets.size(); i++) {
        const long long target = targets[i];
        const auto &values = valuesVec[i];
        if (dfsP2(1, target, values, values[0])) {
            result += target;
        }
    }

    return result;
}

int main(int argc, char **argv) {
    std::string filePath = argv[1];

    auto input = readInput(filePath);

    auto resultP1 = solverP1(input);
    std::cout << "P1 result: " << resultP1 << std::endl;

    auto resultP2 = solverP2(input);
    std::cout << "P2 result: " << resultP2 << std::endl;

    return 0;
}
