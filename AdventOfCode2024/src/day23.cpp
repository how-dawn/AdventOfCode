#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <sstream>
#include <cassert>

using Connection = std::pair<std::string, std::string>;
using Graph = std::map<std::string, std::vector<std::string>>;

std::vector<Connection> readInput(const std::string &filePath) {
    auto split = [](const std::string &s, const char delimiter) {
        std::vector<std::string> tokens;
        std::string token;
        std::istringstream tokenStream(s);
        while (std::getline(tokenStream, token, delimiter)) {
            tokens.push_back(token);
        }
        return tokens;
    };

    if (!std::filesystem::exists(filePath)) {
        std::cerr << "File does not exist: " << filePath << std::endl;
        return {};
    }

    std::ifstream inputFile(filePath);
    if (!inputFile) {
        std::cerr << "Error opening file: " << filePath << std::endl;
        return {};
    }

    std::vector<Connection> input;
    std::string line;
    while (std::getline(inputFile, line)) {
        auto nodes = split(line, '-');
        assert(nodes.size() == 2);
        input.emplace_back(nodes[0], nodes[1]);
    }

    inputFile.close();

    return input;
}

Graph createGraph(const std::vector<Connection> &connections) {
    Graph graph;
    for (const auto &[a, b]: connections) {
        graph[a].push_back(b);
        graph[b].push_back(a);
    }

    return graph;
}

std::set<std::vector<std::string>> createLANParties(const Graph &graph) {
    std::set<std::vector<std::string>> LANParties;

    for (const auto &[k, conns]: graph) {
        for (const auto &i: conns) {
            for (const auto &j: graph.at(i)) {
                if (k == j || i == j || k == i) {
                    continue;
                }
                if (std::find(graph.at(j).begin(), graph.at(j).end(), k) != graph.at(j).end()) {
                    std::vector<std::string> party{k, i, j};
                    std::sort(party.begin(), party.end());
                    LANParties.insert(party);
                }

            }
        }
    }

    return LANParties;
}


int solverP1(const std::vector<Connection> &inputData) {
    const auto graph = createGraph(inputData);
    const auto parties = createLANParties(graph);

    int result = 0;
    for (const auto &party: parties) {
        for (const auto &computer: party) {
            if (computer.front() == 't') {
                result++;
                break;
            }
        }
    }

    return result;
}

//int solverP2(const std::vector<std::string> &inputData) {
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