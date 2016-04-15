#include <iostream>

#include "Log.h"

#define TAG_MAX_LENGTH 35 // Maximum range of a tag

Log::Log():m_debug(false)
{	
}

Log::~Log()
{	
}

void Log::debugMode()
{
	m_debug = true;
}

void Log::logToConsole( std::string tag, std::string msg)
{
	std::string spaces( TAG_MAX_LENGTH -tag.size(), ' ');
	std::cout << tag<<spaces << " : " << msg <<std::endl;
}
void Log::logToConsole( std::string tag, std::string step,  std::string msg)
{
  std::string spaces( TAG_MAX_LENGTH -tag.size(), ' ');
  std::cout << tag<<spaces << " : "<<step<<" " << msg <<std::endl;
}

void Log::log( std::string tag, std::string msg)
{
	if(m_debug)
		logToConsole( tag, msg);
}
void Log::log( std::string tag, std::string step,  std::string msg)
{
  if(m_debug)
    logToConsole( tag, step, msg);
}