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

int solverP1(const std::vector<Rotation> &sList) {
    int res = 0;
    int val = 50;

    for (const auto [l, v]: sList) {
        val = (l == 'L') ? (val - v) % 100 : (val + v) % 100;
        if (val < 0) {
            val = val + 100;
        } else if (val > 99) {
            val = val - 100;
        }

        if (val == 0) res++;
        std::cout << "[DEBUG] " << l << " " << v << ": " << val << std::endl;
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
