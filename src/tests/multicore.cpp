/*
 *  Zephyr has several layers and mechanisms for IPC:
 *   - mbox (basically just an inter-CPU interrupt)
 *   - RPmsg (also allows passing data with basic format for messages)
 *   - OpenAMP (open source standard built on top of rpmsg, adds events, lifecycles, etc)
 *   - (Zephyr also defines "ICMsg backend", which is a lighter alternative to OpenAMP for the IPC service module)
 * 	 - (Zephyr also defines "rpmsg service" on top of openamp, but that appears mostly obsolete)
 *   - IPC service (Zephyr's highest level and most common module for IPC; built on top of OpenAMP or IPMsg)
 * We're only interested in IPC service (with OpenAMP) for now, but others may be required for performance reasons.
 */

#include <stdlib.h>
#include <zephyr/drivers/mbox.h>
#include <zephyr/ipc/ipc_service.h>
#include <zephyr/kernel.h>

#include "gale_ipc.hpp"
// TODO: Getting IPC working with gale_ipc interface.
#include "tests/multicore.hpp"

#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(multicore);

K_THREAD_STACK_DEFINE(ipc_stack, 4096);
static volatile uint8_t ipc_recv_data;
K_SEM_DEFINE(ipc_bound_sem, 0, 1);
K_SEM_DEFINE(ipc_data_sem, 0, 1);

static void ipc_endpoint_bound_cb(void* priv) { k_sem_give(&ipc_bound_sem); }
static void ipc_endpoint_unbound_cb(void* priv) { LOG_ERR("IPC endpoint unbound"); }
static void ipc_endpoint_error_cb(const char* message, void* priv) { LOG_ERR("IPC endpoint error: %s", message); }
static void ipc_endpoint_recv_cb(const void* data, size_t len, void* priv) {
	ipc_recv_data = *((uint8_t*)data);
	k_sem_give(&ipc_data_sem);
}

static ipc_ept_cfg ipc_endpoint_cfg = {
	.name = "ipc_ping_pong",
	.cb =
		{
			.bound = ipc_endpoint_bound_cb,
			.unbound = ipc_endpoint_unbound_cb,
			.received = ipc_endpoint_recv_cb,
			.error = ipc_endpoint_error_cb,
		},
};

static void test_ipc(void*, void*, void*) {
	const device* ipc_instance = DEVICE_DT_GET(DT_NODELABEL(ipc_user));
	unsigned char message = 0;
	ipc_ept ipc_user_ept;

	int ret = ipc_service_open_instance(ipc_instance);
	if (ret < 0 && ret != -EALREADY) {
		LOG_ERR("ipc_service_open_instance() failure");
		return;
	}
	ret = ipc_service_register_endpoint(ipc_instance, &ipc_user_ept, &ipc_endpoint_cfg);
	if (ret < 0) {
		LOG_ERR("ipc_service_register_endpoint() failure");
		return;
	}
	LOG_INF("Waiting for IPC endpoint to be bound");
	k_sem_take(&ipc_bound_sem, K_FOREVER);

	while (true) {
		ret = ipc_service_send(&ipc_user_ept, &message, sizeof(message));
		if (ret < 0) {
			LOG_ERR("send_message(%d) failed with ret %d", message, ret);
			break;
		}

		k_sem_take(&ipc_data_sem, K_FOREVER);
		message = ipc_recv_data;

		LOG_INF("HOST [1]: %d", message);
		message++;
	}

	LOG_INF("IPC-service HOST [INST 1] demo ended.");
}
K_THREAD_DEFINE(test_ipc_thread, 4096, test_ipc, NULL, NULL, NULL, 0, 0, K_FOREVER.ticks);

void test_multicore() { k_thread_start(test_ipc_thread); }
