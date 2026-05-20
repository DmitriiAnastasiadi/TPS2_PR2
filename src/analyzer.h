#pragma once

#include <string>
#include <vector>

namespace analyzer {

struct FileAnalysis {
    std::string filename;
    size_t words = 0;
    size_t characters = 0;
    bool ok = true;
    std::string error;
};

std::vector<std::string> collectInputFiles(int argc, char** argv);
std::vector<FileAnalysis> analyzeFiles(const std::vector<std::string>& files);
void printAnalysisResults(const std::vector<FileAnalysis>& results);
void printNoInputFilesMessage();

} // namespace analyzer
