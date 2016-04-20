#include "RenderSubsystem.h"

#include "Log.h"
#include "Camera.h"
#include "Scene.h"
#include "RenderShader.h"
#include "ShadowBuffer.h"
#include "RenderBuffer.h"
#include "TextShader.h"
#include "ImageShader.h"
#include "PostProcessingShader.h"
#include "GBuffer.h"
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
	delete m_shader;
	delete m_text_shader;
	delete m_image_shader;
	delete m_pp_shader;
	delete m_gbuffer;
	delete m_render_quad;
	//delete m_shadow_buffer;
	delete m_fxaa_buffer;
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

		m_gbuffer = new GBuffer();	
		m_fxaa_buffer = new RenderBuffer();		
		m_render_quad = new Quad();

	  if( ! m_shader->load("shaders/rendershaderV1") )
			return false;
		if( ! m_text_shader->load("shaders/textshaderV1") )
			return false;
		if( ! m_image_shader->load("shaders/imageshaderV1") )
			return false;
		if( ! m_pp_shader->load("shaders/ppshaderV1") )
			return false;
		if( ! m_gbuffer->initialize() )
			return false;
		if( ! m_render_quad->load() )
			return false;
		if( !m_fxaa_buffer->initialize())
			return false;
		//m_shadow_buffer = new ShadowBuffer();
		//if( !m_shadow_buffer->initialize(800,600))
	//		return false;

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

void RenderSubsystem::renderPassOne()
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
}
void RenderSubsystem::renderPassShadow()
{
/*	m_shader->use();
	m_shadow_buffer->bindForInput();
	m_shader->setRenderPass(3);

	//Transform trans;
 	//m_shader->setProjectionMatrix(cam->getProjection());
 //	m_shader->setViewMatrix(cam->getView());

 	glEnable(GL_CULL_FACE);
  glCullFace(GL_FRONT);

  Scene * scene = Engine::getScene();

	if(scene != nullptr)
		scene->render(m_shader);

	glDisable(GL_CULL_FACE);
	glFinish();*/

}

void RenderSubsystem::renderPassTwo()
{
 	m_shader->use();
	m_gbuffer->unbindFromInput();
	m_gbuffer->bindForOutput();

	m_fxaa_buffer->bindForInput();

	m_shader->setRenderPass(2);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glDisable(GL_DEPTH_TEST);

  m_shader->setIdentityMatrices();
  m_shader->setAllMaterialsForRenderPass();

	m_render_quad->render();	

	glFinish();

	m_fxaa_buffer->unbindFromInput();
}
void RenderSubsystem::renderPassFXAA()
{
	m_pp_shader->use();
	m_fxaa_buffer->bindForOutput();

	//m_pp_shader->setRenderPass();
	m_pp_shader->setIdentityMatrices();
	m_pp_shader->setTextureScaling( glm::vec2(1.0f /Engine::getWindowWidth(),1.0f /Engine::getWindowHeight()) );

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glDisable(GL_DEPTH_TEST);

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
	renderPassOne();
	//renderPassShadow();
	renderPassTwo();
	renderPassFXAA();

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