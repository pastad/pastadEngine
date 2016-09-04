#include <iostream>

#include "Log.h"

#define TAG_MAX_LENGTH 35 // Maximum range of a tag

Log::Log(LogFlag lf):m_flags(lf),m_debug(false)
{	
}

Log::~Log()
{	
}


// logging ------------------------------------------

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
  m_lock.lock();
	if(m_debug)
		logToConsole( tag, msg);
  m_lock.unlock();
}
void Log::log( std::string tag, std::string step,  std::string msg)
{
  m_lock.lock();
  if(m_debug)
    logToConsole( tag, step, msg);
  m_lock.unlock();
}

void Log::log(LogFlag lf, std::string tag, std::string msg)
{
  if( (lf & m_flags ) || ( LF_All & m_flags )  )
  {
    m_lock.lock();
    if (m_debug)
      logToConsole(tag, msg);
    m_lock.unlock();
  }
}
void Log::log(LogFlag lf, std::string tag, std::string step, std::string msg)
{
  if ((lf & m_flags) || (LF_All & m_flags))
  {
    m_lock.lock();
    if (m_debug)
      logToConsole(tag, step, msg);
    m_lock.unlock();
  }
}






// getter/setter ------------------------------------------

void Log::debugMode()
{
	m_debug = true;
}
