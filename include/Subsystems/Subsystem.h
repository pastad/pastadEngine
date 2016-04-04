#ifndef SUBSYSTEM_H
#define SUBSYSTEM_H

// Represents the basic subsystem

class Subsystem
{

public:
	Subsystem();
	virtual ~Subsystem();

	// starts the system
	virtual bool startUp();

	// shuts down the system
	virtual bool shutDown();

	// returns true if everything is up and running
	virtual bool systemCheck();

protected:

	// indicates initialization
	bool m_initialized;
	
};

#endif