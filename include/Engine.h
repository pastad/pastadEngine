#ifndef ENGINE_H
#define ENGINE_H

#include "gl_core_4_.h"
#include <GLFW/glfw3.h>

// Engine class ... nothing more .. nothing less

class Log;
class RenderSubsystem;
class IOSubsystem;

enum SubsystemType
{
	RENDER_SUBSYSTEM = 1,
	IO_SUBSYSTEM = 2
};


class Engine
{
public:

	Engine();
	~Engine();

	// Initializes Engine
	static bool initialize(unsigned int width, unsigned int height, unsigned int types);

	// Shuts down Engine and it's components
	static void shutDown();	

	// Sets shut down flag because of major error
	static void errorShutDown();

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
	static IOSubsystem  * m_io_system;

	// gl context
	static GLFWwindow * m_window;

	// the subsystems to be started
	static unsigned int m_system_flags;

	// starts the subsystems
	static bool startUpSubsystems();

	// shuts down the subsystems
	static bool shutDownSubsystems();

};


#endif