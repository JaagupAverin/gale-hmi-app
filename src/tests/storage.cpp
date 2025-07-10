#include "storage.hpp"

#include <format>
#include <zephyr/fs/fs.h>
#include <zephyr/fs/littlefs.h>
#include <zephyr/storage/flash_map.h>

#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(storage);

// LittleFS is automatically mounted as defined by the 'fstab' devicetree node.
#define PARTITION_NODE DT_NODELABEL(lfs1)
FS_FSTAB_DECLARE_ENTRY(PARTITION_NODE);
struct fs_mount_t* mountpoint = &FS_FSTAB_ENTRY(PARTITION_NODE);

void test_littlefs_boot_count() {
    const std::string filename_str = std::format("{}/boot_count.txt", mountpoint->mnt_point);
    auto filename = filename_str.c_str();

    fs_file_t file;
    fs_file_t_init(&file);

    int rc = fs_open(&file, filename, FS_O_CREATE | FS_O_RDWR);
    if (rc < 0) {
        LOG_ERR("FAIL: open %s: %d", filename, rc);
        return;
    }

    auto cleanup = [&]() -> void {
        int ret = fs_close(&file);
        if (ret < 0) {
            LOG_ERR("FAIL: close %s: %d", filename, ret);
        }
    };

    fs_dirent dirent;
    rc = fs_stat(filename, &dirent);
    if (rc < 0) {
        LOG_ERR("FAIL: stat %s: %d", filename, rc);
        cleanup();
        return;
    }

    // Read or initialize boot count:
    uint32_t boot_count = 0;
    if (dirent.type == FS_DIR_ENTRY_FILE && dirent.size != 0) {
        // File exists:
        rc = fs_read(&file, &boot_count, sizeof(boot_count));
        if (rc < 0) {
            LOG_ERR("FAIL: read %s: %d", filename, rc);
            cleanup();
            return;
        }
    }

    // Increment and write back:
    LOG_INF("Boot count: %u", ++boot_count);

    rc = fs_seek(&file, 0, FS_SEEK_SET);
    if (rc < 0) {
        LOG_ERR("FAIL: seek %s: %d", filename, rc);
        cleanup();
        return;
    }

    rc = fs_write(&file, &boot_count, sizeof(boot_count));
    if (rc < 0) {
        LOG_ERR("FAIL: write %s: %d", filename, rc);
        cleanup();
        return;
    }

    // Success!
    cleanup();
}

void test_storage() { test_littlefs_boot_count(); }
