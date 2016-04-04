#ifndef IOSUBSYSTEM
#define IOSUBSYSTEM

#include "Subsystem.h"

#include <string>

// Subsytem for IO operations

class IOSubsystem : public Subsystem
{
public:
	IOSubsystem();
	~IOSubsystem();

	// starts up the render system
	bool startUp();

	// shuts down the render system
	bool shutDown();

	// returns true if everything is up and running
	bool systemCheck();


	// --- Additionals ---
	
	static std::string readFile(std::string path);

private:

	
	
};




#endif