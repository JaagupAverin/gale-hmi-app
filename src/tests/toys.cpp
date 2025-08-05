#include "tests/toys.hpp"
#include "cpp_shell_args.hpp"

#include <zephyr/bindesc.h>
#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(toys);

// TODO: Test symtab on actual board (doesn't compile on native_sim):
#ifdef CONFIG_SYMTAB
#include <zephyr/debug/symtab.h>
#endif

enum binary_descriptor_id {
	HELLO_WORLD = 0,
	PROGRAM_NAME = 1,
};

BINDESC_STR_DEFINE(my_string, binary_descriptor_id::HELLO_WORLD, "Hello world!");
BINDESC_STR_DEFINE(program_name, binary_descriptor_id::PROGRAM_NAME, "My Program");

void test_binary_descriptors() {
	LOG_INF("My own binary data: %s", BINDESC_GET_STR(my_string));
	LOG_INF("CONFIG_BINDESC_BUILD_DATE_TIME_STRING: %s", BINDESC_GET_STR(build_date_time_string));
	LOG_INF("CONFIG_BINDESC_C_COMPILER_VERSION: %s", BINDESC_GET_STR(c_compiler_version));
	LOG_INF("CONFIG_BINDESC_APP_BUILD_VERSION: %s", BINDESC_GET_STR(app_build_version));
	LOG_INF("CONFIG_BINDESC_DATE_TIME_STRING: %s", BINDESC_GET_STR(build_date_time_string));
	LOG_INF("CONFIG_BINDESC_HOST_NAME: %s", BINDESC_GET_STR(host_name));
}

void test_symtab() {
#ifdef CONFIG_SYMTAB
	const char* fn_name = symtab_find_symbol_name(reinterpret_cast<uintptr_t>(test_binary_descriptors), 0);
	LOG_INF("fn @%p is named %s:", reinterpret_cast<void*>(test_binary_descriptors), fn_name);
#else
	LOG_INF("Skipping test_symtab");
#endif
}

void test_toys() {
	test_binary_descriptors();
	test_symtab();
}

static int cmd_hello(const struct shell* sh, size_t argc, char** argv) {
	auto first_name = StringArg{"first", "First name of person to greet"};
	auto last_name = StringArg{"last", "Last name of person to greet"};
	auto polite = FlagArg{"polite", "Use polite form of greeting over casual"};
	auto height_m = FloatArg{"height", "Height of person to greet, in meters", 0};
	auto args = Args("Print hello to the user", {&first_name, &last_name, &polite, &height_m});

	if (!args.parse_all(sh, argc, argv)) {
		shell_print(sh, "Error: Invalid arguments");
		return EINVAL;
	}
	if (polite.get()) {
		shell_print(sh, "Hello, Mr./Mrs. %s %s!", first_name.get(), last_name.get());
	} else {
		if (height_m.get() == 0.f) {
			shell_print(sh, "Yo %s %s!", first_name.get(), last_name.get());
		} else if (height_m.get() >= 1.8f) {
			shell_print(sh, "Yo %s %s, you're huge!", first_name.get(), last_name.get());
		} else {
			shell_print(sh, "Yo %s %s, you're tiny!", first_name.get(), last_name.get());
		}
	}
	return 0;
}

SHELL_STATIC_SUBCMD_SET_CREATE(app_cmds, SHELL_CMD(hello, NULL, "Say hello", cmd_hello), SHELL_SUBCMD_SET_END);

SHELL_CMD_REGISTER(app, &app_cmds, "Application-defined commands", NULL);
