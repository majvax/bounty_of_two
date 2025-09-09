#include <spdlog/logger.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/spdlog.h>

inline void init_loggers()
{
    constexpr auto filename = "client.log";
    auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(filename, true);

    auto app_logger = std::make_shared<spdlog::logger>("app", file_sink);
    spdlog::register_logger(app_logger);

    auto net_logger = std::make_shared<spdlog::logger>("network", file_sink);
    spdlog::register_logger(net_logger);

    auto engine_logger = std::make_shared<spdlog::logger>("engine", file_sink);
    spdlog::register_logger(engine_logger);
}
