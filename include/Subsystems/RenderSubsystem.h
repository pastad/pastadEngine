#ifndef RENDERSUBSYSTEM_H
#define RENDERSUBSYSTEM_H
#include "Engine.h"
#include <GLFW/glfw3.h>

#include "Subsystem.h"

class RenderShader;
class TextShader;
class GBuffer;
class Quad;

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

	// returns true if everything is up and running
	bool systemCheck();


	// --- Additionals ---

	// Renders the scene currently set in the Engine
	void render();

	// the render passess
	void renderPassOne();
	void renderPassTwo();

	// returns the shader used for standard rendering
	RenderShader * getRenderShader();

	// returns the shader for text rendering
	TextShader * getTextShader();
	

private:

	// the gl window pointer
	GLFWwindow * m_window;

	// the render shader
	RenderShader * m_shader; 

	// the text shader
	TextShader * m_text_shader;

	// the gbuffer
	GBuffer * m_gbuffer;

	// the quad where the scene should be rendered on
	Quad * m_render_quad;

	// starts the rendering cycle
	void startRender();

	// ends the rendering cycle
	void endRender();
	
};

#endif