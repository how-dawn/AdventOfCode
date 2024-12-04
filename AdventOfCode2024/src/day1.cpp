#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <map>
#include <numeric>
#include <sstream>
#include <vector>

struct SearchList {
  std::vector<int> left;
  std::vector<int> right;
};

SearchList readInput(const std::string &filePath) {
  SearchList sList;

  if (!std::filesystem::exists(filePath)) {
    std::cerr << "File does not exist: " << filePath << std::endl;
    return sList;
  }

  std::ifstream inputFile(filePath);
  if (!inputFile) {
    std::cerr << "Error opening file: " << filePath << std::endl;
    return sList;
  }

  std::string line;
  int num1, num2;

  while (std::getline(inputFile, line)) {
    std::istringstream iss(line);
    if (iss >> num1 >> num2) {
      sList.left.push_back(num1);
      sList.right.push_back(num2);
    } else {
      std::cerr << "Invalid line format: " << line << std::endl;
    }
  }

  inputFile.close();

  std::sort(sList.left.begin(), sList.left.end());
  std::sort(sList.right.begin(), sList.right.end());

  return sList;
}

int solverP1(const SearchList &sList) {
  const auto &left = sList.left;
  const auto &right = sList.right;

  std::vector<int> pairList(left.size());

  std::transform(left.begin(), left.end(), right.begin(), pairList.begin(),
                 [](const int v1, const int v2) { return std::abs(v1 - v2); });

  return std::reduce(pairList.begin(), pairList.end());
}

int solverP2(const SearchList &sList) {
  const auto &left = sList.left;
  const auto &right = sList.right;

  std::map<int, int> rightMap;

  for (const auto &rVal : right) {
    rightMap[rVal]++;
  }

  std::vector<int> pairList(left.size());

  std::transform(left.begin(), left.end(), pairList.begin(),
                 [&rightMap](const int v1) { return v1 * rightMap[v1]; });

  return std::reduce(pairList.begin(), pairList.end());
}

int main(int argc, char *argv[]) {
  std::string filePath = argv[1];

  auto sList = readInput(filePath);

  int resultP1 = solverP1(sList);
  std::cout << "P1 result: " << resultP1 << std::endl;

  int resultP2 = solverP2(sList);
  std::cout << "P2 result: " << resultP2 << std::endl;

  return 0;
}