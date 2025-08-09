#pragma GCC diagnostic ignored "-Wuse-after-free"

#include "tests/debugging.hpp"

#include <memory.h>
#include <stdlib.h>

#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(debugging);

// When run, should trigger AddressSanitizer's "use after free" error; see CONFIG_ASAN=y.
void test_address_sanitizer() {
	char* buf = (char*)malloc(512);

	// Works:
	strcpy(buf, "Hello, world!");
	LOG_INF("buf=%s", buf);

	free(buf);

	// Will trigger error detection:
	strcpy(buf, "Bye, world!");
	LOG_INF("buf=%s", buf);
}

void test_debugging() { test_address_sanitizer(); }
