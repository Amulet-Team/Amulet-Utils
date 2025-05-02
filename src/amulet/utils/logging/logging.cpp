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

Amulet::Signal<int, std::string>& get_logger()
{
    std::cout << "get_logger" << std::endl;
    get_min_log_level();
    static Amulet::Signal<int, std::string> logger;
    std::cout << "got_logger" << std::endl;
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

static void default_log_handler(int level, const std::string& msg)
{
    std::cout << "log default" << std::endl;
    static std::mutex log_mutex;
    std::unique_lock lock(log_mutex);
    std::cout << msg << std::endl;
}

static Amulet::SignalToken<int, std::string> default_log_handler_token = get_logger().connect(&default_log_handler);

void register_default_log_handler()
{
    default_log_handler_token = get_logger().connect(default_log_handler);
}

void unregister_default_log_handler()
{
    get_logger().disconnect(default_log_handler_token);
}

} // namespace Amulet
