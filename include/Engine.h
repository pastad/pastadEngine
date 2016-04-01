#ifndef ENGINE_H
#define ENGINE_H

#include <GLFW/glfw3.h>

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

	// check for gl version
	static bool checkVersionSupport(unsigned int version_major, unsigned int version_minor );

	// update function
	static void update();

	// returns true if application should continue running
	static bool running();

	// starts the rendering cycle
	static void beginRender();

	// ends the rendering cycle
	static void endRender();

private:

	// indicates if Engine is initialized
	static bool m_initialized;

	// holds the standard log object
	static Log * m_log;

	// Subsystems
	static RenderSubsystem * m_render_system;

	// gl context
	static GLFWwindow * m_window;
};


#endif