#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

struct Rotation {
    char direction;
    int value;
};


std::vector<Rotation> readInput(const std::string &filePath) {
    std::vector<Rotation> rotList;

    if (!std::filesystem::exists(filePath)) {
        std::cerr << "File does not exist: " << filePath << std::endl;
        return rotList;
    }

    std::ifstream inputFile(filePath);
    if (!inputFile) {
        std::cerr << "Error opening file: " << filePath << std::endl;
        return rotList;
    }

    std::string line;

    while (std::getline(inputFile, line)) {
        const auto direction = line[0];
        line.erase(line.begin());
        const auto value = std::stoi(line);
        rotList.push_back({direction, value});
    }

    inputFile.close();
    return rotList;
}

int solverP1(const std::vector<Rotation> &rotList) {
    int res = 0;
    int val = 50;

    for (const auto& [l, v]: rotList) {
        val = (l == 'L') ? (val - v) % 100 : (val + v) % 100;
        if (val < 0) {
            val = val + 100;
        } else if (val > 99) {
            val = val - 100;
        }

        if (val == 0) res++;
        // std::cout << "[DEBUG] " << l << " " << v << ": " << val << std::endl;
    }
    return res;
}

int solverP2(const std::vector<Rotation> &rotList) {
    int res = 0;
    int val = 50;

    for (const auto& [l, v]: rotList) {
        std::cout << "[DEBUG] " << val << std::endl;
        const auto raw = (l == 'L') ? val - v : val + v;
        const auto cnt = abs(raw) / 100;

        if (raw < 0 && val != 0) {
            res += 1;
        }
        if (raw == 0) {
            res += 1;
        }
        res += cnt;

        val = raw % 100;
        if (val < 0) {
            val = val + 100;
        } else if (val > 99) {
            val = val - 100;
        }

        // std::cout << "[DEBUG] \t" << l << " " << v << ": " << val << " " << raw << " " << " " << cnt << " " << res  << std::endl;
    }
    return res;
}



int main(int argc, char *argv[]) {
    const std::string filePath = argv[1];

    const auto input = readInput(filePath);

    const int resultP1 = solverP1(input);
    std::cout << "P1 result: " << resultP1 << std::endl;

    const int resultP2 = solverP2(input);
    std::cout << "P2 result: " << resultP2 << std::endl;

    return 0;
}
