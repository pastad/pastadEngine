#ifndef RENDERSUBSYSTEM_H
#define RENDERSUBSYSTEM_H
#include "Engine.h"
#include <GLFW/glfw3.h>

#include "Subsystem.h"

class RenderShader;
class TextShader;
class ImageShader;
class PostProcessingShader;
class ShadowShader;

class GBuffer;
class Quad;
class DirectionalShadowBuffer;
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
	void renderPassGBuffer();
	void renderPassLight();
	void renderPassPostProcess();
	void renderPassShadow();
	void renderPassTest();

	// renders the ui set to the engine
	void renderUI();

	// returns the shader used for standard rendering
	RenderShader * getRenderShader();

	// returns the shader for text rendering
	TextShader * getTextShader();

	// sets the post processing technique according to boolean
	void setPostProcessing(PostprocessType type, bool enable);

	// sets a shading technique property
	void setShadowTechnique(ShadowTechniqueType type, bool enable);
	

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

	// the shadow shader
	ShadowShader * m_shadow_shader;

	// the gbuffer
	GBuffer * m_gbuffer;

	// the quad where the scene should be rendered on
	Quad * m_render_quad;

	// tmp shadow buffer
	DirectionalShadowBuffer * m_shadow_buffer;

	// buffer for the fxaa step
	RenderBuffer * m_pp_buffer;

	// true if standard shadows are enabled
	bool m_shadows_standard_enabled;

	// starts the rendering cycle
	void startRender();

	// ends the rendering cycle
	void endRender();

	
};

#endif