#include <Logger.h>

Logger::Logger() :
		File()  {
	File.open("logs/log" + GetTime());
	if (!File.is_open()) {
		LogError("Cant open file");
	}
}



Logger::~Logger() {
	File.close();
}


Logger::Logger(Logger & logger) {
	
}



Logger & Logger::GetLogger() {
	static Logger LoggerInstance; 
	return LoggerInstance;

}



void Logger::Log (std::string log) {
	std::string writtenLog = "[" ANSI_COLOR_GREEN "STDOUT"
	       	             	ANSI_COLOR_RESET "]: "
			       	+ log + "\n";
	
	File << writtenLog;
	
	std::cout << writtenLog;
}



void Logger::LogError (std::string log) {
	std::string writtenLog = "[" ANSI_COLOR_RED "ERROR"
	       			ANSI_COLOR_RESET "]: "
			       	+ log + "\n";

	File << log;
	
	std::cout << writtenLog;
}



std::string Logger::GetTime () {
	return std::to_string(std::time(NULL)); 
}

