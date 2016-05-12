#include "RenderSubsystem.h"

#include "Log.h"
#include "Camera.h"
#include "Scene.h"

#include "DirectionalShadowBuffer.h"
#include "RenderBuffer.h"
#include "GBuffer.h"
#include "JitterTexture.h"
#include "Texture.h"


#include "RenderShader.h"
#include "TextShader.h"
#include "ImageShader.h"
#include "PostProcessingShader.h"
#include "RenderBaseShader.h"
#include "SkyboxShader.h"

#include "Quad.h"
#include "Transform.h"

#include "GUI.h"
#include "EngineGUI.h"

RenderSubsystem::RenderSubsystem()
{	
	m_initialized = false;	
}

RenderSubsystem::~RenderSubsystem()
{	
	if(m_shader != nullptr)
		delete m_shader;
	if(m_text_shader != nullptr)
		delete m_text_shader;
	if(m_image_shader != nullptr)
		delete m_image_shader;
	if(m_pp_shader != nullptr)
		delete m_pp_shader;
	if(m_shadow_shader != nullptr)
		delete m_shadow_shader;
	if(m_point_shadow_shader != nullptr)
		delete m_point_shadow_shader;
	if(m_skybox_shader != nullptr)
		delete m_skybox_shader;
	if(m_gbuffer != nullptr)
		delete m_gbuffer;
	if(m_render_quad != nullptr)
		delete m_render_quad;
	if(m_pp_buffer != nullptr)
		delete m_pp_buffer;
	if(m_light_buffer != nullptr)
		delete m_light_buffer;
	if(m_blur_buffer != nullptr)
		delete m_blur_buffer;
	if(m_jitter != nullptr)
		delete m_jitter;
}

bool RenderSubsystem::startUp(GLFWwindow * window)
{
	if(!m_initialized)
	{
		m_window = window;
		m_shader = new RenderShader();
		m_text_shader = new TextShader();
		m_image_shader = new ImageShader();
		m_pp_shader = new PostProcessingShader();
		m_shadow_shader = new RenderBaseShader();
		m_point_shadow_shader = new RenderBaseShader();
		m_skybox_shader = new SkyboxShader();

		m_gbuffer = new GBuffer();	
		m_pp_buffer = new RenderBuffer();
		m_light_buffer = new RenderBuffer();
		m_blur_buffer = new RenderBuffer();							
		m_render_quad = new Quad();
		m_jitter =  new JitterTexture();

	  if( ! m_shader->load("shaders/rendershaderV1") )
			return false;
		if( ! m_text_shader->load("shaders/textshaderV1") )
			return false;
		if( ! m_image_shader->load("shaders/imageshaderV1") )
			return false;
		if( ! m_pp_shader->load("shaders/ppshaderV1") )
			return false;
		if( ! m_shadow_shader->load("shaders/shadowshaderV1") )
			return false;
		if( ! m_point_shadow_shader->load("shaders/pointshadowshaderV1") )
			return false;
		if( ! m_skybox_shader->load("shaders/skyboxshaderV1") )
			return false;
		if( ! m_gbuffer->initialize() )
			return false;
		if( ! m_render_quad->load() )
			return false;
		if( !m_pp_buffer->initialize())
			return false;
		if( !m_light_buffer->initialize())
			return false;
		if( !m_blur_buffer->initialize())
			return false;
		if( !m_jitter->load())
			return false;
		
		m_pp_shader->use();
		m_pp_shader->setFXAA(true);		
		m_pp_shader->setHDR(false);		
		m_enable_hdr = false;


		m_shader->use();
		m_shader->setShadows(ST_STANDARD_RS);

		m_shadows_standard_enabled = true;

		m_pp_shader->setGaussSize(20);

		//glEnable(GL_FRAMEBUFFER_SRGB); 

		Engine::getLog()->log("RenderSubsystem", "started");
		m_initialized = true;
		return true;
	}
	return false;
}
bool RenderSubsystem::shutDown()
{
	if(m_initialized)
	{
		Engine::getLog()->log("RenderSubsystem", "shut down");
		return true;
	}
	return false;
}

void RenderSubsystem::startRender()
{
	if(m_initialized)
	{
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		m_shader->reset();
	}
}

void RenderSubsystem::renderPassGBuffer()
{
 	m_shader->use();
	m_gbuffer->bindForInput();

 	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glEnable(GL_DEPTH_TEST);

 	m_shader->setRenderPass(1);

 	Camera * cam = Engine::getScene()->getCamera();
 	m_shader->setProjectionMatrix(cam->getProjection());
 	m_shader->setViewMatrix(cam->getView());

 	Scene * scene = Engine::getScene();

	if(scene != nullptr)
		scene->render(m_shader, m_skybox_shader);
	
	glFinish();
	m_gbuffer->unbindFromInput();
}

void RenderSubsystem::renderPassShadow()
{
	m_shadow_shader->use();

  Scene * scene = Engine::getScene();

	if(scene != nullptr)
		scene->renderShadow(m_shadow_shader,m_point_shadow_shader);
}

bool RenderSubsystem::refreshShaders()
{
	if(m_shader != nullptr)
		delete m_shader;
	if(m_text_shader != nullptr)
		delete m_text_shader;
	if(m_image_shader != nullptr)
		delete m_image_shader;
	if(m_pp_shader != nullptr)
		delete m_pp_shader;
	if(m_shadow_shader != nullptr)
		delete m_shadow_shader;
	if(m_point_shadow_shader != nullptr)
		delete m_point_shadow_shader;
	if(m_skybox_shader != nullptr)
		delete m_skybox_shader;
	m_shader = new RenderShader();
	m_text_shader = new TextShader();
	m_image_shader = new ImageShader();
	m_pp_shader = new PostProcessingShader();
	m_shadow_shader = new RenderBaseShader();
	m_point_shadow_shader = new RenderBaseShader();
	m_skybox_shader = new SkyboxShader();
	if( ! m_shader->load("shaders/rendershaderV1") )
		return false;
	if( ! m_text_shader->load("shaders/textshaderV1") )
		return false;
	if( ! m_image_shader->load("shaders/imageshaderV1") )
		return false;
	if( ! m_pp_shader->load("shaders/ppshaderV1") )
		return false;
	if( ! m_shadow_shader->load("shaders/shadowshaderV1") )
		return false;
	if( ! m_point_shadow_shader->load("shaders/pointshadowshaderV1") )
		return false;
	if( ! m_skybox_shader->load("shaders/skyboxshaderV1") )
		return false;

	m_pp_shader->setGaussSize(20);


	return true;
}

void RenderSubsystem::renderPassLight()
{
 	m_shader->use();
	m_gbuffer->unbindFromInput();
	m_gbuffer->bindForOutput();

	m_pp_buffer->bindForInput();

	m_shader->setRenderPass(2);
	m_jitter->bind(TEXTURE_JITTER);
	
	Scene * scene = Engine::getScene();

	if(m_shadows_standard_enabled)
		scene->setupLightsForShadingPass(m_shader);


	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glDisable(GL_DEPTH_TEST);

  m_shader->setIdentityMatrices();
  m_shader->setAllMaterialsForRenderPass();

	m_render_quad->render();	

	glFinish();

	m_pp_buffer->unbindFromInput();
}
void RenderSubsystem::renderPassPostProcess()
{
	m_pp_shader->use();
	m_pp_shader->setRenderPass(1);
	m_pp_buffer->bindForOutput();
	m_blur_buffer->bindForOutput(1);
	m_pp_shader->setExposure(Engine::getScene()->getCamera()->getExposure());
	if(m_enable_hdr)
	{
		//float average_luminance = m_pp_buffer->getAverageLuminance();
		//m_pp_shader->setAverageLuminance(average_luminance);
		//m_pp_shader->setExposure(Engine::getScene()->getCamera()->getExposure());
	}

	m_pp_shader->setIdentityMatrices();
	m_pp_shader->setTextureScaling( glm::vec2(1.0f /Engine::getWindowWidth(),1.0f /Engine::getWindowHeight()) );

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glDisable(GL_DEPTH_TEST);

  m_render_quad->render();	

	glFinish();
}

void RenderSubsystem::renderPassLightBlur()
{
	// light 
	m_pp_shader->use();
	m_pp_shader->setRenderPass(2);
	m_pp_buffer->bindForOutput();
	m_light_buffer->bindForInput();

	m_pp_shader->setIdentityMatrices();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glDisable(GL_DEPTH_TEST);

  m_render_quad->render();	

	glFinish();	
	m_light_buffer->unbindFromInput();

	// blur
	m_light_buffer->bindForOutput();
	m_blur_buffer->bindForInput();

	m_pp_shader->setRenderPass(3);
	m_pp_shader->setIdentityMatrices();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glDisable(GL_DEPTH_TEST);

  m_render_quad->render();	  

	glFinish();
	m_blur_buffer->unbindFromInput();

	// blur2 
	m_blur_buffer->bindForOutput();
	m_light_buffer->bindForInput();

	m_pp_shader->setRenderPass(4);
	m_pp_shader->setIdentityMatrices();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glDisable(GL_DEPTH_TEST);

  m_render_quad->render();	  

	glFinish();
	m_light_buffer->unbindFromInput();



}

void RenderSubsystem::renderUI()
{
	m_text_shader->use();
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);	
	m_text_shader->setProjection();

	std::vector<GUI* > * guis = Engine::getGUIs();
	for( std::vector<GUI*>::iterator it = guis->begin(); it != guis->end();it++)
		(*it)->render(m_text_shader,m_image_shader, m_render_quad);

	Engine::getEngineGUI()->render(m_text_shader,m_image_shader, m_render_quad);

	glDisable(GL_BLEND);
	glFinish();
}

void RenderSubsystem::render()
{
	
	startRender();
	if( Engine::getScene() != nullptr)
	{
		renderPassGBuffer();
		if(m_shadows_standard_enabled)
			renderPassShadow();
		renderPassLight();
		renderPassLightBlur();
		renderPassPostProcess();
	}
	renderUI();

	endRender();

}

void RenderSubsystem::endRender()
{
	if(m_initialized)
	{		
		glfwSwapBuffers(m_window);
	}
}

bool RenderSubsystem::systemCheck()
{
	m_skybox_shader->use();
	return m_initialized;
}

RenderShader * RenderSubsystem::getRenderShader()
{
	return m_shader;
}
TextShader * RenderSubsystem::getTextShader()
{
	return m_text_shader;
}


void RenderSubsystem::setPostProcessing(PostprocessType type, bool enable)
{
	m_pp_shader->use();
	if( type == PP_FXAA)
		m_pp_shader->setFXAA(enable);
	if( type == PP_HDR)
	{
		m_pp_shader->setHDR(enable);
		m_enable_hdr = enable;
	}
}
void RenderSubsystem::setShadowTechnique(ShadowTechniqueType type)
{
	m_shader->use();
	m_shader->setShadows(type);
	if(type == ST_NONE)
		m_shadows_standard_enabled = false;
	else
		m_shadows_standard_enabled = true;
}

Object * RenderSubsystem::pickObjectAtCenter()
{
	int id  = m_gbuffer->pickObjectAtCenter();
	return Engine::getScene()->getObject(id);
}
Object * RenderSubsystem::pickObjectAt(glm::vec2 p)
{
	int id  = m_gbuffer->pickObjectAt(p);
	return  Engine::getScene()->getObject(id);
}