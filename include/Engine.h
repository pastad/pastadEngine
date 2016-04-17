#ifndef ENGINE_H
#define ENGINE_H

#include "gl_core_4_.h"
#include <GLFW/glfw3.h>
#include <vector>

#define NUM_TIME_SAMPLES 40

// Engine class ... nothing more .. nothing less

class Log;
class Scene;
class RenderSubsystem;
class IOSubsystem;
class GUI;

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

	// renders the current set scene threw the render subsystem
	static void render();
	
	// sets the scene 
	static void setScene(Scene * scene);

	// returns the current set scene
	static Scene * getScene();

	// function to request a GUI
	static GUI * getGUI();

	// window size getters
	static unsigned int getWindowWidth();
	static unsigned int getWindowHeight();

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

	// the scene that should be used
	static Scene * m_scene;

	// the graphical user interfaces and the id
	static std::vector<GUI *> m_guis;
	static unsigned int m_guis_id;

	// the window size
	static unsigned int m_win_width;
	static unsigned int m_win_height;

	// times samples
	static float m_time_samples[NUM_TIME_SAMPLES];

	// current time sample
	static unsigned int m_current_time_sample;

	// the time difference between two updates and the last time
	static float m_time_delta;
	static float m_time_last;

	// starts the subsystems
	static bool startUpSubsystems();

	// shuts down the subsystems
	static bool shutDownSubsystems();

	// updates the time (FPS,...)
	static void timeUpdate();

	// callbacks for opengl
	static void windowSizeChangedCallback(GLFWwindow* window, int width, int height);

};


#endif