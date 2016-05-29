#include "Light.h"
#include "Log.h"
#include "Engine.h"
#include "Scene.h"
#include "Camera.h"

#include "DirectionalShadowBuffer.h"
#include "PointShadowBuffer.h"
#include "RenderBaseShader.h"
#include "RenderShader.h"
#include "Model.h"
#include "Object.h"
#include "Helper.h"
#include "RessourceManager.h"

#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/vector_angle.hpp>

#include  <iostream>

unsigned int Light::m_num_point_lights = 0;
unsigned int Light::m_num_spot_lights = 0;
unsigned int Light::m_num_directional_lights = 0;

unsigned int Light::m_num_directional_shadows = 0;
unsigned int Light::m_num_point_shadows = 0;

unsigned int Light::m_light_index_counter = 1000;

Object * Light::m_point_object = nullptr;
Object * Light::m_spot_object = nullptr;
Object * Light::m_directional_object = nullptr;

Light::Light():m_type(LIGHT_NONE)
{
}

Light::~Light()
{
  if(m_directional_buffer != nullptr)
    delete m_directional_buffer;
  if(m_point_buffer != nullptr)
    delete m_point_buffer;
  if(getType() == LIGHT_DIRECTIONAL )
  {
    m_num_directional_lights--;
    if(m_shadow_enabled)
      m_num_directional_shadows--;
  }
  if(getType() == LIGHT_SPOT)
  {
    m_num_spot_lights--;
    if(m_shadow_enabled)
      m_num_directional_shadows--;
  }
  if(getType() == LIGHT_POINT )
  {
    m_num_point_lights--;
    if(m_shadow_enabled)
      m_num_point_shadows--;
  }
}

bool Light::setDirectional(glm::vec3 direction, glm::vec3 col_am ,glm::vec3 col_dif, glm::vec3 col_spec,float intensity, bool enable_shadow )
{

  m_num_directional_lights++;
  if( m_num_directional_lights < MAX_NUM_DIRECTIONAL_LIGHTS )
  {
    m_type = LIGHT_DIRECTIONAL;
    m_direction = direction;
    m_color_ambient = col_am;
    m_color_diffuse =col_dif;
    m_color_specular = col_spec;
    m_intensity = intensity;
    m_refresh_shadow =true;
    m_position = glm::vec3(0,0,0);
    m_id = getNextId();

    m_shadow_enabled = enable_shadow;
    if(enable_shadow)
    {
      if(m_num_directional_shadows+1 < MAX_NUM_DIRECTIONAL_SHADOWS)
      {
        m_directional_buffer = new DirectionalShadowBuffer();        
        m_num_directional_shadows++;
        if( !m_directional_buffer->initialize(Engine::getWindowWidth() *4.0f,
         Engine::getWindowHeight() *4.0f))
          return false;
      }
      else
        Engine::getLog()->log("Light", "Too many directional shadows allready enabled");
    }
    else
        Engine::getLog()->log("Light", "Too many directional lights allready set");

    m_model = RessourceManager::loadModel("resources/cylinder.obj",false);
    if(m_directional_object == nullptr)
    {
      m_directional_object = m_model->getInstance();
      m_directional_object->setScale(glm::vec3(0.2f,0.2f,0.2f));
    }
   

    
    return true;
  }

  return false;
}

bool Light::setPoint(glm::vec3 positon, glm::vec3 col_am ,glm::vec3 col_dif, glm::vec3 col_spec,float intensity,
                      float con ,float lin, float qua, bool enable_shadow )
{

  m_num_point_lights++;
  if( m_num_point_lights < MAX_NUM_POINT_LIGHTS )
  {
    m_type = LIGHT_POINT;
    m_position = positon;
    m_color_ambient = col_am;
    m_color_diffuse =col_dif;
    m_color_specular = col_spec;
    m_att_const = con;
    m_att_linear = lin;
    m_att_quadratic = qua;
    m_intensity = intensity;
    m_id = getNextId();

    m_shadow_enabled = enable_shadow;
    if(enable_shadow)
    {
      if(m_num_point_shadows+1 < MAX_NUM_POINT_SHADOWS)
      {
        m_point_buffer = new PointShadowBuffer();        
        
        m_num_point_shadows++;
        if( !m_point_buffer->initialize(1000, 1000))
          return false;
      }
      else
        Engine::getLog()->log("Light", "Too many point shadows allready enabled");
    }
    m_refresh_shadow =true;

    m_model = RessourceManager::loadModel("resources/sphere.obj",false);

    if(m_point_object == nullptr)
    {
      m_point_object = m_model->getInstance();
      m_point_object->setScale(glm::vec3(0.2f,0.2f,0.2f));
    }
    return true;
  }
  else
    Engine::getLog()->log("Light", "Too many point lights allready set");
  return false;
}
bool Light::setSpot(glm::vec3 position, glm::vec3 col_am ,glm::vec3 col_dif, glm::vec3 col_spec,float intensity,
                      float con ,float lin, float qua, float cutoffAngle, glm::vec2 rotation , bool enable_shadow)
{

  m_num_spot_lights++;
  if( m_num_spot_lights < MAX_NUM_SPOT_LIGHTS )
  {
    m_type = LIGHT_SPOT;
    m_position = position;
    m_color_ambient = col_am;
    m_color_diffuse =col_dif;
    m_color_specular = col_spec;
    m_att_const = con;
    m_att_linear = lin;
    m_att_quadratic = qua;
    m_intensity = intensity;

    m_rotation = rotation;
    rotate(glm::vec2(0.0f,0.0f));

    m_cutoff_angle = cutoffAngle;
    m_shadow_enabled = enable_shadow;
    m_id = getNextId();

    if(enable_shadow)
    {
      if(m_num_directional_shadows+1 < MAX_NUM_DIRECTIONAL_SHADOWS)
      {
        m_directional_buffer = new DirectionalShadowBuffer();        
        m_num_directional_shadows++;
        if( !m_directional_buffer->initialize(Engine::getWindowWidth(),
            Engine::getWindowHeight()  ) ) 
          return false;
      }
      else
        Engine::getLog()->log("Light", "Too many directional shadows allready enabled");
    }
    m_refresh_shadow =true;
    m_model = RessourceManager::loadModel("resources/cone.obj",false);
    if(m_spot_object == nullptr)
    {
      m_spot_object = m_model->getInstance();
      m_spot_object->setScale(glm::vec3(0.2f,0.2f,0.2f));
    }
   

    return true;
  }
  else
    Engine::getLog()->log("Light", "Too many spot lights allready set");
  return false;
}



unsigned int Light::getType()
{
    return m_type;
}

glm::vec3 Light::getDirection()
{
    return m_direction;
}

glm::vec3 Light::getAmbientColor()
{
    return m_color_ambient;
}
glm::vec3 Light::getDiffuseColor()
{
    return m_color_diffuse;
}
glm::vec3 Light::getSpecularColor()
{
    return m_color_specular;
}
float Light::getIntensity()
{
    return m_intensity;
}
float Light::getAttenuationConstant()
{
    return m_att_const;
}
float Light::getAttenuationLinear()
{
    return m_att_linear;
}
float Light::getAttenuationQuadratic()
{
    return m_att_quadratic;
}
glm::vec3 Light::getPosition()
{
    return m_position;
}
float Light::getCutoffAngle()
{
  return m_cutoff_angle;
}

void Light::bindForShadowRenderDirectional(RenderBaseShader * shadow_shader)
{
  if(  ( m_type == LIGHT_SPOT) || ( m_type == LIGHT_DIRECTIONAL) )
  {     
    shadow_shader->use();
    m_directional_buffer->bindForInput();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    glPolygonOffset(0.01f,0.01f);

    shadow_shader->setProjectionMatrix(getProjection());    
    shadow_shader->setViewMatrix(getView() ); 

    glViewport(0,0, m_directional_buffer->getWidth() ,m_directional_buffer->getHeight());
    glClearColor(1000,0,0,0);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);    

  }

}
void Light::bindForShadowRenderPoint( RenderBaseShader * point_shadow_shader, int iteration)
{
 
  if( m_type == LIGHT_POINT)
  { 
     
    point_shadow_shader->use();
    if( iteration == 0)
    {
      m_point_buffer->bindForInput(GL_TEXTURE_CUBE_MAP_POSITIVE_X);
      point_shadow_shader->setViewMatrix(getView(glm::vec3(1.0f,0.0f,0.0f), glm::vec3(0.0f, -1.0f, 0.0f)  ));      
    }
    if( iteration == 1)
    {
      m_point_buffer->bindForInput(GL_TEXTURE_CUBE_MAP_NEGATIVE_X);
      point_shadow_shader->setViewMatrix(getView(glm::vec3(-1.0f,0.0f,0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));      
    }
    if( iteration == 2)
    {
      m_point_buffer->bindForInput(GL_TEXTURE_CUBE_MAP_POSITIVE_Y);
      point_shadow_shader->setViewMatrix(getView(glm::vec3(0.0f,1.0f,0.0f), glm::vec3(0.0f, 0.0f, 1.0f)));      
    }
    if( iteration == 3)
    {
      m_point_buffer->bindForInput(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y);
      point_shadow_shader->setViewMatrix(getView(glm::vec3(0.0f,-1.0f,0.0f), glm::vec3(0.0f, 0.0f, -1.0f) ));      
    }
    if( iteration == 4)
    {
      m_point_buffer->bindForInput(GL_TEXTURE_CUBE_MAP_POSITIVE_Z);
      point_shadow_shader->setViewMatrix(getView(glm::vec3(0.0f,0.0f,1.0f), glm::vec3(0.0f, -1.0f, 0.0f)));      
    }
    if( iteration == 5)
    {
      m_point_buffer->bindForInput(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z);
      point_shadow_shader->setViewMatrix(getView(glm::vec3(0.0f,0.0f,-1.0f), glm::vec3(0.0f, -1.0f, 0.0f)));      
    }  
   
    glPolygonOffset(0.01f,0.01f);

    point_shadow_shader->setProjectionMatrix(getProjection());  
    point_shadow_shader->setUniform("LightPosition",getPosition());  
    glViewport(0,0, 1000 ,1000);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);
    
  }
  
}

void Light::unbindFromShadowRender()
{

  if( ( getType() == LIGHT_SPOT) || ( getType() == LIGHT_DIRECTIONAL) )
  {
    glFlush();
    glFinish();
    glPolygonOffset(0.0f,0.0f);
    glDisable(GL_CULL_FACE);
    m_directional_buffer->unbindFromInput();
    glViewport(0,0, Engine::getWindowWidth(),Engine::getWindowHeight());
  }
  if( getType() == LIGHT_POINT)
  {    
    glFlush();
    glFinish();
    glPolygonOffset(0.0f,0.0f);
    glDisable(GL_CULL_FACE);
    m_point_buffer->unbindFromInput();
    glViewport(0,0, Engine::getWindowWidth(),Engine::getWindowHeight());
  }  
}
void Light::bindForRender(RenderShader * render_shader)
{
  if( isShadowEnabled() )
  {
    if( ( getType() == LIGHT_SPOT) || ( getType() == LIGHT_DIRECTIONAL) )
    {
      glm::mat4 biasMatrix(
      0.5, 0.0, 0.0, 0.0,
      0.0, 0.5, 0.0, 0.0,
      0.0, 0.0, 0.5, 0.0,
      0.5, 0.5, 0.5, 1.0
      );
      int num = render_shader->setShadowMap(biasMatrix * getProjection() * getView());
      m_shadow_index = num;
      m_directional_buffer->bindForOutput(num);
    }
    if( getType() == LIGHT_POINT)
    {
      int num = render_shader->setPointShadow();
      m_shadow_index = num;
      m_point_buffer->bindForOutput(num);    
    }
  }
}

glm::mat4 Light::getView()
{
  if(getType() != LIGHT_DIRECTIONAL)
   return glm::lookAt(m_position, m_position +  m_direction, glm::vec3(0,1,0));
  else
  {
    glm::vec3 cp = Engine::getScene()->getCamera()->getPosition();
    return glm::lookAt(cp,cp +  m_direction, glm::vec3(0,1,0));
  } 
}
glm::mat4 Light::getView(glm::vec3 dir, glm::vec3 up)
{
  return glm::lookAt(m_position, m_position +  dir, up);
}
glm::mat4 Light::getProjection()
{
  if( getType() == LIGHT_SPOT )
    return glm::perspective( m_cutoff_angle* 2.0f, 1.0f, 0.1f, 100.0f );
  if( getType() == LIGHT_POINT )
    return glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 100.0f );
  if( getType() == LIGHT_DIRECTIONAL )
  {  
    float bound = FAR_DIRECTIONAL_SHADOW_BOUND;
    return glm::ortho(-bound,bound,-bound,bound,-bound,FAR_CLIPPING_PLANE);
  }
}
bool Light::getShadowRefresh()
{
  if(m_refresh_shadow)
  {
    m_refresh_shadow = false;
    return true;
  }
  return false;
}

void Light::refresh()
{
  m_refresh_shadow = true;
}

void Light::setPosition(glm::vec3 p )
{
  m_position = p;
  m_refresh_shadow = true;
}

void Light::setShadowIndex( unsigned int idx)
{
  m_shadow_index = idx;
}
unsigned int Light::getShadowIndex()
{
  return m_shadow_index;
}
void Light::editRender(RenderShader * render_shader, int c)
{
  if( getType() == LIGHT_POINT )
  {
    render_shader->use();
    m_point_object->setPosition(m_position);
    m_point_object->setId(m_id);
    m_model->render(render_shader, false);
  }
  if( getType() == LIGHT_SPOT )
  {
    render_shader->use();
    m_spot_object->setPosition( m_position);
    //m_spot_object->setRotation( m_rotation);
    m_spot_object->setId(m_id);
    m_model->render(render_shader, false);
  }
  if( getType() == LIGHT_DIRECTIONAL)
  {
    render_shader->use();
    m_directional_object->setPosition( glm::vec3(0,10+c,0) );
    //m_spot_object->setRotation( m_rotation);
    m_directional_object->setId(m_id);
    m_model->render(render_shader, false);
  }
}

glm::vec2 Light::getRotation()
{
  return m_rotation;
}
void Light::rotate(glm::vec2 delta)
{
  m_rotation.x += delta.x;
  m_rotation.y -= delta.y;

  if(m_rotation.x >360.0f)
    m_rotation.x = 0.0f;
  if(m_rotation.y >360.0f)
    m_rotation.y = 0.0f;
  if(m_rotation.x <0.0f)
    m_rotation.x = 360.0f;
  if(m_rotation.y <0.0f)
    m_rotation.y = 360.0f;

  m_direction= Helper::anglesToDirection(m_rotation.x,m_rotation.y);
  m_refresh_shadow = true;
}
void Light::setRotation(glm::vec2 rot)
{
  m_rotation = rot;
  m_direction= Helper::anglesToDirection(m_rotation.x,m_rotation.y);
  m_refresh_shadow = true;
}


unsigned int Light::getNextId()
{
  int c = m_light_index_counter;
  m_light_index_counter++;
  return c;
}
unsigned int Light::getId()
{
  return m_id;
}
bool Light::isShadowEnabled()
{
  return m_shadow_enabled;
}

void Light::move(glm::vec3 delta)
{
  setPosition( getPosition()+ delta);
}
void Light::setColor(glm::vec3 c)
{
  m_color_ambient = m_color_specular = m_color_diffuse = c;
}  