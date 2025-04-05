#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(main, LOG_LEVEL_DBG);

int main() {
  while (true) {
    LOG_INF("Hello from HMI.");
    k_sleep(K_MSEC(1000));
  }
  return 0;
}
