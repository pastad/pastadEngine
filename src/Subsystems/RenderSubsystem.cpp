#include "RenderSubsystem.h"

#include "Log.h"
#include "Camera.h"
#include "Scene.h"

#include "DirectionalShadowBuffer.h"
#include "RenderBuffer.h"
#include "TransparentBuffer.h"
#include "GBuffer.h"
#include "JitterTexture.h"
#include "NoiseTexture.h"
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
#include "SceneEditor.h"

#include "Helper.h"

#include <sstream>
#include <iostream>

RenderSubsystem::RenderSubsystem():Subsystem("RenderSubsystem")
{	
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
	if(m_ssao_buffer != nullptr)
		delete m_ssao_buffer;
  if (m_transparent_buffer != nullptr)
    delete m_transparent_buffer;
	if(m_jitter != nullptr)
		delete m_jitter;
  if (m_noise_texture != nullptr)
    delete m_noise_texture;
	if(m_terrain_shader != nullptr)
		delete m_terrain_shader;
  if(m_water_shader != nullptr)
    delete m_water_shader;  
}

bool RenderSubsystem::startUp(GLFWwindow * window)
{
	if(! m_initialized)
	{
//EDIT		m_window = window;
		m_shader = new RenderShader();
		m_text_shader = new TextShader();
		m_image_shader = new ImageShader();
		m_pp_shader = new PostProcessingShader();
		m_shadow_shader = new RenderBaseShader();
		m_point_shadow_shader = new RenderBaseShader();
		m_terrain_shader = new RenderBaseShader();
    m_water_shader = new RenderBaseShader();
		m_skybox_shader = new SkyboxShader();

		m_gbuffer = new GBuffer();	
		m_pp_buffer = new RenderBuffer();
		m_light_buffer = new RenderBuffer();
		m_blur_buffer = new RenderBuffer();		
		m_ssao_buffer = new RenderBuffer();					
    m_transparent_buffer = new TransparentBuffer();
		m_render_quad = new Quad();
		m_jitter =  new JitterTexture();
    m_noise_texture = new NoiseTexture();

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
		if( ! m_terrain_shader->load("shaders/terrainshaderV1") )
			return false;
    if( ! m_water_shader->load("shaders/watershaderV1") )
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
		if( !m_ssao_buffer->initialize())
			return false;
    if (!m_transparent_buffer->initialize())
      return false;
		if( !m_jitter->load())
			return false;
    if (!m_noise_texture->create())
      return false;
		
    if(Engine::isInEditMode())
    {
  		m_pp_shader->use();
  		m_pp_shader->setFXAA(true);		
  		m_pp_shader->setHDR(true);	
  		m_pp_shader->setBloom(true);	
    }	

		m_shader->use();
		m_shader->setShadowsDirectional(ST_STANDARD_PCF);
    m_shader->setShadowsPoint(ST_STANDARD_PCF);
    m_shader->setSSAO(true);
    m_shadow_ssao = true;
    m_shadow_techniques = ST_STANDARD_PCF;
    m_shadow_technique_point = ST_STANDARD_PCF;

		m_shadows_standard_directional_enabled = m_shadows_standard_point_enabled = true;

		m_pp_shader->setGaussSize(10);
		m_pp_shader->setBloomThreshold(0.8f);
		m_pp_shader->setSSAOSamples();

		//glEnable(GL_FRAMEBUFFER_SRGB); 

		Engine::getLog()->log("RenderSubsystem", "started");
		m_initialized = true;
		return true;
	}
	return false;
}



// helpers ------------------------------------------

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
	if(m_terrain_shader != nullptr)
		delete m_terrain_shader;
  if(m_water_shader != nullptr)
    delete m_water_shader;

	m_shader = new RenderShader();
	m_text_shader = new TextShader();
	m_image_shader = new ImageShader();
	m_pp_shader = new PostProcessingShader();
	m_shadow_shader = new RenderBaseShader();
	m_point_shadow_shader = new RenderBaseShader();
	m_skybox_shader = new SkyboxShader();
	m_terrain_shader = new RenderBaseShader();
	m_water_shader = new RenderBaseShader();

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
	if( ! m_terrain_shader->load("shaders/terrainshaderV1") )
		return false;
	if( ! m_water_shader->load("shaders/watershaderV1") )
		return false;

	m_pp_shader->setGaussSize(20);
	m_pp_shader->setBloomThreshold(0.6f);
	m_pp_shader->setSSAOSamples();

  setShadowTechniqueDirectional(m_shadow_techniques);
  setShadowTechniquePoint(m_shadow_technique_point);

  m_pp_shader->use();
  if (m_pp_techniques | PP_FXAA)
    m_pp_shader->setFXAA(true);
  if (m_pp_techniques | PP_HDR)
  {
    m_pp_shader->setHDR(true);
    m_enable_hdr = true;
  }
  if (m_pp_techniques | PP_BLOOM)
  {
    m_pp_shader->setBloom(true);
    m_enable_bloom = true;
  } 

	return true;
}


// render passes ------------------------------------------

void RenderSubsystem::renderPassGBuffer()
{

  m_shader->use();
  m_gbuffer->bindForInput();
 
  gl::Clear(gl::COLOR_BUFFER_BIT | gl::DEPTH_BUFFER_BIT);
  gl::Enable(gl::DEPTH_TEST);

  Camera * cam = Engine::getScene()->getCamera();
  m_shader->setProjectionMatrix(cam->getProjection());
  m_shader->setViewMatrix(cam->getView());

  m_terrain_shader->use();
  m_terrain_shader->setProjectionMatrix(cam->getProjection());
  m_terrain_shader->setViewMatrix(cam->getView());

  m_water_shader->use();
  m_water_shader->setProjectionMatrix(cam->getProjection());
  m_water_shader->setViewMatrix(cam->getView());
  m_water_shader->setUniform("Delta", Engine::getScene()->getTime());

  gl::Enable(gl::BLEND);
  gl::BlendFunc(gl::SRC_ALPHA, gl::ONE_MINUS_SRC_ALPHA);


  m_shader->use();
  Scene * scene = Engine::getScene();

  m_shader->setRenderPass(1);

  if(scene != nullptr)
    scene->render(m_shader, m_skybox_shader, m_terrain_shader,m_water_shader, false);

  gl::Disable(gl::BLEND);

  gl::Finish();
  m_gbuffer->unbindFromInput();
  //Engine::getLog()->log("RenderSubsystem", "gbuffer passs");

}

void RenderSubsystem::renderPassLight()
{
  m_shader->use();
  m_gbuffer->unbindFromInput();
  m_gbuffer->bindForOutput();

  m_pp_buffer->bindForInput();
  //glBindFramebuffer(GL_FRAMEBUFFER, 0);

  m_jitter->bind(TEXTURE_JITTER);

  Scene * scene = Engine::getScene();

  //if(m_shadows_standard_enabled  )
    scene->setupLightsForShadingPass(m_shader);


  gl::Clear(gl::COLOR_BUFFER_BIT | gl::DEPTH_BUFFER_BIT);
  gl::Disable(gl::DEPTH_TEST);


  gl::Enable(gl::BLEND);
  gl::BlendFunc(gl::SRC_ALPHA, gl::ONE_MINUS_SRC_ALPHA);

  m_shader->setIdentityMatrices();
  m_shader->setAllMaterialsForRenderPass();

  m_ssao_buffer->bindForOutput(5);
  //m_transparent_buffer->bindForOutput(6);

  m_shader->setRenderPass(2);

  m_render_quad->render();  

  gl::Disable(gl::BLEND);

  gl::Finish();

  m_pp_buffer->unbindFromInput();
  //Engine::getLog()->log("RenderSubsystem", "light pass");
}

void RenderSubsystem::renderPassTransparent()
{
  m_shader->use();

  m_shader->setGBufferSize(glm::vec2(Engine::getWindowWidth(), Engine::getWindowHeight()));
  m_gbuffer->bindForOutput(6);
  m_transparent_buffer->bindForInput();

  gl::ClearColor(0.0f, 0.0f, 0.0f, 0.0f);
  gl::Clear(gl::COLOR_BUFFER_BIT | gl::DEPTH_BUFFER_BIT);
  gl::Enable(gl::DEPTH_TEST);


  Camera * cam = Engine::getScene()->getCamera();
  m_shader->setProjectionMatrix(cam->getProjection());
  m_shader->setViewMatrix(cam->getView());
  m_shader->setCameraPosition(cam->getPosition());

  gl::Enable(gl::BLEND); 
  gl::BlendFunc(gl::SRC_ALPHA, gl::ONE_MINUS_SRC_ALPHA);


  Scene * scene = Engine::getScene();

  m_shader->setRenderPass(4);

  if (scene != nullptr)
    scene->render(m_shader, nullptr, nullptr, nullptr, true);

  gl::Finish();
  m_transparent_buffer->unbindFromInput();
  //Engine::getLog()->log("RenderSubsystem", "gbuffer passs");

  gl::Disable(gl::BLEND);
  gl::Disable(gl::DEPTH_TEST);
}

void RenderSubsystem::renderPassShadow()
{
  gl::CullFace(gl::FRONT);
  m_shadow_shader->use();

  Scene * scene = Engine::getScene();

  if(scene != nullptr)
    scene->renderShadow(m_shadow_shader,m_point_shadow_shader, m_shadows_standard_directional_enabled, m_shadows_standard_point_enabled);
  //Engine::getLog()->log("RenderSubsystem", "shadow pass");

   gl::CullFace(gl::BACK);
}

// ppps

void RenderSubsystem::renderPassPostProcess()
{
	m_pp_shader->use();
	
	Camera * cam = Engine::getScene()->getCamera();
 	m_pp_shader->setProjectionMatrix(cam->getProjection());
 	m_pp_shader->setViewMatrix(cam->getView());

	m_pp_buffer->bindForOutput(0);
	m_light_buffer->bindForOutput(1);
	m_ssao_buffer->bindForOutput(2);


	m_pp_shader->setExposure(Engine::getScene()->getCamera()->getExposure());
//	if(m_enable_hdr)
//	{
		//float average_luminance = m_pp_buffer->getAverageLuminance();
		//m_pp_shader->setAverageLuminance(average_luminance);
//	}

	m_pp_shader->setIdentityMatrices();
	m_pp_shader->setTextureScaling( glm::vec2(1.0f /Engine::getWindowWidth(),1.0f /Engine::getWindowHeight()) );

	gl::Clear(gl::COLOR_BUFFER_BIT | gl::DEPTH_BUFFER_BIT);
    gl::Disable(gl::DEPTH_TEST);

    m_pp_shader->setRenderPass(PASS_STANDARD);
    m_render_quad->render();

	gl::Finish();
    //Engine::getLog()->log("RenderSubsystem", "pp pass");
}

void RenderSubsystem::renderPassLightBlur()
{
	// light 
	m_pp_shader->use();
	m_pp_shader->setExposure(Engine::getScene()->getCamera()->getExposure());
  m_pp_shader->setEmmissiveIndices(m_shader->getEmmissiveMaterialIndices());
	
	m_pp_buffer->bindForOutput();  
  m_gbuffer->bindMaterialTextureForOutput(1);
	m_light_buffer->bindForInput();

	m_pp_shader->setIdentityMatrices();

  m_pp_shader->setRenderPass(PASS_BRIGHT);
	gl::Clear(gl::COLOR_BUFFER_BIT | gl::DEPTH_BUFFER_BIT);
  gl::Disable(gl::DEPTH_TEST);

  // render the bright regions

  m_render_quad->render();	

	gl::Finish();	
	m_light_buffer->unbindFromInput();

	// blur
	m_light_buffer->bindForOutput();
	m_blur_buffer->bindForInput();

	
	m_pp_shader->setIdentityMatrices();

  m_pp_shader->setRenderPass(PASS_BLUR_1);
	gl::Clear(gl::COLOR_BUFFER_BIT | gl::DEPTH_BUFFER_BIT);
  gl::Disable(gl::DEPTH_TEST);
 
  m_render_quad->render();	  

	gl::Finish();
	m_blur_buffer->unbindFromInput();

	// blur2 
	m_blur_buffer->bindForOutput();
	m_light_buffer->bindForInput();

	
	m_pp_shader->setIdentityMatrices();

	gl::Clear(gl::COLOR_BUFFER_BIT | gl::DEPTH_BUFFER_BIT);
  gl::Disable(gl::DEPTH_TEST);

  m_pp_shader->setRenderPass(PASS_BLUR_2);
  m_render_quad->render();	  

	gl::Finish();
	m_light_buffer->unbindFromInput();

}

void RenderSubsystem::renderSSAO()
{
	m_pp_shader->use();
  m_pp_shader->setRenderPass(PASS_SSAO);

  Camera * cam = Engine::getScene()->getCamera();
  m_pp_shader->setProjectionMatrix(cam->getProjection());
  m_pp_shader->setViewMatrix(cam->getView());
  m_pp_shader->setCameraPosition(cam->getPosition());

  m_gbuffer->bindForOutput();
  m_noise_texture->bind(5);
 // m_transparent_buffer->bindForOutput(6);
  m_ssao_buffer->bindForInput();
 
	gl::Clear(gl::COLOR_BUFFER_BIT | gl::DEPTH_BUFFER_BIT);
  gl::Disable(gl::DEPTH_TEST);
 
  m_render_quad->render();	  

	gl::Finish();

	m_ssao_buffer->unbindFromInput();
  
  // blur 1

  m_pp_shader->setRenderPass(PASS_BLUR_1);

  m_pp_shader->setIdentityMatrices();
  m_pp_shader->setTextureScaling(glm::vec2(1.0f / (float)Engine::getWindowWidth(), 1.0f / (float)Engine::getWindowHeight()));

  m_ssao_buffer->bindForOutput(0);

  m_blur_buffer->bindForInput();

  gl::Clear(gl::COLOR_BUFFER_BIT | gl::DEPTH_BUFFER_BIT);
  gl::Disable(gl::DEPTH_TEST);

  m_render_quad->render();

  gl::Finish();
  m_blur_buffer->unbindFromInput();

  // blur 2

  m_pp_shader->setIdentityMatrices();

  m_pp_shader->setRenderPass(PASS_BLUR_2);

  m_blur_buffer->bindForOutput(0);
  m_ssao_buffer->bindForInput();

  gl::Clear(gl::COLOR_BUFFER_BIT | gl::DEPTH_BUFFER_BIT);
  gl::Disable(gl::DEPTH_TEST);

  m_render_quad->render();

  gl::Finish();

  m_ssao_buffer->unbindFromInput();
}

void RenderSubsystem::renderUI()
{
	m_text_shader->use();
	gl::Enable(gl::BLEND);
	gl::BlendFunc(gl::SRC_ALPHA, gl::ONE_MINUS_SRC_ALPHA);
	m_text_shader->setProjection();

	std::vector<GUI* > * guis = Engine::getGUIs();
	for( std::vector<GUI*>::iterator it = guis->begin(); it != guis->end();it++)
		(*it)->render(m_text_shader,m_image_shader, m_render_quad);

	if(Engine::getEngineGUI() != nullptr)
		Engine::getEngineGUI()->render(m_text_shader,m_image_shader, m_render_quad);
	if(Engine::getSceneEditor() != nullptr)
		Engine::getSceneEditor()->render(m_text_shader,m_image_shader, m_render_quad);

	gl::Disable(gl::BLEND);
	gl::Finish();
	//Engine::getLog()->log("RenderSubsystem", "ui pass");
}


// basic render methods ------------------------------------------

void RenderSubsystem::startRender()
{
  if(m_initialized)
  {
   // acquireRenderLock("RenderSubsystem");
  //  Engine::getLog()->log(LF_Main,"RenderSubsystem", "start render");
    gl::ClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    m_shader->reset();
    Helper::checkGLError("startRender");
  }
}

void RenderSubsystem::render(bool swap_buffer)
{
//	float now = float(glfwGetTime());
	//std::cout << "rb"<< now << std::endl;
	startRender();
	if( Engine::getScene() != nullptr)
	{
	//	now = float(glfwGetTime());
       // std::cout<<"pre gb"  << std::endl;
		renderPassGBuffer();
	//	now = float(glfwGetTime());
	//	std::cout << "pre sb" << float(glfwGetTime())- now  << std::endl;
		if(m_shadows_standard_directional_enabled || m_shadows_standard_point_enabled)
			renderPassShadow();
	//	now = float(glfwGetTime());
	//	std::cout << now << std::endl;

   // renderPassTransparent();

    renderSSAO();   

		renderPassLight();


	//	now = float(glfwGetTime());
		//std::cout << now << std::endl;
		if(m_enable_bloom)
			renderPassLightBlur();
	  //now = float(glfwGetTime());
		//std::cout << now << std::endl;

		renderPassPostProcess();
		//now = float(glfwGetTime());
		//std::cout <<"rb end "<< now << std::endl;
	}
	renderUI();

    endRender(swap_buffer);

}

void RenderSubsystem::endRender(bool swap_buffer)
{
	if(m_initialized)
    {
        //if(swap_buffer)
        //    glfwSwapBuffers(m_window);
		m_shader->reset();

    Helper::checkGLError("end Render");
   // Engine::getLog()->log(LF_Main, "RenderSubsystem", "end render");
   // releaseRenderLock("RenderSubsystem");
	}
}


// shader getters ------------------------------------------

RenderShader * RenderSubsystem::getRenderShader()
{
	return m_shader;
}

TextShader * RenderSubsystem::getTextShader()
{
	return m_text_shader;
}


// technique setters ------------------------------------------

void RenderSubsystem::setPostProcessing(PostprocessType type, bool enable)
{
  if (enable)  
    m_pp_techniques != type;  
  else  
  {
    unsigned int  t  = m_pp_techniques & (!(unsigned int)type);
    m_pp_techniques = (PostprocessType) t;
  }
  

	m_pp_shader->use();
	if( type == PP_FXAA)
		m_pp_shader->setFXAA(enable);
	if( type == PP_HDR)
	{
		m_pp_shader->setHDR(enable);
		m_enable_hdr = enable;
	}
	if( type == PP_BLOOM)
	{
		m_pp_shader->setBloom(enable);
		m_enable_bloom = enable;
	}
}
void RenderSubsystem::setShadowTechniqueDirectional(ShadowTechniqueType type)
{
  m_shadow_techniques = type;
	m_shader->use();
	m_shader->setShadowsDirectional(type);
	if(type == ST_NONE)
		m_shadows_standard_directional_enabled = false;
	else
    m_shadows_standard_directional_enabled = true;
}
void RenderSubsystem::setShadowTechniquePoint(ShadowTechniqueType type)
{
  m_shadow_techniques = type;
  m_shader->use();
  m_shader->setShadowsPoint(type);
  if (type == ST_NONE)
    m_shadows_standard_point_enabled = false;
  else
    m_shadows_standard_point_enabled = true;
}
void RenderSubsystem::setShadowSSAO(ShadowTechniqueType type)
{
  if (type == ST_NONE)
    m_shadow_ssao = false;
  else
    m_shadow_ssao = true;
  m_shader->use();
  m_shader->setSSAO(m_shadow_ssao);

}


// pick methods ------------------------------------------

Object * RenderSubsystem::pickObjectAtCenter()
{
	int id  = m_gbuffer->pickObjectAtCenter();
	return Engine::getScene()->getObject(id);
}

Object * RenderSubsystem::pickObjectAt(glm::vec2 p)
{
	int id  = m_gbuffer->pickObjectAt(p);
/*	std::stringstream ss;
	ss << id;
	Engine::getLog()->log("RenderSubsystem", "id", ss.str());*/
	return  Engine::getScene()->getObject(id);
}

Light * RenderSubsystem::pickLightAt(glm::vec2 p)
{
	int id  = m_gbuffer->pickObjectAt(p);
	return  Engine::getScene()->getLight(id);
}

// lock methods ------------------------------------------

void RenderSubsystem::acquireRenderLock(std::string who)
{
  Engine::getLog()->log(LF_TS, "RenderSubsystem","lock wanted by ", who);
 // std::cout << &m_render_mutex <<std::endl;
  try
  {  //m_render_mutex.lock();
  }
  catch (std::exception ex)
  {
    Engine::getLog()->log("EXCEPTION", "locking");
  }
  Engine::getLog()->log(LF_TS, "RenderSubsystem", "lock acquired by ", who);
}
void RenderSubsystem::releaseRenderLock(std::string who)
{
  try
  {  //m_render_mutex.unlock();
    }
  catch (std::exception ex)
  {
    Engine::getLog()->log("EXCEPTION", "locking");
  }
  Engine::getLog()->log(LF_TS, "RenderSubsystem", "lock released by ", who);
}
