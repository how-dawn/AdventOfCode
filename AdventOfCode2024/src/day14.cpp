#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <regex>
#include <set>

using Position = std::pair<int, int>;
using Velocity = std::pair<int, int>;
using Robot = std::pair<Position, Velocity>;

using Size = std::pair<int, int>;

std::vector<Robot> readInput(const std::string &filePath) {
    if (!std::filesystem::exists(filePath)) {
        std::cerr << "File does not exist: " << filePath << std::endl;
        return {};
    }

    std::ifstream inputFile(filePath);
    if (!inputFile) {
        std::cerr << "Error opening file: " << filePath << std::endl;
        return {};
    }

    std::regex pattern(R"(p=(-?\d+),(-?\d+)\s+v=(-?\d+),(-?\d+))");


    std::vector<Robot> input;
    std::string line;
    while (std::getline(inputFile, line)) {
        std::smatch matches;
        if (std::regex_match(line, matches, pattern)) {
            Robot robot = {{std::stoi(matches[1]), std::stoi(matches[2])},
                           {std::stoi(matches[3]), std::stoi(matches[4])}};
            input.push_back(robot);
        }
    }

    inputFile.close();

    return input;
}

Position finalPos(const int t, const Size &size, const Robot &robot) {
    const auto &[pos, vel] = robot;
    const auto &[x, y] = pos;
    const auto &[dx, dy] = vel;
    const auto &[sx, sy] = size;

    auto newX = x + t * dx, newY = y + t * dy;
    newX = newX % sx;
    newY = newY % sy;
    newX = newX < 0 ? sx + newX : newX;
    newY = newY < 0 ? sy + newY : newY;

    return {newX, newY};
}

int solverP1(const std::vector<Robot> &robots, const int time, const Size &size) {
    std::vector<Position> finalPositions;
    for (const auto &robot: robots) {
        auto pos = finalPos(time, size, robot);
        finalPositions.push_back(pos);
    }

    auto &[sx, sy] = size;
    std::vector<std::vector<Position>> quadrants{
            {{0,          0},          {sx / 2 - 1, sy / 2 - 1}},
            {{0,          sy / 2 + 1}, {sx / 2 - 1, sy - 1}},
            {{sx / 2 + 1, 0},          {sx - 1,     sy / 2 - 1}},
            {{sx / 2 + 1, sy / 2 + 1}, {sx - 1,     sy - 1}}
    };

    int result = 1;

    for (const auto &q: quadrants) {
        int cnt = 0;
        const auto &[tlX, tlY] = q[0];
        const auto &[brX, brY] = q[1];

        for (const auto &[x, y]: finalPositions) {
            if (x >= tlX && y >= tlY && x <= brX && y <= brY) {
                cnt++;
            }
        }

        result *= cnt;
    }

    return result;
}

std::string expandHomePath(const std::string &path) {
    if (!path.empty() && path[0] == '~') {
        const char *home = std::getenv("HOME");
        if (!home) {
            std::cerr << "Error: HOME environment variable not set." << std::endl;
            return path;
        }
        return std::string(home) + path.substr(1);
    }
    return path;
}

std::vector<std::vector<int>> generateImage(const Size &size, std::set<Position> &positions) {
    const auto &[sx, sy] = size;
    std::vector<std::vector<int>> image(sx, std::vector<int>(sy, 0));
    for (const auto &[x, y]: positions) {
        image[x][y] = 1;
    }

    return image;
}

void saveImageToFile(const std::vector<std::vector<int>> &image, const std::string &filename) {
    std::ofstream outFile(expandHomePath(filename));

    if (!outFile) {
        std::cerr << "Error: Could not open file: " << filename << std::endl;
        return;
    }

    for (const auto &row: image) {
        for (const auto &elem: row) {
            outFile << elem;
        }
        outFile << "\n";
    }

    outFile.close();
    if (!outFile.good()) {
        std::cerr << "Write Error!" << std::endl;
    }
}

int solverP2(const std::vector<Robot> &robots, const Size &size) {
    int time = 0;
    while (true) {
        std::set<Position> finalPositions;
        bool flag = true;
        for (const auto &robot: robots) {
            auto pos = finalPos(time, size, robot);
            if (finalPositions.count(pos)) {
                flag = false;
                break;
            }
            finalPositions.insert(pos);
        }

        if (flag) {
            saveImageToFile(generateImage(size, finalPositions),
                            "~/Workspace/Dev/AdventOfCode/AdventOfCode2024/data/day14/day14_easter_egg.output");
            return time;
        } else {
            time++;
        }
    }
}

int main(int argc, char *argv[]) {
    const std::string &filePath = argv[1];
    auto input = readInput(filePath);

    Size size = {101, 103};
    int time = 100;
    auto resultP1 = solverP1(input, time, size);
    std::cout << "P1 result: " << resultP1 << std::endl;

    auto resultP2 = solverP2(input, size);
    std::cout << "P2 result: " << resultP2 << std::endl;

    return 0;
}