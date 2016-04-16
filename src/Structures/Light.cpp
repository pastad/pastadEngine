#include "Light.h"
#include "Log.h"
#include "Engine.h"

Light::Light()
{
}

Light::~Light()
{
}

void Light::setDirectional(glm::vec3 direction, glm::vec3 col_am ,glm::vec3 col_dif, glm::vec3 col_spec,float intensity)
{
    m_type = LIGHT_DIRECTIONAL;
    m_direction = direction;
    m_color_ambient = col_am;
    m_color_diffuse =col_dif;
    m_color_specular = col_spec;
    m_intensity = intensity;
}

void Light::setPoint(glm::vec3 positon, glm::vec3 col_am ,glm::vec3 col_dif, glm::vec3 col_spec,float intensity,
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
}
void Light::setSpot(glm::vec3 position, glm::vec3 col_am ,glm::vec3 col_dif, glm::vec3 col_spec,float intensity,
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