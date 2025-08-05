/*
 * A QoL wrapper around getopt for parsing shell arguments.
 *
 * Features:
 *  - Uses both getopt and getopt_long to parse command-line arguments;
 *  - Automatically generates a help message from arguments and prints it out when parsing fails;
 *  - Provides automatic conversion for common types;
 *
 * Usage:
 *  - For short arguments use '-fArg' or '-f Arg' (space or no space) or '-f=Arg'
 *  - For long arguments use only '--file=Arg'
 *
 * Issues:
 *  - When running in Linux native_sim, the getopt_long sometimes crashes if used with invalid
 * argument several times in a row - probably due to some internal state issue- however it doesn't
 * support the 'optreset' flag either so we can't do much about if for now. Very nice bug -
 * whatever.
 */

#include <getopt.h>
#include <optional>
#include <string>
#include <vector>
#include <zephyr/shell/shell.h>

struct Arg {
	Arg(const char* name, const char* help, std::optional<std::string> default_value, bool has_arg)
		: long_name(name), help(help), default_value(default_value), short_name(name[0]), has_arg(has_arg) {}

	const char* long_name;						// Full name of the argument, e.g. "file"
	const char* help;						  // Human-readable description of the argument
	std::optional<std::string> default_value; // Default value for argument; if not given, the argument is required
	const char short_name;					  // Defaults to the first character of the long name, e.g. 'f'
	bool has_arg;							  // True for non-flag arguments, e.g. '-f test.txt'

	std::optional<std::string> value; // Filled out during parsing by user input or falling back to default value
};

struct IntegerArg : public Arg {
	IntegerArg(const char* help, const char* long_name, std::optional<int> def = std::nullopt)
		: Arg(help, long_name, std::nullopt, true) {
		if (def.has_value()) {
			this->default_value = std::to_string(def.value());
		}
		this->value = this->default_value;
	}
	int get() const { return this->value ? std::stoi(*this->value) : 0; }
};

struct FloatArg : public Arg {
	FloatArg(const char* help, const char* long_name, std::optional<float> def = std::nullopt)
		: Arg(help, long_name, std::nullopt, true) {
		if (def.has_value()) {
			this->default_value = std::to_string(def.value());
		}
		this->value = this->default_value;
	}
	float get() const { return this->value ? std::stof(*this->value) : 0.0f; }
};

struct StringArg : public Arg {
	StringArg(const char* help, const char* long_name, std::optional<const char*> def = std::nullopt)
		: Arg(help, long_name, std::nullopt, true) {
		if (def.has_value()) {
			this->default_value = def.value();
		}
		this->value = this->default_value;
	}
	const char* get() const { return this->value ? this->value->c_str() : "NULL"; }
};

struct FlagArg : public Arg {
	FlagArg(const char* help, const char* long_name) : Arg(help, long_name, std::nullopt, false) {
		this->default_value = "false";
		this->value = "false";
	}
	bool get() const { return this->value ? this->value == "true" : false; }
};

struct Args {
	Args(const char* help, std::vector<Arg*> args) : help(help), args(args) {
		short_opts.reserve(args.size() * 2);
		for (const auto& arg : args) {
			short_opts += arg->short_name;
			if (arg->has_arg) {
				short_opts += ':';
			}
		}

		long_opts.reserve(args.size());
		for (const auto& arg : args) {
			long_opts.emplace_back(
				option{
					arg->long_name,
					arg->has_arg ? required_argument : no_argument,
					nullptr,
					arg->short_name,
				});
		}
	}

	void print_help(const shell* sh) {
		shell_print(sh, "---\n%s\nArguments:", this->help);
		for (size_t i = 0; i < this->args.size(); ++i) {
			std::string required_str = this->args[i]->default_value.has_value()
										   ? "optional, default: " + this->args[i]->default_value.value()
										   : "required";
			shell_print(
				sh,
				"\t-%c, --%s: %s (%s)",
				this->args[i]->short_name,
				this->args[i]->long_name,
				this->args[i]->help,
				required_str.c_str());
		}
	}

	// Return true if all required arguments are parsed successfully, false otherwise.
	bool parse_all(const shell* sh, size_t argc, char** argv) {
		optind = 1; // Reset getopt state.

		while (true) {
			const int opt = getopt_long(argc, argv, short_opts.c_str(), long_opts.data(), NULL);
			if (opt == -1) {
				break;
			}

			for (auto& arg : args) {
				if (arg->short_name == opt) {
					if (arg->has_arg) {
						arg->value = optarg;
					} else {
						arg->value = "true";
					}
					break;
				}
			}
		}

		bool args_missing = false;
		for (auto& arg : args) {
			if (!arg->value.has_value()) {
				shell_print(sh, "Missing required argument: %s", arg->long_name);
				args_missing = true;
			}
		}
		if (args_missing) {
			this->print_help(sh);
			return false;
		}

		return true;
	}

	const char* help;
	std::vector<Arg*> args;

	std::string short_opts;
	std::vector<option> long_opts;
};
