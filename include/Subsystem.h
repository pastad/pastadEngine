#ifndef SUBSYSTEM_H
#define SUBSYSTEM_H

// Represents the basic subsystem

class Subsystem
{

public:
	Subsystem();
	virtual ~Subsystem();

	virtual bool startUp();
	virtual bool shutDown();

private:
	bool m_initialized;
	
};

#endif