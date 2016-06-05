#ifndef OBJECT
#define OBJECT_H

#include "Transform.h"

#include <string>
#include <vector>

#include <assimp/scene.h>

#include "tinyxml2.h"

// the representation of an Model to the user

// the animation was inspired by the ogldev tutorial | the extent is not know due to adaption from previous project

class Model;
class Light;

class Object : public Transform
{
public:
	Object(std::string identifier, Model * parent);
	~Object();

  // updates the bones 
  void boneUpdate(float time_in_seconds);

  // returns the bone transforms for rendering
  std::vector<glm::mat4> * getBoneTransforms(unsigned int mesh_id);

	// returns the model matrix
	glm::mat4 getModelMatrix();

  // moves the object
  void move(glm::vec3 direction);

  // rotates the object
  void rotate(glm::vec3 delta);

  // priority render getter setters
  void setPriorityRender();
  void unsetPriorityRender();
  bool getPriorityRender();

  // visble getter setters
  void setVisible();
  void setInvisible();
  bool isVisible();

  // physics getter setters
  void dontApplyPhysics();
  void applyPhysics();
  bool isPhysicsApplied();

  // animation time setter geter
  float getAnimationTime();
  void advanceAnimation(float delta);

  // returns true if animated
  bool isAnimated();

  // id getter setter
  void setId(int id);
  int getId();

  // directly sets the model matrix
  void setModel(glm::mat4 model);

  // returns the identifier of the belonging model
  std::string getIdentifier();

  // saves the object
  void save(tinyxml2::XMLNode * parent, tinyxml2::XMLDocument * doc);

  // loads the object
  bool load(tinyxml2::XMLElement *  element);

  // returns the min bb distant corner point in relation to ref
  glm::vec3 getMinBBDistantPoint(glm::vec3 ref);

  // returns the minimal angle to the camera
  float getMinAngleToLight(Light * light);

  using  Transform::setPosition;  
  using  Transform::getPosition;
  using  Transform::setRotation;
  using  Transform::getRotationDegrees;
  using  Transform::setScale;
  using  Transform::getScale;
  
private:

  // pointer to the "parent" model
  Model * m_model;

  // determines if object should always be rendered
  bool m_priority_render = false;

  // the identifier of the belonging material
  std::string m_identifier;

  // determines if object should be rendered when in the visible range
  bool m_visible = true;

  // determines is physics should be applied
  bool m_physics_enabled = false;

  // the duration of the animation
  float m_animation_time = 0.0f;

  // the identificator of the object
  unsigned int m_id;

  // returns the angle to the camer 
  float getAngleToLight(Light * light, glm::vec3 pos);


};

#endif