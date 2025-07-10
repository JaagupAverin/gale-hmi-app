#include "cpp.hpp"

#include <array>
#include <format>
#include <random>
#include <string>
#include <zephyr/random/random.h>


#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(cpp);

uint32_t get_random_number(uint32_t min_inc, uint32_t max_inc) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<uint32_t> dist(0, UINT32_MAX);
    const uint32_t x = dist(gen);
    return x % (max_inc - min_inc + 1) + min_inc;
}

void bad_function() {
    std::array<int, 3> arr = {1, 2, 3};
    const int x = arr.at(42);
    LOG_INF("Non-existent number: %d", x);
}

void test_stl() {
    static const std::array names = {"June", "Jane", "Jill", "Joe"};

    const uint32_t rand_n = get_random_number(0, names.size() - 1);
    const std::string message = std::format("Hello, {}", names.at(rand_n));
    LOG_INF("%s", message.c_str());
}

void test_exceptions() {
    try {
        bad_function();
    } catch (const std::exception& e) {
        LOG_INF("Bad function threw exception: %s", e.what());
    }
}

void test_cpp() {
    test_stl();
    test_exceptions();
}
