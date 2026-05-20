#pragma once

#include <iostream>
#include <string>

class TestFramework {
private:
    int passedTests = 0;
    int failedTests = 0;
    std::string currentTestName;

public:
    void startTest(const std::string& name) {
        currentTestName = name;
    }

    void assertTrue(bool condition, const std::string& message) {
        if (condition) {
            passedTests++;
            std::cout << "  ✓ " << message << "\n";
        } else {
            failedTests++;
            std::cout << "  ✗ " << message << "\n";
        }
    }

    void assertEqual(int expected, int actual, const std::string& message) {
        if (expected == actual) {
            passedTests++;
            std::cout << "  ✓ " << message << " (получено: " << actual << ")\n";
        } else {
            failedTests++;
            std::cout << "  ✗ " << message << " (ожидалось: " << expected 
                      << ", получено: " << actual << ")\n";
        }
    }

    void printSummary() {
        std::cout << "\n" << std::string(50, '=') << "\n";
        std::cout << "РЕЗУЛЬТАТЫ ТЕСТИРОВАНИЯ\n";
        std::cout << std::string(50, '=') << "\n";
        std::cout << "Пройдено тестов: " << passedTests << "\n";
        std::cout << "Провалено тестов: " << failedTests << "\n";
        std::cout << "Всего тестов: " << (passedTests + failedTests) << "\n";
        if (failedTests == 0) {
            std::cout << "Статус: ✓ ВСЕ ТЕСТЫ ПРОЙДЕНЫ\n";
        } else {
            std::cout << "Статус: ✗ НЕКОТОРЫЕ ТЕСТЫ НЕ ПРОЙДЕНЫ\n";
        }
        std::cout << std::string(50, '=') << "\n";
    }

    bool allTestsPassed() const {
        return failedTests == 0;
    }
};
