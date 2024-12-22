#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <numeric>
#include <map>
#include <set>
#include <thread>


std::vector<int64_t> readInput(const std::string &filePath) {
    if (!std::filesystem::exists(filePath)) {
        std::cerr << "File does not exist: " << filePath << std::endl;
        return {};
    }

    std::ifstream inputFile(filePath);
    if (!inputFile) {
        std::cerr << "Error opening file: " << filePath << std::endl;
        return {};
    }

    std::vector<int64_t> input;
    std::string line;
    while (std::getline(inputFile, line)) {
        input.push_back(std::stoll(line));
    }

    inputFile.close();

    return input;
}

int64_t mix(const int64_t &secret_number, const int64_t &num) {
    return secret_number ^ num;
}

int64_t prune(const int64_t &secret_number) {
    return secret_number % 16777216;
}

int64_t process(const int64_t &secret_number) {
    auto result = mix(secret_number, secret_number * 64);
    result = prune(result);

    result = mix(result, result / 32);
    result = prune(result);

    result = prune(mix(result, result * 2048));

    return result;
}

int64_t solverP1(const std::vector<int64_t> &inputData, const int steps) {
    std::vector<int64_t> result;
    for (const auto &val_: inputData) {
        auto val = val_;
        for (int i = 0; i < steps; i++) {
            val = process(val);
        }
        result.push_back(val);
    }

    return std::accumulate(result.begin(), result.end(), 0LL);
}

std::vector<int64_t> seqDiff(std::vector<int64_t> seq) {
    std::vector<int64_t> diff;
    for (auto it = seq.begin() + 1; it != seq.end(); it++) {
        auto it_last = it - 1;
        int64_t val = *it - *it_last;
        diff.push_back(val);
    }
    return diff;
}

int KMP(const std::vector<int64_t> &seq, const std::vector<int64_t> &pattern) {
    std::vector<int> pos;

    const auto getNext = [&](const std::vector<int64_t> &pattern) {
        const auto len = static_cast<int>(pattern.size());

        std::vector<int> next(len);

        int j = -1;
        next[0] = -1;

        for (int i = 1; i < len; i++) {
            while (j != -1 && pattern[i] != pattern[j + 1]) {
                j = next[j];
            }
            if (pattern[i] == pattern[j + 1]) {
                j++;
            }
            next[i] = j;
        }

        return next;
    };

    auto next = getNext(pattern);
    const int n = static_cast<int>(seq.size());
    const int m = static_cast<int>(pattern.size());

    int j = -1;
    for (int i = 0; i < n; i++) {
        while (j != -1 && seq[i] != pattern[j + 1]) {
            j = next[j];
        }
        if (seq[i] == pattern[j + 1]) {
            j++;
        }
        if (j == m - 1) {
            return i;
        }
    }

    return -1;
}

std::set<std::vector<int64_t>> getPatterns(const std::vector<std::vector<int64_t>> &diffs,
                                           const int patternLen) {
    std::set<std::vector<int64_t>> patterns;

    for (const auto &diff: diffs) {
        for (auto i = 0; i + patternLen < diff.size(); i++) {
            std::vector<int64_t> pattern(diff.begin() + i, diff.begin() + i + patternLen);
            patterns.insert(pattern);
        }
    }

    return patterns;
}

int64_t getMostBanana(const std::vector<std::vector<int64_t>> &seqs, const std::vector<std::vector<int64_t>> &diffs,
                      const int patternLen, const size_t &nThreads = 12) {
    const auto patterns = getPatterns(diffs, patternLen);

    int64_t result = std::numeric_limits<int64_t>::min();
    size_t numPatterns = patterns.size();
    size_t numThreads = std::min(numPatterns, nThreads);

    auto processPatternRange = [&seqs, &diffs, &patterns](int startIdx, int endIdx, int64_t &localMax) {
        for (int i = startIdx; i < endIdx; ++i) {
            const auto &pattern = *(std::next(patterns.begin(), i));
            int64_t sum = 0;
            for (size_t j = 0; j < diffs.size(); ++j) {
                const auto &diff_ = diffs[j];
                int matchPos = KMP(diff_, pattern);
                int64_t val_ = matchPos != -1 ? seqs[j][matchPos + 1] : 0;
                sum += val_;
            }
            localMax = std::max(localMax, sum);
        }
    };

    size_t patternsPerThread = (numPatterns + numThreads - 1) / numThreads;
    std::vector<std::thread> threads;
    std::vector<int64_t> threadResults(numThreads, std::numeric_limits<int64_t>::min());

    for (size_t i = 0; i < numThreads; ++i) {
        size_t startIdx = i * patternsPerThread;
        size_t endIdx = std::min(startIdx + patternsPerThread, numPatterns);
        threads.emplace_back(processPatternRange, startIdx, endIdx, std::ref(threadResults[i]));
    }

    for (auto &t: threads) {
        t.join();
    }

    return *std::max_element(threadResults.begin(), threadResults.end());
}


int64_t solverP2(const std::vector<int64_t> &inputData, const int steps) {
    std::vector<std::vector<int64_t>> seqs;
    for (const auto &val_: inputData) {
        auto val = val_;
        std::vector<int64_t> seq;
        seq.push_back(val % 10);
        for (int i = 0; i < steps; i++) {
            val = process(val);
            seq.push_back(val % 10);
        }
        seqs.push_back(seq);
    }

    std::vector<std::vector<int64_t>> diffs;
    diffs.reserve(seqs.size());
    for (const auto &seq: seqs) {
        diffs.push_back(seqDiff(seq));
    }

    std::map<std::vector<int64_t>, int64_t> memo;
    int64_t result = getMostBanana(seqs, diffs, 4, 32);
    return result;
}


int main(int argc, char *argv[]) {
    const std::string &filePath = argv[1];
    auto input = readInput(filePath);

    auto resultP1 = solverP1(input, 2000);
    std::cout << "P1 result: " << resultP1 << std::endl;

    auto resultP2 = solverP2(input, 2000);
    std::cout << "P2 result: " << resultP2 << std::endl;

    return 0;
}