#ifndef RENDERSUBSYSTEM_H
#define RENDERSUBSYSTEM_H

#include "Engine.h"
#include <GLFW/glfw3.h>

#include "Subsystem.h"

class RenderShader;
class TextShader;
class ImageShader;
class PostProcessingShader;
class RenderBaseShader;
class PointShadowShader;
class SkyboxShader;

class GBuffer;
class Quad;
class DirectionalShadowBuffer;
class RenderBuffer;
class TransparentBuffer;
class JitterTexture;
class NoiseTexture;

// Subsystem that is used for rendering

class RenderSubsystem : public Subsystem
{
public:
	RenderSubsystem();
	~RenderSubsystem();

	// starts up the render system
	bool startUp( GLFWwindow * window );

	// from base class
	using Subsystem::systemCheck;
	using Subsystem::shutDown;


	// --- Additionals ---

	// Renders the scene currently set in the Engine
    void render(bool swap_buffer);


	// returns the shader used for standard rendering
	RenderShader * getRenderShader();

	// returns the shader for text rendering
	TextShader * getTextShader();

	// sets the post processing technique according to boolean
	void setPostProcessing(PostprocessType type, bool enable);

	// sets a shading technique property
	void setShadowTechniqueDirectional(ShadowTechniqueType type);
  void setShadowTechniquePoint(ShadowTechniqueType type);
  void setShadowSSAO(ShadowTechniqueType type);

	// reloads the shaders
	bool refreshShaders();

	// returns the object at the center
	Object * pickObjectAtCenter();

	// returns object at position
	Object * pickObjectAt(glm::vec2 p);

	// returns light at position
	Light * pickLightAt(glm::vec2 p);

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
	RenderBaseShader * m_shadow_shader;

	// the point shadow shader
	RenderBaseShader * m_point_shadow_shader;

	// the skybox shader
	SkyboxShader * m_skybox_shader;

	// the terrain shader
	RenderBaseShader * m_terrain_shader;

	// the water shader
	RenderBaseShader * m_water_shader;

	// the gbuffer
	GBuffer * m_gbuffer;

	// the quad where the scene should be rendered on
	Quad * m_render_quad;

	// tmp shadow buffer
	DirectionalShadowBuffer * m_shadow_buffer;

	// buffer for the fxaa step
	RenderBuffer * m_pp_buffer;

	// buffer for the light texture
	RenderBuffer * m_light_buffer;

	// buffer for the blurring texture
	RenderBuffer * m_blur_buffer;

	// buffer for the ssao texture
	RenderBuffer * m_ssao_buffer;

  // buffer for the transparent texture
  TransparentBuffer * m_transparent_buffer;

	// the jitter texture to smooth shadows
	JitterTexture * m_jitter;

  // the texture for the noisein the ssao
  NoiseTexture * m_noise_texture;

	// true if standard shadows are enabled
	bool m_shadows_standard_directional_enabled;
  bool m_shadows_standard_point_enabled;

  // the shadow technique
  ShadowTechniqueType m_shadow_techniques;
  ShadowTechniqueType m_shadow_technique_point;
  bool m_shadow_ssao;
  PostprocessType m_pp_techniques;

	// true if hdr is set
	bool m_enable_hdr;

	// true if bloom is set
	bool m_enable_bloom;

	// starts the rendering cycle
	void startRender();

	// ends the rendering cycle
  void endRender(bool swap_buffer);

	// the render passess
	void renderPassGBuffer();
	void renderPassLight();
  void renderPassTransparent();

	// postprocessing passes
	void renderPassPostProcess();
	void renderPassShadow();
	void renderPassLightBlur();
	void renderSSAO();

	// renders the ui set to the engine
	void renderUI();

	
};

#endif
