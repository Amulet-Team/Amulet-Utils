#pragma once

#include <string>

#include <amulet/utils/dll.hpp>
#include <amulet/utils/signal/signal.hpp>

namespace Amulet {

// Register the default log handler.
// This is registered by default with a log level of 20.
// Thread safe.
AMULET_UTILS_EXPORT void register_default_log_handler();

// Unregister the default log handler.
// Thread safe.
AMULET_UTILS_EXPORT void unregister_default_log_handler();

// Get the log level used by the default logger.
// The default logger will only log messages with a level at least this large.
// Thread safe.
AMULET_UTILS_EXPORT int get_default_log_level();

// Set the log level used by the default logger.
// The default logger will only log messages with a level at least this large.
// Thread safe.
AMULET_UTILS_EXPORT void set_default_log_level(int);

// The logger signal.
// This is emitted with the message and its level every time a message is logged.
AMULET_UTILS_EXPORT extern Signal<int, std::string> logger;

// Log a message with a custom level.
// Thread safe.
AMULET_UTILS_EXPORT void log(int level, const std::string& msg);

// Log a message with debug level (10).
// Thread safe.
AMULET_UTILS_EXPORT void debug(const std::string& msg);

// Log a message with info level (20).
// Thread safe.
AMULET_UTILS_EXPORT void info(const std::string& msg);

// Log a message with warning level (30).
// Thread safe.
AMULET_UTILS_EXPORT void warning(const std::string& msg);

// Log a message with error level (40).
// Thread safe.
AMULET_UTILS_EXPORT void error(const std::string& msg);

// Log a message with info level (50).
// Thread safe.
AMULET_UTILS_EXPORT void critical(const std::string& msg);

}
