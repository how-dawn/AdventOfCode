#include <filesystem>
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>

using Coord = std::pair<int, int>;
using Direct = std::pair<int, int>;

const std::vector<Direct> directions = {{-1, 1}, {0, 1},   {1, 1},  {-1, 0},
                                        {1, 0},  {-1, -1}, {0, -1}, {1, -1}};

const std::vector<Direct> directionsP2 = {{-1, 1}, {1, 1}, {-1, -1}, {1, -1}};

const std::string targetP1 = "XMAS";
const std::string targetP2 = "MAS";

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

bool isCoordValid(const Coord &coord, const std::vector<std::string> &map) {
  int ySize = map.size();
  int xSize = map[0].size();

  const auto &[x, y] = coord;

  return x >= 0 && x <= xSize - 1 && y >= 0 && y <= ySize - 1;
}

int dfsP1(const std::vector<std::string> &map, const Coord &coord,
          const Direct &direct, std::string current) {
  int result = 0;

  const auto &[x, y] = coord;
  current.insert(current.end(), 1, map[x][y]);

  // std::cout << x << ", " << y << "  " << current << std::endl;

  if (current.size() == 4) {
    if (current == targetP1) {
      return 1;
    }
    return 0;
  }

  const auto &[dx, dy] = direct;
  Coord newCoord = {x + dx, y + dy};
  if (isCoordValid(newCoord, map)) {
    result += dfsP1(map, newCoord, direct, current);
  }

  return result;
}

int solverP1(const std::vector<std::string> &map) {
  int result = 0;

  for (int j = 0; j < map.size(); j++) {
    for (int i = 0; i < map[0].size(); i++) {

      for (const auto &direct : directions) {
        std::string cur = "";
        result += dfsP1(map, {i, j}, direct, cur);
      }
    }
  }

  return result;
}

void dfsP2(const std::vector<std::string> &map, const Coord &coord,
           const Direct &direct, std::string current,
           std::map<Coord, int> &ret) {
  const auto &[x, y] = coord;
  current.insert(current.end(), 1, map[x][y]);

  // std::cout << x << ", " << y << "  " << current << std::endl;

  if (current.size() == 3) {
    if (current == targetP2) {
      const auto &[dx, dy] = direct;
      Coord lastCoord = {x - dx, y - dy};
      ret[lastCoord]++;
    }
  }

  const auto &[dx, dy] = direct;
  Coord newCoord = {x + dx, y + dy};
  if (isCoordValid(newCoord, map)) {
    dfsP2(map, newCoord, direct, current, ret);
  }
}

int solverP2(const std::vector<std::string> &map) {
  int result = 0;

  std::map<Coord, int> ret;

  for (int j = 0; j < map.size(); j++) {
    for (int i = 0; i < map[0].size(); i++) {
      for (const auto &direct : directionsP2) {
        std::string cur = "";
        dfsP2(map, {i, j}, direct, cur, ret);
      }
    }
  }

  for (const auto [coord, cnt] : ret) {
    if (cnt == 2) {
      // std::cout << coord.first << ", " << coord.second << " | "
      //           << map[coord.first][coord.second] << " | "
      //           << ret[{coord.first, coord.second}] << std::endl;
      result++;
    }
  }

  return result;
}

int main(int argc, char *argv[]) {
  std::string filePath = argv[1];

  auto input = readInput(filePath);
  // std::cout << "Input: " << input << std::endl;

  int resultP1 = solverP1(input);
  std::cout << "P1 result: " << resultP1 << std::endl;

  int resultP2 = solverP2(input);
  std::cout << "P2 result: " << resultP2 << std::endl;

  return 0;
}
