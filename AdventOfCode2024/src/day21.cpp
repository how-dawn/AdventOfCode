#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using Direction = std::pair<int, int>;
using Position = std::pair<int, int>;

const std::vector<std::string> NumericKeyPad{
        "789", "456", "123", "#0A"
};

const std::vector<std::string> DirectionalKeyPad{
        "#^A", "<v>"
};

std::vector<std::string> readInput(const std::string &filePath) {
    if (!std::filesystem::exists(filePath)) {
        std::cerr << "File does not exist: " << filePath << std::endl;
        return {};
    }

    std::ifstream inputFile(filePath);
    if (!inputFile) {
        std::cerr << "Error opening file: " << filePath << std::endl;
        return {};
    }

    std::vector<std::string> input;
    std::string line;
    while (std::getline(inputFile, line)) {
        input.push_back(line);
    }

    inputFile.close();

    return input;
}

Position findInMap(const std::vector<std::string> &map, const char &c) {
    for (int i = static_cast<int>(map.size() - 1); i >= 0; i--) {
        for (int j = 0; j < static_cast<int>(map[i].size()); j++) {
            if (map[i][j] == c)
                return {i, j};
        }
    }
    return {-1, -1};
}


std::vector<std::string> sp(const std::vector<std::string> &keypad, const Position &from, const Position &to) {
    auto [fx, fy] = from;
    auto [tx, ty] = to;

    int dx = tx - fx, dy = ty - fy;

    std::string xMvs = dx >= 0 ? std::string(dx, 'v') : std::string(-dx, '^');
    std::string yMvs = dy >= 0 ? std::string(dy, '>') : std::string(-dy, '<');

    if (dx == 0 && dy == 0) {
        return {""};
    } else if (dx == 0) {
        return {yMvs};
    } else if (dy == 0) {
        return {xMvs};
    } else if (keypad[fx][ty] == '#') {
        return {xMvs + yMvs};
    } else if (keypad[tx][fy] == '#') {
        return {yMvs + xMvs};
    } else {
        return {xMvs + yMvs, yMvs + xMvs};
    }
}

uint64_t dfs(std::string pwd, int depth, int maxDepth) {
    if (depth > maxDepth) {
        return pwd.length();
    }
    const auto &keypad = depth == 1 ? NumericKeyPad : DirectionalKeyPad;

    pwd = 'A' + pwd;
    int i = 0, j = 1;

    uint64_t result = 0;
    while (i < pwd.size() && j < pwd.size()) {
        auto paths = sp(keypad,
                        findInMap(keypad, pwd[i]),
                        findInMap(keypad, pwd[j]));

        auto minVal = std::numeric_limits<uint64_t>::max();
        for (const auto &path: paths) {
            minVal = std::min(minVal, dfs(path + 'A', depth + 1, maxDepth));
        }

        result += minVal;

        i++;
        j++;
    }
    return result;
}

uint64_t solverP1(const std::vector<std::string> &inputData) {
    uint64_t result = 0;
    for (const auto &pwd: inputData) {
        uint64_t val = std::stoll(pwd.substr(0, pwd.size() - 1));

        auto spLen = dfs(pwd, 1, 3);

        result += spLen * val;
    }

    return result;
}

//uint64_t solverP2(const std::vector<std::string> &inputData) {
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