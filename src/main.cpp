#include <app_version.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/storage/flash_map.h>

LOG_MODULE_REGISTER(main, LOG_LEVEL_DBG);

int flash_test() {
    const struct flash_area *storage_area;
    int err = flash_area_open(FIXED_PARTITION_ID(storage_partition), &storage_area);

    if (err != 0) {
        LOG_ERR("Could not open storage partition");
        return EINVAL;
    }

    uint8_t buf[512];
    err = flash_area_read(storage_area, 0, buf, sizeof(buf));
    if (err != 0) {
        LOG_ERR("Could not read storage partition");
        return EINVAL;
    }

    LOG_HEXDUMP_INF(buf, sizeof(buf), "Storage partition data loaded");

    // Increment all bytes by 1:
    for (int i = 0; i < sizeof(buf); i++) {
        buf[i]++;
    }

    // Write the modified data back to the storage partition
    err = flash_area_write(storage_area, 0, buf, sizeof(buf));
    if (err != 0) {
        LOG_ERR("Could not write storage partition: %d", err);
        return EINVAL;
    }

    // TODO: Works nicely, but probably reached the end of QEMU limits since it can't have persistent storage. Let's try
    // moving to native-sim.

    return 0;
}

int main() {
    LOG_INF("Hello from HMI; " APP_VERSION_STRING);

    flash_test();

    while (true) {
        LOG_INF("TICK!");
        k_sleep(K_MSEC(1000));
    }
    return 0;
}
