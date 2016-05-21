#ifndef OBJECT
#define OBJECT_H

#include "Transform.h"

#include <string>
#include <vector>

#include <assimp/scene.h>

// the representation of an Model to the user

// the animation was inspired by the ogldev tutorial | the extent is not know due to adaption from previous project

class Model;

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

  // id getter setter
  void setId(int id);
  int getId();

  // directly sets the model matrix
  void setModel(glm::mat4 model);

  // returns the identifier of the belonging model
  std::string getIdentifier();

  using  Transform::setPosition;
  using  Transform::setRotation;
  using  Transform::setScale;
  using  Transform::getPosition;

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

};

#endif