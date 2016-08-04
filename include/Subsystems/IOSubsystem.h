#ifndef IOSUBSYSTEM
#define IOSUBSYSTEM

#include "Engine.h"

#include <glm/glm.hpp>
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

	// from base class
	using Subsystem::systemCheck;
	using Subsystem::shutDown;


	// --- Additionals ---
	
	// reads in the file
	static std::string readFile(std::string path);

	// the callback functions for glfw
	static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void mouseMoveCallback(GLFWwindow* window, double x, double y );
	static void mouseButtonCallback(GLFWwindow *, int, int, int);

	// returns true if key is pressed
	static bool isKeyPressed(int key);

	// returns true if mouse button is pressed
	static bool isMouseButtonPressed(int key);

	// returns true if mouse button was pressed and released
	static bool isMouseButtonPressedAndReleased(int key);

	// returns true if mouse button was released and pressed
	static bool isMouseButtonReleasedAndPressed(int key);



	// returns the current mouse coords
	static glm::vec2 getMouseCoordinates();

	// returns the movement of the mouse | attention resets to (0,0) after call
	static glm::vec2 getMouseDelta();

	// returns the keys that the user just pressed and released 
	static bool * getKeysPressedAndReleased();

	// returns true if the key was just pressed and released
	static bool   isKeyPressedAndReleased(int key);

	// returns true if the key was just released and pressed
	static bool   isKeyReleasedAndPressed(int key);

	// resets the keys when queried
	static void clearKeys();

private:

	// determines if keys are pressed,...
	static bool m_keys[GLFW_KEY_LAST + 1];
	static bool m_keys_pressed_and_released[GLFW_KEY_LAST + 1];
	static bool m_keys_released_and_pressed[GLFW_KEY_LAST + 1];

	// determines which mouse buttons are currently pressed,....
	static bool m_mouse_buttons[GLFW_MOUSE_BUTTON_8 + 1 ];
	static bool m_mouse_buttons_pressed_and_released[GLFW_MOUSE_BUTTON_8 + 1 ];
	static bool m_mouse_buttons_released_and_pressed[GLFW_MOUSE_BUTTON_8 + 1 ];

	// current mouse position and movement
	static double m_mouse_x;
	static double m_mouse_y;
	static glm::vec2 m_mouse_delta;
	
	
};




#endif