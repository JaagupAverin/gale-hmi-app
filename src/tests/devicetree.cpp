#include "tests/devicetree.hpp"

#include <ranges>
#include <zephyr/drivers/gpio.h>

#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(devicetree);

template <typename Num> auto range(Num min, Num max) { return std::ranges::views::iota(min, max); }

void test_user_gpio() {
    static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(DT_PATH(zephyr_user), led_gpios);

    bool on = false;
    for (int _ : range(0, 10)) {
        LOG_INF("%s", on ? "On" : "Off");
        gpio_pin_set_dt(&led, on);
        k_sleep(K_MSEC(100));
        on = !on;
    }
}

void test_devicetree() { test_user_gpio(); }
