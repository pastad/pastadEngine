#include "Light.h"
#include "Log.h"
#include "Engine.h"
#include "Scene.h"
#include "Camera.h"

#include "DirectionalShadowBuffer.h"
#include "PointShadowBuffer.h"
#include "ShadowShader.h"
#include "PointShadowShader.h"
#include "RenderShader.h"

#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/transform.hpp>


unsigned int Light::m_num_point_lights = 0;
unsigned int Light::m_num_spot_lights = 0;
unsigned int Light::m_num_directional_lights = 0;

Light::Light():m_type(LIGHT_NONE)
{
}

Light::~Light()
{
  if(m_directional_buffer != nullptr)
    delete m_directional_buffer;
  if(m_point_buffer != nullptr)
    delete m_point_buffer;
}

bool Light::setDirectional(glm::vec3 direction, glm::vec3 col_am ,glm::vec3 col_dif, glm::vec3 col_spec,float intensity)
{
  if( m_num_directional_lights < MAX_NUM_DIRECTIONAL_LIGHTS )
  {
    m_type = LIGHT_DIRECTIONAL;
    m_direction = direction;
    m_color_ambient = col_am;
    m_color_diffuse =col_dif;
    m_color_specular = col_spec;
    m_intensity = intensity;
    m_num_directional_lights++;
    m_refresh_shadow =true;
    m_position = glm::vec3(0,0,0);

    m_directional_buffer = new DirectionalShadowBuffer();
    if( !m_directional_buffer->initialize(Engine::getWindowWidth() *4.0f,
     Engine::getWindowHeight() *4.0f))
      return false;


    return true;
  }

  return false;
}

bool Light::setPoint(glm::vec3 positon, glm::vec3 col_am ,glm::vec3 col_dif, glm::vec3 col_spec,float intensity,
                      float con ,float lin, float qua )
{
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

    m_point_buffer = new PointShadowBuffer();
    if( !m_point_buffer->initialize(1000, 1000))
      return false;
    m_refresh_shadow =true;

    m_num_point_lights++;

    return true;
  }
  return false;
}
bool Light::setSpot(glm::vec3 position, glm::vec3 col_am ,glm::vec3 col_dif, glm::vec3 col_spec,float intensity,
                      float con ,float lin, float qua, float cutoffAngle, glm::vec3 direction )
{
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
    m_direction = direction;
    m_cutoff_angle = cutoffAngle;
    m_directional_buffer = new DirectionalShadowBuffer();
    if( !m_directional_buffer->initialize(Engine::getWindowWidth(),
        Engine::getWindowHeight()  ) ) 
      return false;
    m_num_spot_lights++;
    m_refresh_shadow =true;
    return true;
  }
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

void Light::bindForShadowRenderSpot(ShadowShader * shadow_shader)
{
  if( m_type == LIGHT_SPOT)
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

void Light::bindForShadowRenderDirectional(ShadowShader * shadow_shader)
{
  if( m_type == LIGHT_DIRECTIONAL)
  {  
    
    shadow_shader->use();
    m_directional_buffer->bindForInput();
   
    glEnable(GL_DEPTH_TEST);

    glPolygonOffset(0.01f,0.01f);

    shadow_shader->setProjectionMatrix(getProjection());    
    shadow_shader->setViewMatrix(getView() ); 

    glViewport(0,0, m_directional_buffer->getWidth() ,m_directional_buffer->getHeight());
    glClearColor(1000,0,0,0);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    

  }
}
  
void Light::bindForShadowRenderPoint( PointShadowShader * point_shadow_shader, int iteration)
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
    point_shadow_shader->setLightPosition(getPosition());  
    glViewport(0,0, 1000 ,1000);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);
    
  }
  
}

void Light::unbindFromShadowRender()
{
  if( m_type == LIGHT_SPOT)
  {
    glFlush();
    glFinish();
    glPolygonOffset(0.0f,0.0f);
    glDisable(GL_CULL_FACE);
    m_directional_buffer->unbindFromInput();
    glViewport(0,0, Engine::getWindowWidth(),Engine::getWindowHeight());
  }
  if( m_type == LIGHT_DIRECTIONAL)
  {
    glFlush();
    glFinish();
    glPolygonOffset(0.0f,0.0f);
    glDisable(GL_CULL_FACE);
    m_directional_buffer->unbindFromInput();
    glViewport(0,0, Engine::getWindowWidth(),Engine::getWindowHeight());

  }
  if( m_type == LIGHT_POINT)
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
  if( getType() == LIGHT_SPOT)
  {
    glm::mat4 biasMatrix(
    0.5, 0.0, 0.0, 0.0,
    0.0, 0.5, 0.0, 0.0,
    0.0, 0.0, 0.5, 0.0,
    0.5, 0.5, 0.5, 1.0
    );
    int num = render_shader->setShadowMap(biasMatrix * getProjection() * getView());
    m_directional_buffer->bindForOutput(num);
  }
  if( getType() == LIGHT_DIRECTIONAL)
  {
    glm::mat4 biasMatrix(
    0.5, 0.0, 0.0, 0.0,
    0.0, 0.5, 0.0, 0.0,
    0.0, 0.0, 0.5, 0.0,
    0.5, 0.5, 0.5, 1.0
    );
    int num = render_shader->setShadowMap(biasMatrix * getProjection() * getView());
    m_directional_buffer->bindForOutput(num);
  }
  if( getType() == LIGHT_POINT)
  {
    int num = render_shader->setPointShadow();
    m_point_buffer->bindForOutput(num);    
  }
}

glm::mat4 Light::getView()
{
  if(getType() != LIGHT_DIRECTIONAL)
   return glm::lookAt(m_position, m_position +  m_direction, glm::vec3(0,1,0));
  else
  {
   // glm::quat rot(m_direction);
   // glm::mat4 m = glm::mat4_cast(rot);
   // return m;
    glm::vec3 cp = Engine::getScene()->getCamera()->getPosition();
   // cp += glm::vec3(0,50,0);
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