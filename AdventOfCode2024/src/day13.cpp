#include <filesystem>
#include <fstream>
#include <iostream>
#include <ostream>
#include <sstream>
#include <string>
#include <vector>

struct Machine {
  std::pair<long long, long long> btnA;
  std::pair<long long, long long> btnB;
  std::pair<long long, long long> prize;
};

void printMachine(const Machine &machine) {
  std::cout << "[Button A] " << machine.btnA.first << ", "
            << machine.btnA.second << std::endl;
  std::cout << "[Button B] " << machine.btnB.first << ", "
            << machine.btnB.second << std::endl;
  std::cout << "[Prize] " << machine.prize.first << ", " << machine.prize.second
            << std::endl
            << std::endl;
}

std::vector<Machine> readInput(const std::string &filePath) {
  if (!std::filesystem::exists(filePath)) {
    std::cerr << "File does not exist: " << filePath << std::endl;
    return {};
  }

  std::ifstream inputFile(filePath);
  if (!inputFile) {
    std::cerr << "Error opening file: " << filePath << std::endl;
    return {};
  }

  std::vector<Machine> machines;
  std::vector<std::string> input;
  std::string line;
  Machine machine;
  while (std::getline(inputFile, line)) {
    std::vector<std::string> parts;
    std::string part;
    std::istringstream iss(line);

    while (iss >> part) {
      parts.push_back(part);
    }
    if (!parts.empty()) {
      if (parts[1].find('A') != std::string::npos) {
        long long x = std::stoll(parts[2].substr(1, parts[2].size() - 2));
        long long y = std::stoll(parts[3].substr(1, parts[3].size() - 1));
        machine.btnA = {x, y};
      } else if (parts[1].find('B') != std::string::npos) {
        long long x = std::stoll(parts[2].substr(1, parts[2].size() - 2));
        long long y = std::stoll(parts[3].substr(1, parts[3].size() - 1));
        machine.btnB = {x, y};
      } else if (parts[0] == "Prize:") {
        long long x = std::stoll(parts[1].substr(2, parts[1].size() - 3));
        long long y = std::stoll(parts[2].substr(2, parts[2].size() - 2));
        machine.prize = {x, y};
      }
    } else {
      machines.push_back(machine);
    }
  }
  machines.push_back(machine);

  inputFile.close();

  return machines;
}

long long howToWin(const Machine &machine) {
  const auto &[ax, ay] = machine.btnA;
  const auto &[bx, by] = machine.btnB;
  const auto &[px, py] = machine.prize;

  std::pair<long long, long long> result;

  long long nNumerator = px * ay - py * ax;
  long long nDominator = bx * ay - by * ax;

  bool sameSignN =
      (nDominator > 0 && nNumerator > 0) || (nDominator < 0 && nNumerator < 0);

  if (sameSignN && nDominator != 0) {
    if (nNumerator % nDominator) {
      return 0;
    }
    result.second = nNumerator / nDominator;
  }

  long long mNumerator = px * by - py * bx;
  long long mDominator = ax * by - ay * bx;

  bool sameSignM =
      (mDominator > 0 && mNumerator > 0) || (mDominator < 0 && mNumerator < 0);

  if (sameSignM && mDominator != 0) {
    if (mNumerator % mDominator) {
      return 0;
    }
    result.first = mNumerator / mDominator;
  }

  return result.first * 3 + result.second;
}

long long solverP1(const std::vector<Machine> &machines) {
  long long result = 0;
  for (const auto machine : machines) {
    long long ans = howToWin(machine);

    result += ans;
  }

  return result;
}

long long howToWinP2(const Machine &machine) {
  const auto &[ax, ay] = machine.btnA;
  const auto &[bx, by] = machine.btnB;
  const auto &[px_, py_] = machine.prize;

  const auto px = px_ + 10000000000000;
  const auto py = py_ + 10000000000000;

  std::pair<long long, long long> result;

  long long nNumerator = px * ay - py * ax;
  long long nDominator = bx * ay - by * ax;

  bool sameSignN =
      (nDominator > 0 && nNumerator > 0) || (nDominator < 0 && nNumerator < 0);

  if (sameSignN && nDominator != 0) {
    if (nNumerator % nDominator) {
      return 0;
    }
    result.second = nNumerator / nDominator;
  }

  long long mNumerator = px * by - py * bx;
  long long mDominator = ax * by - ay * bx;

  bool sameSignM =
      (mDominator > 0 && mNumerator > 0) || (mDominator < 0 && mNumerator < 0);

  if (sameSignM && mDominator != 0) {
    if (mNumerator % mDominator) {
      return 0;
    }
    result.first = mNumerator / mDominator;
  }

  return result.first * 3 + result.second;
}

long long solverP2(const std::vector<Machine> &machines) {
  long long result = 0;
  for (const auto machine : machines) {
    long long ans = howToWinP2(machine);

    result += ans;
  }

  return result;
}

int main(int argc, char *argv[]) {
  const std::string &filePath = argv[1];
  auto input = readInput(filePath);

  // for (const auto &m : input) {
  //   printMachine(m);
  // }

  auto resultP1 = solverP1(input);
  std::cout << "P1 result: " << resultP1 << std::endl;

  auto resultP2 = solverP2(input);
  std::cout << "P2 result: " << resultP2 << std::endl;

  return 0;
}