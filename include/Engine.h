#ifndef ENGINE_H
#define ENGINE_H

// Engine base class

class Log;
class RenderSubsystem;

class Engine
{
public:

	Engine();
	~Engine();

	// Initializes Engine
	static bool initialize(unsigned int width, unsigned int height);

	// Shuts down Engine and it's components
	static void shutDown();	

	// GetterMethods
	static Log * getLog();

private:

	// indicates if Engine is initialized
	static bool m_initialized;

	// holds the standard log object
	static Log * m_log;

	// Subsystems
	static RenderSubsystem * m_render_system;
};


#endif