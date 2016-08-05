#ifndef ENGINE_H
#define ENGINE_H

#include "gl_core_4_.h"
#define GLM_FORCE_RADIANS
#include <GLFW/glfw3.h>
#include <vector>
#include <glm/glm.hpp>

#define NUM_TIME_SAMPLES 40

#define NEAR_CLIPPING_PLANE 0.1f
#define FAR_CLIPPING_PLANE 1000.0f

// Engine class ... nothing more .. nothing less

class Log;
class Scene;
class RenderSubsystem;
class IOSubsystem;
class PhysicSubsystem;
class AudioSubsystem;
class GUI;
class EngineGUI;
class Object;
class Light;
class SceneEditor;

enum SubsystemType
{
	RENDER_SUBSYSTEM = 1,
	IO_SUBSYSTEM = 2,
	PHYSIC_SUBSYSTEM = 4,
	AUDIO_SUBSYSTEM = 8
};
enum PostprocessType
{
	PP_FXAA = 1,
	PP_HDR = 2,
	PP_BLOOM = 3
};
enum ShadowTechniqueType
{	
	ST_NONE =0,
	ST_STANDARD =1,
	ST_STANDARD_PCF = 2,
	ST_STANDARD_RS = 3
};


class Engine
{
public:

	Engine();
	~Engine();

	// Initializes Engine
	static bool initialize(unsigned int width, unsigned int height, unsigned int types, bool edit_mode, bool fullscreen);

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
	static void setScene(Scene * scene, bool delete_old);

	// returns the current set scene
	static Scene * getScene();

	// function to request a GUI
	static GUI * addGUI();

	// returns engine gui
	static EngineGUI * getEngineGUI();

	//returns the scene editor
	static SceneEditor * getSceneEditor();

	// requiering and removing GUis
	static std::vector< GUI *> * getGUIs();
	static void removeGUI(GUI * gui);

	// window size getters
	static unsigned int getWindowWidth();
	static unsigned int getWindowHeight();

	// checks the guis in case mouse button is pressed
	static bool checkGUIsForButtonPresses(float x, float y);

	// sets the post processing technique according to boolean
	static void setPostProcessing(PostprocessType type, bool enable);

	// sets a shadow property
	static void setShadowTechnique(ShadowTechniqueType type);

	// returns true if in edit mode
	static bool isInEditMode();

	// refreshes the shaders
	static void refreshShaders();

	// sets the FPS that shoul be rendered
	static void setFPS(float);

	// returns the time difference between updates
	static float getTimeDelta();

	// called by IO::Subsystem
	static void keyWasPressed(unsigned int key_code);

	// returns true if gui movement lock is set
	static bool isGUIMovementLockSet();

	// setter for GUIMovementLock
	static void setGUIMovementLock();
	static void unsetGUIMovementLock();

	// returns the physics,io subsystem or nullptr if not running
	static PhysicSubsystem * getPhysicSubsystem();
	static IOSubsystem * getIOSubsystem();


	// passing from subsystems

	// returns object at screen center
	static Object * pickObjectAtCenter();

	// returns object at screen po
	static Object * pickObjectAt(glm::vec2 p);

	// returns light at screen po
	static Light * pickLightAt(glm::vec2 p);

	// returns true if key is released and now pressed
	static bool isKeyReleasedAndPressed(int key_code);
	static bool isMouseButtonReleasedAndPressed(int key_code);

private:

	// indicates if Engine is initialized
	static bool m_initialized;

	// holds the standard log object
	static Log * m_log;

	// Subsystems
	static RenderSubsystem * m_render_system;
	static IOSubsystem  * m_io_system;
	static PhysicSubsystem  * m_physic_system;
	static AudioSubsystem  * m_audio_system;

	// gl context
	static GLFWwindow * m_window;

	// the subsystems to be started
	static unsigned int m_system_flags;

	// the scene that should be used
	static Scene * m_scene;
	static Scene * m_scene_next; 
	static bool m_scene_next_delete;

	// the graphical user interfaces and the id
	static std::vector<GUI *> m_guis;
	static unsigned int m_guis_id;

	// internal engine gui 
	static EngineGUI * m_engine_gui;

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

	// the time of the last render;
	static float m_time_last_render;

	// true if rerender needed
	static bool m_render_update_needed;

	// the time between renders
	static float m_render_update_delta ;

	// true if engine is in editing mode
	static bool m_edit_mode;

	// the editor for the scene
	static SceneEditor * m_scene_editor;

	// true if fullscreen is set
	static bool m_fullscreen;

	// the lock for example when a edit box is edited
	static bool m_gui_movement_lock;

	// starts the subsystems
	static bool startUpSubsystems();

	// shuts down the subsystems
	static bool shutDownSubsystems();

	// updates the time (FPS,...)
	static void timeUpdate();

	// callbacks for opengl
	static void windowSizeChangedCallback(GLFWwindow* window, int width, int height);

	// deletes windows if set and creates a new one
	static void refreshWindow();

	//switches the scene at end of turn if requeseted
	static void sceneSwitch();

	// static for async button check
	static bool buttonCheck(GUI *gui , float x, float y);

};


#endif