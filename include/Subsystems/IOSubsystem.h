#ifndef IOSUBSYSTEM
#define IOSUBSYSTEM

#include "Engine.h"
#include <GLFW/glfw3.h>

#include "Subsystem.h"

#include <string>

// Subsytem for IO operations

class IOSubsystem : public Subsystem
{
public:
	IOSubsystem();
	~IOSubsystem();

	// starts up the render system
	bool startUp(GLFWwindow * window);

	// shuts down the render system
	bool shutDown();

	// returns true if everything is up and running
	bool systemCheck();


	// --- Additionals ---
	
	// reads in the file
	static std::string readFile(std::string path);

	// the callback functions for glfw
	static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void mouseMoveCallback(GLFWwindow* window, double x, double y );

	static bool isKeyPressed(int key);

private:

	// determines if keys are pressed
	static bool m_keys[GLFW_KEY_LAST];

	// current mouse position
	static double m_mouse_x;
	static double m_mouse_y;
	
	
};




#endif