#ifndef RENDERSUBSYSTEM_H
#define RENDERSUBSYSTEM_H
#include "Engine.h"
#include <GLFW/glfw3.h>

#include "Subsystem.h"

class RenderShader;
class TextShader;
class ImageShader;
class PostProcessingShader;

class GBuffer;
class Quad;
class ShadowBuffer;
class RenderBuffer;

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
	void renderPassFXAA();
	void renderPassShadow();

	// renders the ui set to the engine
	void renderUI();

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

	// the image shader
	ImageShader * m_image_shader;

	// the post processing shader
	PostProcessingShader * m_pp_shader;

	// the gbuffer
	GBuffer * m_gbuffer;

	// the quad where the scene should be rendered on
	Quad * m_render_quad;

	// tmp shadow buffer
	ShadowBuffer * m_shadow_buffer;

	// buffer for the fxaa step
	RenderBuffer * m_fxaa_buffer;

	// starts the rendering cycle
	void startRender();

	// ends the rendering cycle
	void endRender();
	
};

#endif