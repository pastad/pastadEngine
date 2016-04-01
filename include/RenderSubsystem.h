#ifndef RENDERSUBSYSTEM_H
#define RENDERSUBSYSTEM_H

#include <GLFW/glfw3.h>

#include "Subsystem.h"

// Subsystem that is used for rendering

class RenderSubsystem : public Subsystem
{
public:
	RenderSubsystem();
	~RenderSubsystem();

	// starts up the render system
	bool startUp( GLFWwindow * window );

	// shuts down the render system
	bool shutDown();

private:

	// the gl window pointer
	GLFWwindow * m_window;
	
};

#endif