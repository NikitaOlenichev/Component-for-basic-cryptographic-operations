#ifndef TESTS_GTEST_GTEST_H
#define TESTS_GTEST_GTEST_H

#include <functional>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

namespace testing {

struct TestInfo {
    std::string suite;
    std::string name;
    std::function<void()> run;
};

struct TestFailure {
    std::string file;
    int line;
    std::string message;
};

inline std::vector<TestInfo>& registry() {
    static std::vector<TestInfo> tests;
    return tests;
}

inline std::vector<TestFailure>& currentFailures() {
    static std::vector<TestFailure> failures;
    return failures;
}

inline int& totalFailedExpectations() {
    static int count = 0;
    return count;
}

inline int InitGoogleTest(int*, char**) {
    return 0;
}

class TestRegistrar {
public:
    TestRegistrar(const std::string& suite, const std::string& name, std::function<void()> run) {
        registry().push_back({suite, name, run});
    }
};

inline void addFailure(const char* file, int line, const std::string& message) {
    currentFailures().push_back({file, line, message});
    ++totalFailedExpectations();
}

inline int RunAllTests() {
    int failedTests = 0;

    std::cout << "[==========] Running " << registry().size() << " tests.\n";
    for (const TestInfo& test : registry()) {
        currentFailures().clear();
        std::cout << "[ RUN      ] " << test.suite << "." << test.name << '\n';

        try {
            test.run();
        } catch (const std::exception& error) {
            addFailure(__FILE__, __LINE__, std::string("Unhandled exception: ") + error.what());
        } catch (...) {
            addFailure(__FILE__, __LINE__, "Unhandled unknown exception");
        }

        if (currentFailures().empty()) {
            std::cout << "[       OK ] " << test.suite << "." << test.name << '\n';
        } else {
            ++failedTests;
            std::cout << "[  FAILED  ] " << test.suite << "." << test.name << '\n';
            for (const TestFailure& failure : currentFailures()) {
                std::cout << failure.file << ":" << failure.line << ": Failure\n"
                          << failure.message << '\n';
            }
        }
    }

    const int passedTests = static_cast<int>(registry().size()) - failedTests;
    std::cout << "[==========] " << registry().size() << " tests ran.\n";
    std::cout << "[  PASSED  ] " << passedTests << " tests.\n";
    if (failedTests > 0) {
        std::cout << "[  FAILED  ] " << failedTests << " tests.\n";
    }

    return failedTests == 0 ? 0 : 1;
}

} // namespace testing

#define GTEST_CONCAT_IMPL(a, b) a##b
#define GTEST_CONCAT(a, b) GTEST_CONCAT_IMPL(a, b)

#define TEST(test_suite_name, test_name) \
    static void GTEST_CONCAT(test_suite_name, GTEST_CONCAT(_, test_name))(); \
    static ::testing::TestRegistrar GTEST_CONCAT(test_suite_name, GTEST_CONCAT(_, GTEST_CONCAT(test_name, _registrar)))( \
        #test_suite_name, #test_name, GTEST_CONCAT(test_suite_name, GTEST_CONCAT(_, test_name))); \
    static void GTEST_CONCAT(test_suite_name, GTEST_CONCAT(_, test_name))()

#define EXPECT_TRUE(condition) \
    do { \
        if (!(condition)) { \
            ::testing::addFailure(__FILE__, __LINE__, std::string("Expected true: ") + #condition); \
        } \
    } while (false)

#define EXPECT_FALSE(condition) \
    do { \
        if (condition) { \
            ::testing::addFailure(__FILE__, __LINE__, std::string("Expected false: ") + #condition); \
        } \
    } while (false)

#define EXPECT_EQ(expected, actual) \
    do { \
        const auto& gtestExpected = (expected); \
        const auto& gtestActual = (actual); \
        if (!(gtestExpected == gtestActual)) { \
            std::ostringstream gtestMessage; \
            gtestMessage << "Expected equality of these values:\n  " \
                         << #expected << "\n  " << #actual; \
            ::testing::addFailure(__FILE__, __LINE__, gtestMessage.str()); \
        } \
    } while (false)

#define EXPECT_NE(expected, actual) \
    do { \
        const auto& gtestExpected = (expected); \
        const auto& gtestActual = (actual); \
        if (!(gtestExpected != gtestActual)) { \
            std::ostringstream gtestMessage; \
            gtestMessage << "Expected inequality of these values:\n  " \
                         << #expected << "\n  " << #actual; \
            ::testing::addFailure(__FILE__, __LINE__, gtestMessage.str()); \
        } \
    } while (false)

#define RUN_ALL_TESTS() ::testing::RunAllTests()

#endif
