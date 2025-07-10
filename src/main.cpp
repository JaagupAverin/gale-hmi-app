#include <app_version.h>
#include <zephyr/kernel.h>

#include "tests/cpp.hpp"
#include "tests/storage.hpp"

#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(main, LOG_LEVEL_DBG);

int main() {
    LOG_INF("Hello, world! " APP_VERSION_STRING);

    test_storage();
    test_cpp();

    while (true) {
        LOG_INF("TICK!");
        k_sleep(K_MSEC(10000));
    }
    return 0;
}
