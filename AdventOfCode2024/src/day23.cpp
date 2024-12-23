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

bool canAddToGroup(const Graph &graph, const std::set<std::string> &group, const std::string &newNode) {

    for (const auto &node: group) {
        if (std::find(graph.at(node).begin(), graph.at(node).end(), newNode) == graph.at(node).end()) {
            return false;
        }
    }

    return true;
}

auto cmp = [](const std::set<std::string> &a, const std::set<std::string> &b) {
    return a.size() < b.size();
};

void DFS(const Graph &graph, const std::string &node, std::set<std::string> &visited, std::set<std::string> &group,
         std::set<std::set<std::string>, decltype(cmp)> &groups) {

    visited.insert(node);
    group.insert(node);

    bool hasAdded = false;
    for (const auto &v: graph.at(node)) {
        if (!visited.count(v) && canAddToGroup(graph, group, v)) {
            hasAdded = true;
            DFS(graph, v, visited, group, groups);
        }
    }

    if (!hasAdded) {
        groups.insert(group);
    }

    visited.erase(node);
    group.erase(node);

}

std::string solverP2(const std::vector<Connection> &inputData) {
    const auto graph = createGraph(inputData);


    std::set<std::string> visited;
    std::set<std::string> group;


    std::set<std::set<std::string>, decltype(cmp)> groups(cmp);

    int t = 0;
    int total = graph.size();
    for (const auto &[k, v]: graph) {
        std::cout << t << "/" << total << std::endl;
        DFS(graph, k, visited, group, groups);
        t++;
    }


    std::ostringstream os;
    const auto &best = *(std::prev(groups.end(), 1));
    for (auto it = best.begin(); it != best.end(); it++) {
        os << *it;
        if (it != std::prev(best.end(), 1)) {
            os << ",";
        } else {
            os << std::endl;
        }
    }

    return os.str();
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