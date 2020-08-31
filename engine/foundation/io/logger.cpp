//------------------------------------------------------------------------------
//  logger.cc
//  (C) 2020 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "config.h"
#include "logger.h"
#include "spdlog/sinks/stdout_color_sinks.h"

namespace IO
{

std::shared_ptr<spdlog::logger> Logger::coreLogger;
std::shared_ptr<spdlog::logger> Logger::clientLogger;

Logger::Logger()
{
}

Logger::~Logger()
{
}

void Logger::Init()
{
	spdlog::set_pattern("%^[%T] [%n]: %v%$");
	coreLogger = spdlog::stdout_color_mt("TERRAIN ENGINE");
	coreLogger->set_level(spdlog::level::trace);
	clientLogger = spdlog::stdout_color_mt("APP");
	clientLogger->set_level(spdlog::level::trace);
	
}
} // namespace IO
