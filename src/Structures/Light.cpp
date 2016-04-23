#include "Light.h"
#include "Log.h"
#include "Engine.h"

#include "DirectionalShadowBuffer.h"
#include "ShadowShader.h"
#include "RenderShader.h"

Light::Light()
{
}

Light::~Light()
{
  if(m_directional_buffer != nullptr)
    delete m_directional_buffer;
}

bool Light::setDirectional(glm::vec3 direction, glm::vec3 col_am ,glm::vec3 col_dif, glm::vec3 col_spec,float intensity)
{
    m_type = LIGHT_DIRECTIONAL;
    m_direction = direction;
    m_color_ambient = col_am;
    m_color_diffuse =col_dif;
    m_color_specular = col_spec;
    m_intensity = intensity;

    return true;
}

bool Light::setPoint(glm::vec3 positon, glm::vec3 col_am ,glm::vec3 col_dif, glm::vec3 col_spec,float intensity,
                      float con ,float lin, float qua )
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

    return true;
}
bool Light::setSpot(glm::vec3 position, glm::vec3 col_am ,glm::vec3 col_dif, glm::vec3 col_spec,float intensity,
                      float con ,float lin, float qua, float cutoffAngle, glm::vec3 direction )
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
    if( !m_directional_buffer->initialize(1240, 720))
      return false;
    return true;
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

void Light::bindForShadowRender(ShadowShader * shadow_shader)
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
    int num = render_shader->setDirectionalShadowMap(biasMatrix * getProjection() * getView());
    m_directional_buffer->bindForOutput(num);
  }
}

glm::mat4 Light::getView()
{
  return glm::lookAt(m_position, m_position +  m_direction, glm::vec3(0,1,0));
}
glm::mat4 Light::getProjection()
{
  if( getType() == LIGHT_SPOT )
    return glm::perspective( m_cutoff_angle* 2.0f, 1.0f, 0.1f, 100.0f );
}