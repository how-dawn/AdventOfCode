#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <regex>

struct Operation {
    std::string v1;
    std::string v2;
    std::string op;
};

using InputData = std::pair<std::map<std::string, int>, std::map<std::string, Operation>>;

InputData readInput(const std::string &filePath) {
    if (!std::filesystem::exists(filePath)) {
        std::cerr << "File does not exist: " << filePath << std::endl;
        return {};
    }

    std::ifstream inputFile(filePath);
    if (!inputFile) {
        std::cerr << "Error opening file: " << filePath << std::endl;
        return {};
    }

    std::map<std::string, int> wires;
    std::map<std::string, Operation> wireOperations;
    std::regex wirePattern(R"(([xy]\d{2}): (\d))");
    std::regex operationPattern(
            R"((\w+|[xy]\d{2}) (AND|OR|XOR) (\w+|[xy]\d{2}) -> (\w+|[xy]\d{2}))");
    std::smatch match;

    std::string line;
    while (std::getline(inputFile, line)) {
        if (std::regex_match(line, match, wirePattern)) {
            wires[match[1]] = std::stoi(match[2]);
        }

        if (std::regex_match(line, match, operationPattern)) {
            wireOperations[match[4]] = {match[1], match[3], match[2]};
        }
    }

    inputFile.close();

    return {wires, wireOperations};
}

void dfs(const std::string &wire, const Operation &operation, const std::map<std::string, Operation> &wireOperations,
         std::map<std::string, int> &wires) {
    if (wires.count(wire)) {
        return;
    }

    const auto &[v1, v2, op] = operation;
    if (!wires.count(v1)) {
        if (wireOperations.count(v1)) {
            dfs(v1, wireOperations.at(v1), wireOperations, wires);
        }
    }
    if (!wires.count(v2)) {
        if (wireOperations.count(v2)) {
            dfs(v2, wireOperations.at(v2), wireOperations, wires);
        }
    }

    if (op == "XOR") {
        wires[wire] = wires[v1] ^ wires[v2];
    } else if (op == "AND") {
        wires[wire] = wires[v1] & wires[v2];
    } else if (op == "OR") {
        wires[wire] = wires[v1] | wires[v2];
    }

}

std::map<std::string, int>
process(const std::map<std::string, Operation> &wireOperations, std::map<std::string, int> wires) {
    for (const auto &[wire, operation]: wireOperations) {
        dfs(wire, operation, wireOperations, wires);
    }

    return wires;
}

uint64_t solverP1(const InputData &inputData) {
    const auto &[wires, wireOperations] = inputData;
    const auto rWires = process(wireOperations, wires);

    std::string result;
    for (const auto &[wire, value]: rWires) {
        if (*wire.begin() == 'z') {
            result.insert(0, std::to_string(value));
        }
    }

    return std::bitset<64>(result).to_ullong();
}

//uint64_t solverP2(const InputData &inputData) {
//    return 0;
//}

int main(int argc, char *argv[]) {
    const std::string &filePath = argv[1];
    auto input = readInput(filePath);

    auto resultP1 = solverP1(input);
    std::cout << "P1 result: " << resultP1 << std::endl;

//    auto resultP2 = solverP2(input);
//    std::cout << "P2 result: " << resultP2 << std::endl;

    return 0;
}