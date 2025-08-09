#include <stdlib.h>
#include <zephyr/drivers/mbox.h>
#include <zephyr/kernel.h>

#include "tests/multicore.hpp"

#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(multicore);

static K_SEM_DEFINE(mbox_rx_sem, 0, 1);

static void mbox_cb(const device* dev, mbox_channel_id_t channel_id, void* user_data, mbox_msg* data) {
	k_sem_give(&mbox_rx_sem);
}

static void test_mbox() {
	const mbox_dt_spec rx_channel = MBOX_DT_SPEC_GET(DT_PATH(mbox_consumer), rx);

	if (mbox_register_callback_dt(&rx_channel, mbox_cb, nullptr)) {
		LOG_ERR("mbox_register_callback() error");
		return;
	}

	if (mbox_set_enabled_dt(&rx_channel, true)) {
		LOG_ERR("mbox_set_enable() error");
		return;
	}

	while (true) {
		// RX:
		if (k_sem_take(&mbox_rx_sem, K_SECONDS(10)) != 0) {
			LOG_ERR("k_sem_take() error");
			return;
		}
		LOG_INF("Client received");
	}
}
K_THREAD_DEFINE(test_mbox_thread, 1024, test_mbox, NULL, NULL, NULL, 7, 0, K_FOREVER.ticks);

void test_multicore() { k_thread_start(test_mbox_thread); }
