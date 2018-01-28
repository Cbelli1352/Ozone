#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <thread>
#include <queue>

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

class Logger {
private:
	std::string LogPath;
	std::ofstream File;
	Logger();
	Logger(Logger & logger);
	std::string GetTime();
public:
	void Log (std::string log);
	void LogError(std::string log);
	void LogWarning(std::string log);
	static Logger & GetLogger();
	~Logger();
};
