#include "RenderSubsystem.h"

#include "Log.h"
#include "Camera.h"
#include "Scene.h"

#include "DirectionalShadowBuffer.h"
#include "RenderBuffer.h"
#include "GBuffer.h"

#include "RenderShader.h"
#include "TextShader.h"
#include "ImageShader.h"
#include "PostProcessingShader.h"
#include "ShadowShader.h"

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
	if(m_gbuffer != nullptr)
		delete m_gbuffer;
	if(m_render_quad != nullptr)
		delete m_render_quad;
	if(m_shadow_buffer != nullptr)
		delete m_shadow_buffer;
	if(m_pp_buffer != nullptr)
		delete m_pp_buffer;
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
		m_shadow_shader = new ShadowShader();

		m_gbuffer = new GBuffer();	
		m_pp_buffer = new RenderBuffer();		
		m_render_quad = new Quad();

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
		if( ! m_gbuffer->initialize() )
			return false;
		if( ! m_render_quad->load() )
			return false;
		if( !m_pp_buffer->initialize())
			return false;
		m_shadow_buffer = new DirectionalShadowBuffer();
		if( !m_shadow_buffer->initialize(1240, 720))
			return false;

		m_pp_shader->use();
		m_pp_shader->setFXAA(true);

		m_shader->use();
		m_shader->setPCF(true);
		m_shader->setStandardShadows(true);

		m_shadows_standard_enabled = true;


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
		scene->render(m_shader);
	glFinish();
	m_gbuffer->unbindFromInput();
}
void RenderSubsystem::renderPassShadow()
{
	m_shadow_shader->use();

  Scene * scene = Engine::getScene();

	if(scene != nullptr)
		scene->renderShadow(m_shadow_shader);
}

void RenderSubsystem::renderPassLight()
{
 	m_shader->use();
	m_gbuffer->unbindFromInput();
	m_gbuffer->bindForOutput();

	m_pp_buffer->bindForInput();

	m_shader->setRenderPass(2);
	
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
	m_pp_buffer->bindForOutput();

	m_pp_shader->setIdentityMatrices();
	m_pp_shader->setTextureScaling( glm::vec2(1.0f /Engine::getWindowWidth(),1.0f /Engine::getWindowHeight()) );

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glDisable(GL_DEPTH_TEST);

  m_render_quad->render();	

	glFinish();
}
void RenderSubsystem::renderPassTest()
{
	m_shader->use();
	m_shadow_buffer->bindForOutput(0);
	//m_pp_buffer->bindForOutput();
	m_shader->setRenderPass(2);
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glDisable(GL_DEPTH_TEST);

  m_shader->setIdentityMatrices();
  m_shader->setAllMaterialsForRenderPass();


  m_render_quad->render();	

	glFinish();
}

void RenderSubsystem::renderUI()
{
	std::vector<GUI* > * guis = Engine::getGUIs();
	for( std::vector<GUI*>::iterator it = guis->begin(); it != guis->end();it++)
		(*it)->render(m_text_shader,m_image_shader, m_render_quad);
	Engine::getEngineGUI()->render(m_text_shader,m_image_shader, m_render_quad);
}

void RenderSubsystem::render()
{
	startRender();
	renderPassGBuffer();
	if(m_shadows_standard_enabled)
		renderPassShadow();
	renderPassLight();
	renderPassPostProcess();
	//renderPassTest();

	m_text_shader->use();
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);	
	m_text_shader->setProjection();
	renderUI();
	glDisable(GL_BLEND);
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
}
void RenderSubsystem::setShadowTechnique(ShadowTechniqueType type, bool enable)
{
	m_shader->use();
	if( type == ST_STANDARD_PCF)
		m_shader->setPCF(enable);
	if( type == ST_STANDARD)
	{
		m_shadows_standard_enabled = enable;
		if(enable)
		{
			Engine::getLog()->log("RenderSubsystem", "Enabled standard shadows");
			m_shader->setStandardShadows(true);
		}
		else
		{
			m_shader->setStandardShadows(false);
			Engine::getLog()->log("RenderSubsystem", "Disabled standard shadows");
		}
	}
}