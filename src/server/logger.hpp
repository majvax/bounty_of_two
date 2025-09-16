#include <spdlog/logger.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/spdlog.h>

inline void init_loggers()
{
    constexpr auto filename = "server.log";
    auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(filename);

    auto app_logger = std::make_shared<spdlog::logger>("app", file_sink);
    spdlog::register_logger(app_logger);

    auto net_logger = std::make_shared<spdlog::logger>("network", file_sink);
    spdlog::register_logger(net_logger);

    auto callback_logger = std::make_shared<spdlog::logger>("callback", file_sink);
    spdlog::register_logger(callback_logger);
}
