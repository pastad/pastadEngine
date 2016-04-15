#ifndef LOG_H
#define LOG

#include <string>

class Log
{
public:
	Log();
	~Log();

	// sets the debug mode / logs to console
	void debugMode();

	// logs a message
	void log(std::string tag, std::string msg);	

	// logs a messae with step info
	void log(std::string tag, std::string step, std::string msg);	

private:

	// logs a message to the console
	void logToConsole( std::string tag, std::string msg);

	// logs a message with step info to the console
	void logToConsole( std::string tag, std::string step, std::string msg);

	// determines if debug mode is set
	bool m_debug;
	
};


#endif