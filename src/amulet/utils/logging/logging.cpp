#include <iostream>
#include <mutex>
#include <string>

#include "logging.hpp"

namespace Amulet {

static int min_log_level = 20;

static void default_log_handler(int level, const std::string& msg)
{
    static std::mutex log_mutex;
    std::unique_lock lock(log_mutex);
    std::cout << msg << std::endl;
}

Amulet::Signal<int, std::string>& get_logger()
{
    static Amulet::Signal<int, std::string> logger;
    return logger;
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

int get_min_log_level()
{
    return min_log_level;
}

void set_min_log_level(int level)
{
    min_log_level = level;
}

void log(int level, const std::string& msg)
{
    if (min_log_level <= level) {
        get_logger().emit(level, msg);
    }
}

void debug(const std::string& msg)
{
    log(10, msg);
}

void info(const std::string& msg)
{
    log(20, msg);
}

void warning(const std::string& msg)
{
    log(30, msg);
}

void error(const std::string& msg)
{
    log(40, msg);
}

void critical(const std::string& msg)
{
    log(50, msg);
}

} // namespace Amulet
