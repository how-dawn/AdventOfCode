#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <unordered_set>
#include <set>
#include <map>
#include <algorithm>
#include <queue>

using Warehouse = std::vector<std::string>;
using Position = std::pair<int, int>;
using Direction = std::pair<int, int>;
using Size = std::pair<int, int>;

const std::map<char, Direction> mvMap = {
        {'>', {0,  1}},
        {'v', {1,  0}},
        {'<', {0,  -1}},
        {'^', {-1, 0}}
};

namespace Visualization {
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

    void saveWarehouseToFile(const std::vector<std::string> &wh, const std::string &filename) {
        std::ofstream outFile(expandHomePath(filename));

        if (!outFile) {
            std::cerr << "Error: Could not open file: " << expandHomePath(filename) << std::endl;
            return;
        }

        for (const auto &row: wh) {
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

}

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

Warehouse updateWarehouse(const Warehouse &wh) {
    Warehouse warehouse;
    for (const auto &row: wh) {
        std::string nRow;
        for (const auto &c: row) {
            if (c == '#') {
                nRow += "##";
            } else if (c == 'O') {
                nRow += "[]";
            } else if (c == '.') {
                nRow += "..";
            } else if (c == '@') {
                nRow += "@.";
            }
        }
        warehouse.push_back(nRow);
    }
    return warehouse;
}

std::pair<bool, bool>
checkState(const Warehouse &warehouse, const std::vector<Position> &boxes, const Direction &direction,
           const char &curVal) {
    const bool isVerticalMovement = (direction == mvMap.at('^') || direction == mvMap.at('v'));

    if (boxes.empty()) {
        if (curVal == '.') {
            return {false, true};
        }
        if (curVal == '#') {
            return {false, false};
        }

    }
    const auto &[dx, dy] = direction;
    const auto &[x1, y1] = boxes.back();
    const auto &[x2, y2] = *(boxes.end() - 2);

    int nX1 = x1 + dx, nY1 = y1 + dy;
    int nX2 = x2 + dx, nY2 = y2 + dy;

    if (warehouse[nX1][nY1] == '#' || warehouse[nX2][nY2] == '#') {
        return {false, false}; // {stop checking, unmovable}
    }

    if (isVerticalMovement) {
        if (warehouse[nX1][nY1] == '.' && warehouse[nX2][nY2] == '.') {
            return {false, true}; // {stop checking, movable}
        } else if (warehouse[nX1][nY1] == '[' || warehouse[nX2][nY2] == '[' || warehouse[nX1][nY1] == ']' ||
                   warehouse[nX2][nY2] == ']') {
            return {true, true}; // {checking, movable}
        }
    } else {
        if (warehouse[nX1][nY1] == '.' || warehouse[nX2][nY2] == '.') {
            return {false, true};  // {continue checking, movable}
        }
    }

    return {true, false};
}

std::pair<bool, std::set<Position>>
bfs(const Warehouse &warehouse, const Position &curPos, const Direction &direction) {
    const auto &[dx, dy] = direction;

    std::set<Position> boxes;

    std::queue<Position> q;
    q.push(curPos);

    size_t sz;


    while (!q.empty()) {
        sz = q.size();
        int dotCnt = 0;
        for (auto i = 0; i < sz; i++) {
            const auto &[x, y] = q.front();
            q.pop();
            const char curVal = warehouse[x][y];

            if (curVal == '#') {
                return {false, {}};
            }

            if (curVal == '[' || curVal == ']') {
                boxes.emplace(x, y);
            }

            if (curVal == '.') {
                dotCnt++;
                continue;
            }

            int newX = x + dx, newY = y + dy;
            if (isInWarehouse(warehouse, {newX, newY})) {
                char newVal = warehouse[newX][newY];
                q.emplace(newX, newY);

                if (newVal == '[') {
                    q.emplace(newX, newY + 1);
                } else if (newVal == ']') {
                    q.emplace(newX, newY - 1);
                }

            }
        }

        if (dotCnt == sz) {
            break;
        }
    }

    return {true, boxes};
}

Position moveRobotVertically(Warehouse &warehouse, const Position &curPosition, const Direction &direction) {
    const auto &[x, y] = curPosition;
    const auto &[dx, dy] = direction;

    const auto [movable, boxes_] = bfs(warehouse, curPosition, direction);
    std::vector<Position> boxes(boxes_.begin(), boxes_.end());

    if (!movable) {
        return {x, y};
    }

    Warehouse whcp(warehouse.begin(), warehouse.end());
    if (direction == mvMap.at('v')) {
        std::sort(boxes.begin(), boxes.end(), [](const auto &pos1, const auto &pos2) {
            const auto &[x1, y1] = pos1;
            const auto &[x2, y2] = pos2;
            if (x1 == x2) {
                return y1 > y2;
            }
            return x1 > x2;
        });
    } else {
        std::sort(boxes.begin(), boxes.end(), [](const auto &pos1, const auto &pos2) {
            const auto &[x1, y1] = pos1;
            const auto &[x2, y2] = pos2;
            if (x1 == x2) {
                return y1 < y2;
            }
            return x1 < x2;
        });
    }
    for (const auto &[nX, nY]: boxes) {
        warehouse[nX + dx][nY + dy] = whcp[nX][nY];
        warehouse[nX][nY] = '.';
    }
    warehouse[x][y] = '.';
    warehouse[x + dx][y + dy] = '@';
    return {x + dx, y + dy};
}

Position moveRobotHorizontally(Warehouse &warehouse, const Position &curPosition, const Direction &direction) {
    const auto &[x, y] = curPosition;
    const auto &[dx, dy] = direction;
    std::vector<Position> boxes;

    bool movable = true;
    int cx = x + dx, cy = y + dy;

    while (isInWarehouse(warehouse, {cx, cy})) {
        const auto curVal = warehouse[cx][cy];
        if (curVal == '[' || curVal == ']') {
            boxes.emplace_back(cx, cy);
        }

        if (curVal == '#') {
            movable = false;
            break;
        }
        if (curVal == '.') {
            movable = true;
            break;
        }

        cx = cx + dx;
        cy = cy + dy;
    }


    if (movable) {
        for (auto it = boxes.rbegin(); it != boxes.rend(); ++it) {
            const auto &[bx, by] = *it;
            warehouse[bx + dx][by + dy] = warehouse[bx][by];
            warehouse[bx][by] = '.';
        }

        warehouse[x][y] = '.';
        warehouse[x + dx][y + dy] = '@';
        return {x + dx, y + dy};
    }
    return {x, y};
}

Position moveRobotP2(Warehouse &warehouse, const Position &curPosition, const Direction &direction) {
    const auto &[x, y] = curPosition;
    const auto &[dx, dy] = direction;
    std::vector<Position> boxes;

    const bool isVertical = (direction == mvMap.at('v')) || (direction == mvMap.at('^'));
    if (!isVertical) {
        return moveRobotHorizontally(warehouse, curPosition, direction);
    }

    return moveRobotVertically(warehouse, curPosition, direction);

}

int getGPSCoordsP2(const Warehouse &warehouse) {
    int result = 0;
    for (auto i = 0; i < warehouse.size(); i++) {
        for (auto j = 0; j < warehouse[i].size(); j++) {
            if (warehouse[i][j] == '[') {
                result += 100 * i + j;
            }
        }
    }
    return result;
}

int solverP2(const std::pair<Warehouse, std::string> &input) {
    const auto &[warehouse_, movements] = input;
    auto warehouse = updateWarehouse(warehouse_);
    auto rPos = findRobot(warehouse);
    // visualization
    {
        std::string filePath = "~/Workspace/Dev/AdventOfCode/AdventOfCode2024/data/day15/output/day15_visualization_" +
                               std::to_string(0) + ".output";
        Visualization::saveWarehouseToFile(warehouse, filePath);
    }

    int i = 1;
    for (const auto mv: movements) {
        const auto direction = mvMap.at(mv);
        rPos = moveRobotP2(warehouse, rPos, direction);

        // visualization
        {
            std::string filePath =
                    "~/Workspace/Dev/AdventOfCode/AdventOfCode2024/data/day15/output/day15_visualization_" +
                    std::to_string(i) + ".output";
            Visualization::saveWarehouseToFile(warehouse, filePath);
            i++;
        }

    }

    return getGPSCoordsP2(warehouse);
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