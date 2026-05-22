#ifndef TEST_FRAMEWORK_H
#define TEST_FRAMEWORK_H

#include <functional>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

struct TestCase {
    std::string name;
    std::function<void()> run;
};

inline std::vector<TestCase>& testRegistry() {
    static std::vector<TestCase> tests;
    return tests;
}

struct TestRegistrar {
    TestRegistrar(const std::string& name, std::function<void()> run) {
        testRegistry().push_back({name, run});
    }
};

#define TEST_CONCAT_IMPL(a, b) a##b
#define TEST_CONCAT(a, b) TEST_CONCAT_IMPL(a, b)

#define TEST_CASE(name) \
    static void TEST_CONCAT(testFunction_, __LINE__)(); \
    static TestRegistrar TEST_CONCAT(testRegistrar_, __LINE__)(name, TEST_CONCAT(testFunction_, __LINE__)); \
    static void TEST_CONCAT(testFunction_, __LINE__)()

#define CHECK(condition) \
    do { \
        if (!(condition)) { \
            std::ostringstream message; \
            message << "CHECK failed: " << #condition << " at " << __FILE__ << ":" << __LINE__; \
            throw std::runtime_error(message.str()); \
        } \
    } while (false)

#define CHECK_EQUAL(actual, expected) \
    do { \
        const auto actualValue = (actual); \
        const auto expectedValue = (expected); \
        if (!(actualValue == expectedValue)) { \
            std::ostringstream message; \
            message << "CHECK_EQUAL failed at " << __FILE__ << ":" << __LINE__ \
                    << "\n  actual:   " << actualValue \
                    << "\n  expected: " << expectedValue; \
            throw std::runtime_error(message.str()); \
        } \
    } while (false)

int runAllTests();

#endif
