#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <map>
#include <set>
#include <regex>
#include <sstream>

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


/*
X	Y	Cin	Z	Cout
0	0	0	0	0
0	0	1	1	0
0	1	0	1	0
0	1	1	0	1
1	0	0	1	0
1	0	1	0	1
1	1	0	0	1
1	1	1	1	1

Z = (X ^ Y) ^ Cin
Cout = (X & Y) | (Cin & (X ^ Y))
*/
std::set<std::string> sanityCheck(const std::map<std::string, Operation> &wireOperations, const std::string &finalZ) {
    // utils
    auto in_is_xy = [](const Operation &operation) {
        const auto &[v1, v2, op] = operation;
        return (v1[0] == 'x' || v1[0] == 'y') && (v2[0] == 'x' || v2[0] == 'y');
    };

    auto in_is_00 = [](const Operation &operation) {
        const auto &[v1, v2, op] = operation;
        return v1.substr(1) == "00" && v2.substr(1) == "00";
    };

    std::set<std::string> error;
    for (const auto &wops: wireOperations) {
        // const auto &[w, operation] = wops; // C++20: captured structured bindings
        const auto &w = wops.first;
        const auto &operation = wops.second;
        const auto &[v1, v2, op] = operation;
        // Except the final Z, all Zs should be output by XOR
        if (w[0] == 'z' && w != finalZ && op != "XOR") {
            error.insert(w);
        }

        // if the output is not Z and the inputs are not X & Y
        // the OP should always be AND / OR
        if (w[0] != 'z' && !in_is_xy(operation) && op != "AND" && op != "OR") {
            error.insert(w);
        }

        // If the inputs are X & Y (but not X00 & Y00) and the operation is XOR,
        // the output W should be some operation's input and works with XOR
        if (op == "XOR" && in_is_xy(operation) && !in_is_00(operation)) {
            if (std::none_of(wireOperations.begin(), wireOperations.end(), [&](const auto &wireOperation) {
                const auto &[w_, operation_] = wireOperation;
                const auto &[v1_, v2_, op_] = operation_;
                return (v1_ == w || v2_ == w) && op_ == "XOR";
            })) {
                error.insert(w);
            }
        }

        // If the inputs are X & Y( but not X00 & Y00) and the operator is AND,
        // the output W should be some operation's input and works with XOR
        if (op == "AND" && !in_is_00(operation) && in_is_xy(operation)) {
            // w should work with another OR as the input
            if (std::none_of(wireOperations.begin(), wireOperations.end(), [&](const auto &wireOperation) {
                const auto &[w_, operation_] = wireOperation;
                const auto &[v1_, v2_, op_] = operation_;
                return (v1_ == w || v2_ == w) && op_ == "OR";
            })) {
                error.insert(w);
            }
        }
    }

    return error;
}

std::string solverP2(const InputData &inputData) {
    const auto &[wires, wireOperations] = inputData;
    auto rWires = process(wireOperations, wires);
    const auto &finalZ = (*std::prev(rWires.end(), 1)).first;

    const std::set<std::string> sWires = sanityCheck(wireOperations, finalZ);

    std::ostringstream os;
    for (auto it = sWires.begin(); it != sWires.end(); it++) {
        os << *it;
        if (it != std::prev(sWires.end(), 1)) {
            os << ",";
        }
    }

    return os.str();
}

int main(int argc, char *argv[]) {
    const std::string &filePath = argv[1];
    auto input = readInput(filePath);

    auto resultP1 = solverP1(input);
    std::cout << "P1 result: " << resultP1 << std::endl;


    // Part 2: Get the idea from
    // 1. https://www.reddit.com/r/adventofcode/comments/1hl698z/comment/m3k68gd/?utm_source=share&utm_medium=web3x&utm_name=web3xcss&utm_term=1&utm_content=share_button
    // 2. https://www.reddit.com/r/adventofcode/comments/1hl698z/comment/m3kt1je/?utm_source=share&utm_medium=web3x&utm_name=web3xcss&utm_term=1&utm_content=share_button
    // 3. https://www.reddit.com/r/adventofcode/comments/1hl698z/comment/m3ldsy3/?utm_source=share&utm_medium=web3x&utm_name=web3xcss&utm_term=1&utm_content=share_button
    // Feel a bit uncertain about this solution.
    // Prefer to work with some real swaps. Extra work still needed.
    auto resultP2 = solverP2(input);
    std::cout << "P2 result: " << resultP2 << std::endl;

    return 0;
}