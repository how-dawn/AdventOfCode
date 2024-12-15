#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <map>


using Warehouse = std::vector<std::string>;
using Position = std::pair<int, int>;
using Direction = std::pair<int, int>;

//const std::map<char, Direction> mvMap = {
//        {'>', {1,  0}},
//        {'v', {0,  1}},
//        {'<', {-1, 0}},
//        {'^', {0,  -1}}
//};

const std::map<char, Direction> mvMap = {
        {'>', {0,  1}},
        {'v', {1,  0}},
        {'<', {0,  -1}},
        {'^', {-1, 0}}
};

std::pair<Warehouse, std::string> readInput(const std::string &filePath) {
    if (!std::filesystem::exists(filePath)) {
        std::cerr << "File does not exist: " << filePath << std::endl;
        return {};
    }

    std::ifstream inputFile(filePath);
    if (!inputFile) {
        std::cerr << "Error opening file: " << filePath << std::endl;
        return {};
    }

    std::set<char> mvTemplate{'<', '>', '^', 'v'};
    Warehouse warehouse;
    std::string movements;

    std::string line;
    while (std::getline(inputFile, line)) {
        if (line.front() == '#' && line.back() == '#') {
            warehouse.push_back(line);
            continue;
        }

        auto check = std::all_of(line.begin(), line.end(), [&mvTemplate](const auto &c) {
            return mvTemplate.count(c);
        });

        if (check) {
            movements += line;
        }
    }
    inputFile.close();

    return {warehouse, movements};
}

void printWarehouse(const Warehouse &warehouse) {
    for (const auto &r: warehouse) {
        std::cout << r << std::endl;
    }
    std::cout << std::endl;
}

Position findRobot(const Warehouse &warehouse) {
    for (auto i = 0; i < warehouse.size(); i++) {
        for (auto j = 0; j < warehouse[i].size(); j++) {
            if (warehouse[i][j] == '@') {
                return {i, j};
            }
        }
    }
    return {-1, -1};
}

bool isInWarehouse(const Warehouse &wh, const Position &pos) {
    int xSize = static_cast<int>(wh.size());
    int ySize = static_cast<int>(wh[0].size());

    const auto &[x, y] = pos;

    return x >= 0 && x < xSize && y >= 0 && y < ySize;
}

Position moveRobot(Warehouse &warehouse, const Position &curPosition, const Direction &direction) {
    const auto &[x, y] = curPosition;
    const auto &[dx, dy] = direction;
    std::vector<Position> boxes;

    bool movable = false;
    int cx = x + dx, cy = y + dy;

    if (warehouse[cx][cy] == '#') {
        return {x, y};
    }

    while (isInWarehouse(warehouse, {cx, cy})) {
        const auto curVal = warehouse[cx][cy];
        if (curVal == 'O') {
            boxes.emplace_back(cx, cy);
        } else if (curVal == '#') {
            break;
        } else if (curVal == '.') {
            movable = true;
            break;
        }

        cx = cx + dx;
        cy = cy + dy;
    }

    if (movable) {
        for (auto it = boxes.rbegin(); it != boxes.rend(); ++it) {
            const auto &[bx, by] = *it;
            warehouse[bx + dx][by + dy] = 'O';
        }
        warehouse[x][y] = '.';
        warehouse[x + dx][y + dy] = '@';
        return {x + dx, y + dy};
    }

    return {x, y};

}

int getGPSCoords(const Warehouse &warehouse) {
    int result = 0;
    for (auto i = 0; i < warehouse.size(); i++) {
        for (auto j = 0; j < warehouse[i].size(); j++) {
            if (warehouse[i][j] == 'O') {
                result += 100 * i + j;
            }
        }
    }
    return result;
}

int solverP1(const std::pair<Warehouse, std::string> &input) {
    const auto &[warehouse_, movements] = input;
    Warehouse warehouse(warehouse_.begin(), warehouse_.end());
    auto rPos = findRobot(warehouse);
//    printWarehouse(warehouse);

    for (const auto mv: movements) {
//        std::cout << mv << std::endl;
        const auto direction = mvMap.at(mv);
        rPos = moveRobot(warehouse, rPos, direction);
//        printWarehouse(warehouse);
    }

    return getGPSCoords(warehouse);

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