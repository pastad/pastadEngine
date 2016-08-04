#ifndef SUBSYSTEM_H
#define SUBSYSTEM_H

#include <string>

// Represents the basic subsystem

class Subsystem
{

public:
	Subsystem(std::string name);
	~Subsystem();

	// starts the system
	bool startUp();

	// shuts down the system
	bool shutDown();

	// returns true if everything is up and running
	bool systemCheck();

protected:

	// indicates initialization
	bool m_initialized;

	// name of the subsystem
	std::string m_name;
	
};

#endif