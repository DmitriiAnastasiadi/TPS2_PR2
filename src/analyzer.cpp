#include "analyzer.h"
#include "thread_pool.h"
#include <algorithm>
#include <filesystem>
#include <fstream>
#include <future>
#include <iostream>
#include <mutex>
#include <sstream>

namespace fs = std::filesystem;

namespace analyzer {

static std::string readFile(const std::string& path) {
    std::ifstream in(path, std::ios::binary);
    if (!in) {
        throw std::runtime_error("Cannot open file: " + path);
    }
    std::ostringstream ss;
    ss << in.rdbuf();
    return ss.str();
}

static FileAnalysis analyzeContent(const std::string& filename, const std::string& content) {
    FileAnalysis fa;
    fa.filename = filename;
    fa.characters = content.size();
    std::istringstream iss(content);
    std::string token;
    while (iss >> token) {
        ++fa.words;
    }
    return fa;
}

std::vector<std::string> collectInputFiles(int argc, char** argv) {
    std::vector<std::string> files;
    if (argc > 1) {
        for (int i = 1; i < argc; ++i) {
            files.emplace_back(argv[i]);
        }
        return files;
    }

    const fs::path current = fs::current_path();
    for (auto& entry : fs::directory_iterator(current)) {
        if (entry.is_regular_file() && entry.path().extension() == ".txt") {
            files.push_back(entry.path().string());
        }
    }
    return files;
}

std::vector<FileAnalysis> analyzeFiles(const std::vector<std::string>& files) {
    ThreadPool pool(std::max<size_t>(1, std::thread::hardware_concurrency()));
    std::vector<std::future<std::string>> readFutures;
    readFutures.reserve(files.size());
    for (const auto& file : files) {
        readFutures.emplace_back(std::async(std::launch::async, readFile, file));
    }

    std::vector<FileAnalysis> results;
    std::mutex resultsMutex;
    std::vector<std::future<void>> tasks;
    tasks.reserve(files.size());

    for (size_t i = 0; i < files.size(); ++i) {
        const auto filename = files[i];
        auto& future = readFutures[i];

        tasks.emplace_back(pool.submit([&results, &resultsMutex, filename, &future] {
            FileAnalysis fa;
            fa.filename = filename;
            try {
                const std::string content = future.get();
                fa = analyzeContent(filename, content);
            } catch (const std::exception& e) {
                fa.ok = false;
                fa.error = e.what();
            }
            std::lock_guard<std::mutex> lock(resultsMutex);
            results.push_back(std::move(fa));
        }));
    }

    for (auto& task : tasks) {
        task.get();
    }

    std::sort(results.begin(), results.end(), [](const FileAnalysis& a, const FileAnalysis& b) {
        return a.filename < b.filename;
    });
    return results;
}

void printAnalysisResults(const std::vector<FileAnalysis>& results) {
    size_t totalWords = 0;
    size_t totalChars = 0;
    std::cout << "Результаты анализа:\n";
    for (size_t index = 0; index < results.size(); ++index) {
        const auto& result = results[index];
        if (result.ok) {
            std::cout << index + 1 << ". " << result.filename << ": "
                      << result.words << " слов, " << result.characters << " символов\n";
            totalWords += result.words;
            totalChars += result.characters;
        } else {
            std::cout << index + 1 << ". " << result.filename << ": Ошибка - " << result.error << "\n";
        }
    }
    std::cout << "\nИтог: " << totalWords << " слов, " << totalChars << " символов." << std::endl;
}

void printNoInputFilesMessage() {
    std::cout << "No input files found. Provide file paths as arguments or place .txt files in the current directory.\n";
}

} // namespace analyzer
