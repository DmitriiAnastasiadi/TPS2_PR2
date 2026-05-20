#include "analyzer.h"

int main(int argc, char** argv) {
    const auto files = analyzer::collectInputFiles(argc, argv);
    if (files.empty()) {
        analyzer::printNoInputFilesMessage();
        return 0;
    }

    const auto results = analyzer::analyzeFiles(files);
    analyzer::printAnalysisResults(results);
    return 0;
}
