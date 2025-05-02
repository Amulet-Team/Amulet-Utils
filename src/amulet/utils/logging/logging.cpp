#include <iostream>
#include <mutex>
#include <string>

#include "logging.hpp"

namespace Amulet {

int& get_min_log_level()
{
    static int min_log_level = 20;
    return min_log_level;
}

void set_min_log_level(int level)
{
    get_min_log_level() = level;
}

static std::mutex& get_default_log_mutex() {
    static std::mutex log_mutex;
    return log_mutex;
}

static Amulet::SignalToken<int, std::string> get_default_log_handler_token() {
    static Amulet::SignalToken<int, std::string> default_log_handler_token;
    return default_log_handler_token;
}

static void default_log_handler(int level, const std::string& msg)
{
    std::cout << "log default" << std::endl;
    std::unique_lock lock(get_default_log_mutex());
    std::cout << msg << std::endl;
}

Amulet::Signal<int, std::string>& get_logger()
{
    std::cout << "get_logger" << std::endl;
    // Initialise dependent global variables
    get_min_log_level();
    get_default_log_mutex();
    get_default_log_handler_token();
    static Amulet::Signal<int, std::string> logger;
    std::cout << "got_logger" << std::endl;
    // Setup the default log handler.
    static bool init_hanler = true;
    if (init_hanler) {
        get_default_log_handler_token() = logger.connect(default_log_handler);
        init_hanler = false;
    }
    return logger;
}

void log(int level, const std::string& msg)
{
    std::cout << "log" << std::endl;
    if (get_min_log_level() <= level) {
        get_logger().emit(level, msg);
    }
}

void debug(const std::string& msg)
{
    std::cout << "debug" << std::endl;
    log(10, msg);
}

void info(const std::string& msg)
{
    std::cout << "info" << std::endl;
    log(20, msg);
}

void warning(const std::string& msg)
{
    std::cout << "warning" << std::endl;
    log(30, msg);
}

void error(const std::string& msg)
{
    std::cout << "error" << std::endl;
    log(40, msg);
}

void critical(const std::string& msg)
{
    std::cout << "critical" << std::endl;
    log(50, msg);
}

void register_default_log_handler()
{
    get_default_log_handler_token() = get_logger().connect(default_log_handler);
}

void unregister_default_log_handler()
{
    get_logger().disconnect(get_default_log_handler_token());
}

} // namespace Amulet
