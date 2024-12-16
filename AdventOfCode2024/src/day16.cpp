#include <climits>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <map>
#include <queue>
#include <set>
#include <string>
#include <vector>

using Position = std::pair<int, int>;
using Direction = std::pair<int, int>;
using State = std::pair<Position, Direction>;
using StateScore = std::pair<State, int>;
using Path = std::vector<Position>;
using StateScorePath = std::pair<StateScore, Path>;

std::map<Direction, std::vector<Direction>> directionMap = {
        {{1,  0},  {{0,  -1}, {0,  1}}},
        {{0,  1},  {{1,  0},  {-1, 0}}},
        {{-1, 0},  {{0,  -1}, {0,  1}}},
        {{0,  -1}, {{-1, 0},  {1,  0}}}};

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

bool isInMap(const std::vector<std::string> &map, const Position &coord) {
    int xSize = static_cast<int>(map.size());
    int ySize = static_cast<int>(map[0].size());

    const auto &[x, y] = coord;

    return x >= 0 && x < xSize && y >= 0 && y < ySize;
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

int bfs(const std::vector<std::string> &map, const Position &curPos,
        const Direction &curDirection) {
    int result = INT_MAX;
    std::queue<StateScore> q;
    std::map<State, int> visited;

    State initState = {curPos, curDirection};
    q.push({initState, 0});

    while (!q.empty()) {
        const auto [state, score] = q.front();
        const auto [pos, direct] = state;
        q.pop();

        if (visited.count(state) && visited[state] < score) {
            continue;
        }
        visited[state] = score;

        const auto &[x, y] = pos;

        if (map[x][y] == 'E') {
            result = std::min(score, result);
            continue;
        }

        const auto &[dx_, dy_] = direct;
        int newX_ = x + dx_, newY_ = y + dy_;
        if (isInMap(map, {newX_, newY_}) && map[newX_][newY_] != '#') {
            Position newPos = {newX_, newY_};
            q.push({{newPos, direct}, score + 1});
        }

        const auto &directions = directionMap[direct];
        for (const auto &newDirect: directions) {
            q.push({{pos, newDirect}, score + 1000});
        }
    }

    return result;
}

int solverP1(const std::vector<std::string> &inputData) {
    const auto startPos = findInMap(inputData, 'S');
    return bfs(inputData, startPos, {0, 1});
}

int p2Helper(const std::set<Path> &paths) {
    std::set<Position> positions;
    for (const auto &path: paths) {
        for (const auto &pos: path) {
            positions.insert(pos);
        }
    }
    return positions.size();
}

int bfsP2(const std::vector<std::string> &map, const Position &curPos,
          const Direction &curDirection) {
    int minScore = INT_MAX;
    std::set<Path> paths;

    std::queue<StateScorePath> q;
    std::map<State, int> visited;

    State initState = {curPos, curDirection};
    q.push({{initState, 0},
            {}});

    while (!q.empty()) {
        auto [stateScore, path] = q.front();
        const auto &[state, score] = stateScore;
        const auto &[pos, direct] = state;
        q.pop();

        if (visited.count(state) && visited[state] < score) {
            continue;
        }
        visited[state] = score;
        path.push_back(pos);

        const auto &[x, y] = pos;

        if (map[x][y] == 'E') {
            if (score > minScore) {
                continue;
            }

            if (score < minScore) {
                minScore = score;
                paths.clear();
            }
            paths.insert(path);
            continue;
        }

        const auto &[dx_, dy_] = direct;
        int newX_ = x + dx_, newY_ = y + dy_;
        if (isInMap(map, {newX_, newY_}) && map[newX_][newY_] != '#') {
            Position newPos = {newX_, newY_};
            q.push({{{newPos, direct}, score + 1}, path});
        }

        const auto &directions = directionMap[direct];
        for (const auto &newDirect: directions) {
            q.push({{{pos, newDirect}, score + 1000}, path});
        }
    }

    return p2Helper(paths);
}

int solverP2(const std::vector<std::string> &inputData) {
    const auto startPos = findInMap(inputData, 'S');
    return bfsP2(inputData, startPos, {0, 1});
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