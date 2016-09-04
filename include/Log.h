#ifndef LOG_H
#define LOG

#include <string>
#include <mutex>

enum LogFlag
{
  LF_None = 0,
  LF_Main =2,
  LF_TS = 4,
  LF_Editor = 8,
  LF_All = 16
};


class Log
{
public:
	Log( LogFlag lf);
	~Log();

	// sets the debug mode / logs to console
	void debugMode();

	// logs a message
	void log(std::string tag, std::string msg);	

	// logs a messae with step info
	void log(std::string tag, std::string step, std::string msg);	

  // logs a message  thread safety info
  void log(LogFlag lf, std::string tag, std::string msg);

  // logs a messae with step info  thread safety info
  void log(LogFlag lf,std::string tag, std::string step, std::string msg);

private:

	// logs a message to the console
	void logToConsole( std::string tag, std::string msg);

	// logs a message with step info to the console
	void logToConsole( std::string tag, std::string step, std::string msg);

	// determines if debug mode is set
	bool m_debug;

  // for threadsafe writing
  std::mutex m_lock;
  
  // the flags for the log settings
  LogFlag m_flags;
	
};


#endif