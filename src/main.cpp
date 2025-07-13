#include <app_version.h>
#include <zephyr/kernel.h>

#include "tests/cplusplus.hpp"
#include "tests/devicetree.hpp"
#include "tests/storage.hpp"
#include "tests/toys.hpp"

#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(main, LOG_LEVEL_DBG);

int main() {
    LOG_INF("Hello, world?! " APP_VERSION_STRING);

    test_storage();
    test_cpp();
    test_devicetree();
    test_toys();

    int i = 0;
    while (true) {
        LOG_INF("TICK #%d!", i++);
        k_sleep(K_MSEC(10000));
    }
    return 0;
}
