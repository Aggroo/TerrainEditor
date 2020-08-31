#pragma once
//------------------------------------------------------------------------------
/**
    A class to handle logging to console and file

    (C) 2020 Individual contributors, see AUTHORS file
*/
//------------------------------------------------------------------------------
#include <memory.h>
#include "spdlog/spdlog.h"

namespace IO
{
class Logger
{
public:
    Logger();
    ~Logger();

    static void Init();

    inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return coreLogger; }
    inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return clientLogger; }

private:

    static std::shared_ptr<spdlog::logger> coreLogger;
    static std::shared_ptr<spdlog::logger> clientLogger;

};
} // namespace IO
#define T_CORE_TRACE(...) ::IO::Logger::GetCoreLogger()->trace(__VA_ARGS__)
#define T_CORE_INFO(...) ::IO::Logger::GetCoreLogger()->info(__VA_ARGS__)
#define T_CORE_ERROR(...) ::IO::Logger::GetCoreLogger()->error(__VA_ARGS__)
#define T_CORE_DEBUG(...) ::IO::Logger::GetCoreLogger()->debug(__VA_ARGS__)
#define T_CORE_WARN(...) ::IO::Logger::GetCoreLogger()->warn(__VA_ARGS__)

#define T_CLIENT_TRACE(...) ::IO::Logger::GetClientLogger()->trace(__VA_ARGS__)
#define T_CLIENT_INFO(...) ::IO::Logger::GetClientLogger()->info(__VA_ARGS__)
#define T_CLIENT_ERROR(...) ::IO::Logger::GetClientLogger()->error(__VA_ARGS__)
#define T_CLIENT_DEBUG(...) ::IO::Logger::GetClientLogger()->debug(__VA_ARGS__)
#define T_CLIENT_WARN(...) ::IO::Logger::GetClientLogger()->warn(__VA_ARGS__)