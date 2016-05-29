#ifndef LIGHT_H
#define LIGHT_H

#include "gl_core_4_.h"
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

// class to represent a light (direcitonal, point or spot)

#define MAX_NUM_POINT_LIGHTS 10
#define MAX_NUM_SPOT_LIGHTS 10
#define MAX_NUM_DIRECTIONAL_LIGHTS 3


#define MAX_NUM_POINT_SHADOWS 10
#define MAX_NUM_DIRECTIONAL_SHADOWS 10

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
class Model;
class Object;

class Light
{
public:
  Light();
  ~Light();
  Light(const Light& other) = delete;
  Light& operator=(const Light& other) = delete;

  // light setters        
  bool setDirectional(glm::vec3 direction, glm::vec3 col_am ,glm::vec3 col_dif, glm::vec3 spec,float intensity, bool enable_shadow);
  bool setPoint(glm::vec3 position, glm::vec3 col_am ,glm::vec3 col_dif, glm::vec3 spec,float intensity,
                float con ,float lin, float qua , bool enable_shadow);
  bool setSpot(glm::vec3 position, glm::vec3 col_am ,glm::vec3 col_dif, glm::vec3 spec,float intensity,
                float con ,float lin, float qua, float cutoffAngle, glm::vec2 rotation , bool enable_shadow);


  //returns the type of the light
  unsigned int getType();
  
  // returns the position or direction of the light
  glm::vec3 getDirection();
  glm::vec3 getPosition();

  // sets the position of the light
  void setPosition(glm::vec3 p );

  // moves the light
  void move(glm::vec3 delta);

  // returns the individual color
  glm::vec3 getAmbientColor();
  glm::vec3 getDiffuseColor();
  glm::vec3 getSpecularColor();

  // sets all light colors 
  void setColor(glm::vec3 c);

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

  // renders the light as a sphere 
  void editRender(RenderShader * render_shader, int c);

  // returns the id of the light
  unsigned int getId();

  // returns true if shadow casting is enabled
  bool isShadowEnabled();

  // rotation getter setter
  glm::vec2 getRotation();
  void rotate(glm::vec2 delta);
  void setRotation(glm::vec2 delta);


protected:

private:

  // the ligth type
  unsigned int m_type;

  // position and direction and the rotation 
  glm::vec3 m_position;
  glm::vec3 m_direction;
  glm::vec2 m_rotation;

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
  static unsigned int m_light_index_counter;
  static unsigned int m_num_directional_shadows;
  static unsigned int m_num_point_shadows;


  // the shadow buffer for directional lights
  DirectionalShadowBuffer * m_directional_buffer = nullptr;

  // the shadow buffer for point lights
  PointShadowBuffer * m_point_buffer = nullptr;

  // true if shadow should be cast
  bool m_shadow_enabled;

  // determines if light should be refreshed
  bool m_refresh_shadow;

  // the id of the light
  unsigned int m_id;

  // the sphere model and its object
  Model * m_model;
  static Object * m_directional_object;
  static Object * m_spot_object;
  static Object * m_point_object;

  // returns the next id of the light numbering
  unsigned int getNextId();

};



#endif // LIGHT_H
