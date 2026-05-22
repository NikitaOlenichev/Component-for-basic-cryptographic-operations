#include "test_framework.h"

int runAllTests() {
    int failed = 0;

    for (const TestCase& test : testRegistry()) {
        try {
            test.run();
            std::cout << "[PASS] " << test.name << '\n';
        } catch (const std::exception& error) {
            ++failed;
            std::cerr << "[FAIL] " << test.name << '\n'
                      << error.what() << '\n';
        } catch (...) {
            ++failed;
            std::cerr << "[FAIL] " << test.name << '\n'
                      << "Unknown exception" << '\n';
        }
    }

    const int passed = static_cast<int>(testRegistry().size()) - failed;
    std::cout << "\nPassed: " << passed << ", failed: " << failed << '\n';
    return failed == 0 ? 0 : 1;
}

int main() {
    return runAllTests();
}
