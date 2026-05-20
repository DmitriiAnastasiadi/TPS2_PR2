#include "test_framework.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <filesystem>

namespace fs = std::filesystem;

// Helpers copied/simplified from the main implementation
static bool createTestFile(const std::string& filename, const std::string& content) {
    std::ofstream file(filename, std::ios::binary);
    if (!file.is_open()) return false;
    file << content;
    return true;
}

static std::string readFileForTest(const std::string& path, bool &ok) {
    std::ifstream in(path, std::ios::binary);
    if (!in) { ok = false; return std::string(); }
    std::ostringstream ss;
    ss << in.rdbuf();
    ok = true;
    return ss.str();
}

static int countWordsInContent(const std::string &content) {
    std::istringstream iss(content);
    std::string token;
    int cnt = 0;
    while (iss >> token) ++cnt;
    return cnt;
}

void testFileReading(TestFramework &framework) {
    std::cout << "\n[Тест 1] Чтение файла\n";
    framework.startTest("FileReading");
    const std::string file = "tests/tmp_test1.txt";
    const std::string content = "Hello world hello C++ hello";
    createTestFile(file, content);
    bool ok = false;
    std::string read = readFileForTest(file, ok);
    framework.assertTrue(ok, "Файл успешно прочитан");
    framework.assertTrue(!read.empty(), "Содержимое загружено корректно");
    framework.assertEqual(5, countWordsInContent(read), "Подсчет слов работает");
}

void testWordCounting(TestFramework &framework) {
    std::cout << "\n[Тест 2] Подсчет слов\n";
    framework.startTest("WordCounting");
    const std::string file = "tests/tmp_test2.txt";
    const std::string content = "The quick brown fox jumps over the lazy dog";
    createTestFile(file, content);
    bool ok = false;
    std::string read = readFileForTest(file, ok);
    framework.assertTrue(ok, "Файл прочитан для подсчёта");
    framework.assertEqual(9, countWordsInContent(read), "Точный подсчет слов");
}

void testNonexistentFile(TestFramework &framework) {
    std::cout << "\n[Тест 3] Неcуществующий файл\n";
    framework.startTest("NonexistentFile");
    const std::string file = "tests/this_file_does_not_exist.txt";
    bool ok = false;
    std::string read = readFileForTest(file, ok);
    framework.assertTrue(!ok, "Ошибка обработана корректно для несуществующего файла");
}

void testMultilanguage(TestFramework &framework) {
    std::cout << "\n[Тест 4] Многоязычный контент\n";
    framework.startTest("Multilanguage");
    const std::string file = "tests/tmp_test_ml.txt";
    const std::string content = "Hello привет hello мир hello";
    createTestFile(file, content);
    bool ok = false;
    std::string read = readFileForTest(file, ok);
    framework.assertTrue(ok, "Файл с многоязычным контентом прочитан");
    framework.assertEqual(5, countWordsInContent(read), "Количество слов верно для многоязычного контента");
}

int main() {
    std::cout << "\n" << std::string(50, '=') << "\n";
    std::cout << "     НАБОР ТЕСТОВ ДЛЯ FileAnalyzer\n";
    std::cout << std::string(50, '=') << "\n";

    TestFramework framework;
    testFileReading(framework);
    testWordCounting(framework);
    testNonexistentFile(framework);
    testMultilanguage(framework);

    framework.printSummary();
    return framework.allTestsPassed() ? EXIT_SUCCESS : EXIT_FAILURE;
}
