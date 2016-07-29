#include "Subsystem.h"

#include "Engine.h"
#include "Log.h"

Subsystem::Subsystem( std::string name ):m_initialized(false), m_name(name)
{	
}
Subsystem::~Subsystem()
{	
}
bool Subsystem::startUp()
{
  if(!m_initialized)
  {
    Engine::getLog()->log(m_name,"started");
    m_initialized = true;
    return true;
  }

  return false;
}
bool Subsystem::shutDown()
{
  if(m_initialized)
  {
    Engine::getLog()->log(m_name,"shut down");
    m_initialized = false;
    return true;
  } 

  return false;
}
bool Subsystem::systemCheck()
{
	return m_initialized;
}