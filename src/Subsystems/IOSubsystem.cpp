#include "IOSubsystem.h"

#include "Engine.h"
#include "Log.h"

#include <iostream>
#include <fstream>

IOSubsystem::IOSubsystem()
{
	m_initialized = false;
}

IOSubsystem::~IOSubsystem()
{
}


bool IOSubsystem::startUp()
{
	if(! m_initialized)
	{
		// init system here

		m_initialized = true;
		Engine::getLog()->log("IOSubsystem", "started");
		return true;
	}
	return false;
}

bool IOSubsystem::shutDown()
{
	if(m_initialized)
	{

		m_initialized = false;
		Engine::getLog()->log("IOSubsystem", "shut down");
		return true;
	}
	return false;
}

bool IOSubsystem::systemCheck()
{
	return m_initialized;
}

std::string IOSubsystem::readFile(std::string path)
{
	std::ifstream file;
  file.open((path).c_str());
  std::string output;
 
  if(file.is_open())
  {
    std::string line;
    while(file.good())
    {
      getline(file, line);
      output.append(line + "\n");
    }
    file.close();
  }
  else
    Engine::getLog()->log("Shader","Unable to load shader file");
    
  return output;
}
