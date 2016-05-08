#ifndef LIGHT_H
#define LIGHT_H

#include "gl_core_4_.h"
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

// class to represent a light (direcitonal, point or spot)

#define MAX_NUM_POINT_LIGHTS 10
#define MAX_NUM_SPOT_LIGHTS 10
#define MAX_NUM_DIRECTIONAL_LIGHTS 3

#define FAR_DIRECTIONAL_SHADOW_BOUND 50 // directional shadow size

enum
{
    LIGHT_DIRECTIONAL,
    LIGHT_POINT,
    LIGHT_SPOT,
    LIGHT_NONE
};

class DirectionalShadowBuffer;
class PointShadowBuffer;
class RenderBaseShader;
class RenderShader;

class Light
{
public:
  Light();
  ~Light();
  Light(const Light& other) = delete;
  Light& operator=(const Light& other) = delete;

  // light setters        
  bool setDirectional(glm::vec3 direction, glm::vec3 col_am ,glm::vec3 col_dif, glm::vec3 spec,float intensity);
  bool setPoint(glm::vec3 position, glm::vec3 col_am ,glm::vec3 col_dif, glm::vec3 spec,float intensity,
                float con ,float lin, float qua );
  bool setSpot(glm::vec3 position, glm::vec3 col_am ,glm::vec3 col_dif, glm::vec3 spec,float intensity,
                float con ,float lin, float qua, float cutoffAngle, glm::vec3 direction );


  //returns the type of the light
  unsigned int getType();
  
  // returns the position or direction of the light
  glm::vec3 getDirection();
  glm::vec3 getPosition();

  // sets the position of the light
  void setPosition(glm::vec3 p );

  // returns the individual color
  glm::vec3 getAmbientColor();
  glm::vec3 getDiffuseColor();
  glm::vec3 getSpecularColor();

  // returns attenuation specs
  float getIntensity();
  float getAttenuationConstant();
  float getAttenuationLinear();
  float getAttenuationQuadratic();

  // returns the cutoff angle in case of spot light 
  float getCutoffAngle();

  // binds the light for shadow rendering
  void bindForShadowRenderDirectional(RenderBaseShader * shadow_shader);
  void bindForShadowRenderPoint( RenderBaseShader * point_shadow_shader, int iteration);

  // unbinds the light from shadow rendering
  void unbindFromShadowRender();

  // binds the shadow buffer for render
  void bindForRender(RenderShader * render_shader);

  // returns the view and projection mat
  glm::mat4 getView();
  glm::mat4 getView(glm::vec3 dir, glm::vec3 up);
  glm::mat4 getProjection();

  //returns true if shadow needs to be recalculated
  bool getShadowRefresh();

  // called when shadow should be rerendered
  void refresh();

  // getter and setter for shadow index
  void setShadowIndex( unsigned int idx);
  unsigned int getShadowIndex();


protected:

private:

  // the ligth type
  unsigned int m_type;

  // position and direction
  glm::vec3 m_position;
  glm::vec3 m_direction;

  // light colors
  glm::vec3 m_color_ambient;
  glm::vec3 m_color_diffuse;
  glm::vec3 m_color_specular;

  // attenuation specs
  float m_att_const;
  float m_att_linear;
  float m_att_quadratic;
  float m_intensity;

  // cuttoff angle
  float m_cutoff_angle;

  // defines in which shadow map the shadow is stored
  unsigned int m_shadow_index;

  // light type counters
  static unsigned int m_num_point_lights;
  static unsigned int m_num_spot_lights;
  static unsigned int m_num_directional_lights;

  // the shadow buffer for directional lights
  DirectionalShadowBuffer * m_directional_buffer = nullptr;

  // the shadow buffer for point lights
  PointShadowBuffer * m_point_buffer = nullptr;

  // determines if light should be refreshed
  bool m_refresh_shadow;
};



#endif // LIGHT_H
