#include "tests/toys.hpp"

#include <zephyr/bindesc.h>

#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(toys);

enum binary_descriptor_id {
    HELLO_WORLD = 0,
    PROGRAM_NAME = 1,
};

BINDESC_STR_DEFINE(my_string, binary_descriptor_id::HELLO_WORLD, "Hello world!");
BINDESC_STR_DEFINE(program_name, binary_descriptor_id::PROGRAM_NAME, "My Program");

void test_binary_descriptors() { LOG_INF(": %s\n", BINDESC_GET_STR(my_string)); }

void test_toys() {
    LOG_INF("My own binary data: %s", BINDESC_GET_STR(my_string));
    LOG_INF("CONFIG_BINDESC_BUILD_DATE_TIME_STRING: %s", BINDESC_GET_STR(build_date_time_string));
    LOG_INF("CONFIG_BINDESC_C_COMPILER_VERSION: %s", BINDESC_GET_STR(c_compiler_version));
    LOG_INF("CONFIG_BINDESC_APP_BUILD_VERSION: %s", BINDESC_GET_STR(app_build_version));
    LOG_INF("CONFIG_BINDESC_DATE_TIME_STRING: %s", BINDESC_GET_STR(build_date_time_string));
    LOG_INF("CONFIG_BINDESC_HOST_NAME: %s", BINDESC_GET_STR(host_name));
}
