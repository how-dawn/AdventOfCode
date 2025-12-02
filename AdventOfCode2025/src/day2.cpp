#include <filesystem>
#include <fstream>
#include <iostream>
#include <set>
#include <sstream>
#include <vector>


std::vector<std::string> split(const std::string &s, char delimiter = ',') {
    std::vector<std::string> tokens;
    std::stringstream ss(s);
    std::string item;

    while (std::getline(ss, item, delimiter)) {
        tokens.push_back(item);
    }
    return tokens;
}

std::vector<std::pair<std::string, std::string>> readInput(const std::string &filePath) {
    if (!std::filesystem::exists(filePath)) {
        std::cerr << "File does not exist: " << filePath << std::endl;
        return {};
    }

    std::ifstream inputFile(filePath);
    if (!inputFile) {
        std::cerr << "Error opening file: " << filePath << std::endl;
        return {};
    }

    std::vector<std::pair<std::string, std::string>> input;

    std::string line;

    std::getline(inputFile, line);

    for (const auto &token: split(line)) {
        auto _ids = split(token, '-');
        input.emplace_back(_ids[0], _ids[1]);
    }

    inputFile.close();

    return input;
}

bool isValidID(const std::string &id) {
    if (id.length() % 2 != 0) {
        return true;
    }
    if (id.substr(0, id.length() / 2) == id.substr(id.length() / 2)) {
        return false;
    }
    return true;
}

bool isValidID_P2(const std::string &id) {
    auto ssize = id.length();

    for (auto l = 1; l <= ssize/2; l++) {
        std::vector<std::string> parts;
        for (auto i =0; i < ssize; i+=l) {
            parts.push_back(id.substr(i, l));
        }

        bool repeating = true;
        auto val = parts.front();
        for (const auto &part: parts) {
            if (part!=val) {
                repeating = false;
                break;
            }
        }
        if (repeating) { return false;}
    }

    return true;
}

void getInValidIds(const std::string& start, const std::string& end, std::set<int64_t>& ids, const std::function<bool(const std::string&)>& func) {
    for (int64_t i = std::stoll(start); i <= std::stoll(end); i++) {
        if (auto val = std::to_string(i); !func(val)) {
            ids.insert(i);
        }
    }
}


int64_t solverP1(const std::vector<std::pair<std::string, std::string>>& input) {
    int64_t res = 0LL;

    std::set<int64_t> invalidIDs;
    for (const auto &[start, end] : input) {
         getInValidIds(start, end, invalidIDs,isValidID);
    }

    for (const auto id : invalidIDs) {
        res += id;
    }
    return res;
}

int64_t solverP2(const std::vector<std::pair<std::string, std::string>>& input) {
    int64_t res = 0LL;

    std::set<int64_t> invalidIDs;
    for (const auto &[start, end] : input) {
        getInValidIds(start, end, invalidIDs,isValidID_P2);
    }

    for (const auto id : invalidIDs) {
        res += id;
    }
    return res;
}




int main(int argc, char *argv[]) {
    const std::string filePath = argv[1];

    const auto input = readInput(filePath);

    const auto resultP1 = solverP1(input);
    std::cout << "P1 result: " << resultP1 << std::endl;

    const auto resultP2 = solverP2(input);
    std::cout << "P2 result: " << resultP2 << std::endl;


    return 0;
}
