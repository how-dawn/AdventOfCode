#include <filesystem>
#include <fstream>
#include <iostream>
#include <regex>
#include <string>
#include <vector>

std::string readInput(const std::string &filePath) {
  if (!std::filesystem::exists(filePath)) {
    std::cerr << "File does not exist: " << filePath << std::endl;
    return "";
  }

  std::ifstream inputFile(filePath);
  if (!inputFile) {
    std::cerr << "Error opening file: " << filePath << std::endl;
    return "";
  }

  std::string input;
  std::string line;
  while (std::getline(inputFile, line)) {
    input += line;
  }

  inputFile.close();

  return input;
}

std::vector<std::string> preprocessor(const std::string &input,
                                      const std::string &pattern) {
  std::vector<std::string> result;

  std::regex pattern_(pattern);

  auto begin = std::sregex_iterator(input.begin(), input.end(), pattern_);
  auto end = std::sregex_iterator();

  for (auto it = begin; it != end; ++it) {
    std::cout << "preprocessor match: " << it->str() << std::endl;
    result.push_back(it->str());
  }

  return result;
}

long long calculate(const std::string &operation, const std::string &pattern) {
  //   std::regex pattern(R"(mul\((\d{1,3}),(\d{1,3})\))");
  std::regex pattern_(pattern);

  std::smatch matches;
  long long v1 = 0LL, v2 = 0LL;
  if (std::regex_match(operation, matches, pattern_)) {
    v1 = std::stoi(matches[1].str());
    v2 = std::stoi(matches[2].str());

    // std::cout << "match: " << v1 << " * " << v2 << std::endl;
  }

  return v1 * v2;
}

long long solverP1(const std::string &input) {
  long long result = 0LL;
  std::string pattern = R"(mul\((\d{1,3}),(\d{1,3})\))";
  const auto ops = preprocessor(input, pattern);

  for (const auto op : ops) {
    result += calculate(op, pattern);
    // std::cout << result << std::endl;
  }

  return result;
}

long long solverP2(const std::string &input) {
  long long result = 0LL;

  std::string pattern = R"(mul\((\d{1,3}),(\d{1,3})\)|do\(\)|don't\(\))";
  const auto ops = preprocessor(input, pattern);

  bool enable = true;
  std::string pattern2 = R"(mul\((\d{1,3}),(\d{1,3})\))";
  for (const auto op : ops) {
    if (op == "don't()") {
      enable = false;
      continue;
    }

    if (op == "do()") {
      enable = true;
      continue;
    }

    if (enable) {
      result += calculate(op, pattern2);
    }
  }

  return result;
}

int main(int argc, char *argv[]) {
  std::string filePath = argv[1];

  auto input = readInput(filePath);
  std::cout << "Input: " << input << std::endl;

  long long resultP1 = solverP1(input);
  std::cout << "P1 result: " << resultP1 << std::endl;

  long long resultP2 = solverP2(input);
  std::cout << "P2 result: " << resultP2 << std::endl;

  return 0;
}