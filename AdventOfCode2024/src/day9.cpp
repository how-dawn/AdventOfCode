#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

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

std::vector<std::vector<std::string>> preprocessor(const std::string &diskMap) {
    std::vector<std::vector<std::string>> blocks;
    int id = 0;
    for (int i = 0; i < static_cast<int>(diskMap.size()); i++) {
        std::string elem = ".";
        const int num = diskMap[i] - '0';
        if (i % 2 == 0) {
            elem = std::to_string(id);
            id++;
        }
        std::vector<std::string> block(num, elem);
        blocks.push_back(block);
    }

    return blocks;
}


std::vector<std::string> reduceDim(const std::vector<std::vector<std::string>> &blocks) {
    std::vector<std::string> flatBlocks;
    for (const auto &b: blocks) {
        for (const auto &s: b) {
            if (!s.empty()) {
                flatBlocks.push_back(s);
            }
        }
    }
    return flatBlocks;
}

std::vector<std::string>
moveFileBlocks(std::vector<std::vector<std::string>> &blocks) {
    auto flatBlocks = reduceDim(blocks);

    int i = 0, j = static_cast<int>(flatBlocks.size()) - 1;
    while (i != j) {
        if (flatBlocks[i] != ".") {
            i++;
            continue;
        } else {
            std::swap(flatBlocks[i], flatBlocks[j]);
            j--;
            while (flatBlocks[j] == ".") {
                j--;
            }
        }
    }
    return flatBlocks;
}

long long solverP1(const std::vector<std::string> &inputData) {
    if (inputData.size() > 1) {
        std::cerr << "Error reading disk map. " << std::endl;
        return -1;
    }

    const auto &diskMap = inputData[0];
    auto blocks = preprocessor(diskMap);

    const auto flatBlocks = moveFileBlocks(blocks);

    long long res = 0L;
    long long k = 0L;
    for (const auto &s: flatBlocks) {
        if (s == ".")
            break;
        long long v = std::stoll(s);
        res += (k * v);
        k++;
    }

    return res;
}

std::pair<int, int> getFreeSpace(const std::vector<std::string> &info) {
    int start = -1;
    for (int i = 0; i < info.size(); i++) {
        if (info[i] == ".") {
            start = i;
            break;
        }
    }

    if (start != -1) {
        return {start, info.size() - start};
    }

    return {-1, info.size()};
}


std::vector<std::string>
moveFileBlocksP2(std::vector<std::vector<std::string>> &blocks) {
    for (int j = static_cast<int>(blocks.size()) - 1; j >= 1; j -= 2) {
        const auto fileSize = blocks[j].size();
        if (fileSize > 0 && blocks[j][0] == ".") {
            continue;
        }
        for (int i = 1; i < blocks.size(); i += 2) {
            auto [start, leftSize] = getFreeSpace(blocks[i]);
            if (start == -1) {
                continue;
            }

            if (leftSize >= fileSize && j > i) {
                for (int k = 0; k < fileSize; k++) {
                    std::swap(blocks[i][start], blocks[j][k]);
                    start++;
                }
            }
        }
    }

    return reduceDim(blocks);
}

long long solverP2(const std::vector<std::string> &inputData) {
    if (inputData.size() > 1) {
        std::cerr << "Error reading disk map. " << std::endl;
        return -1;
    }

    const auto &diskMap = inputData[0];
    auto blocks = preprocessor(diskMap);

    const auto flatBlocks = moveFileBlocksP2(blocks);

    long long res = 0L;
    long long k = 0L;
    for (const auto s: flatBlocks) {
        if (s == ".") {
            k++;
            continue;
        }
        long long v = std::stoll(s);
        res += (k * v);
        k++;
    }

    return res;
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