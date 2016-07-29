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
class Camera;
class BoundingBox;

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

  // returns the parent model
  Model * getModel();

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
  void applyPhysicsStatic();
  void dontApplyPhysicsStatic();
  bool isPhysicsStatic();

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

  // returns the minimal angle to the camera|light
  float getMinAngleToLight(Light * light);
  float getMinAngleToCamera(Camera * cam);

  // returns the corners of the bb adjusted to the object position
  std::vector<glm::vec3> getCorners();

  // returns the corners of the aabb 
  std::vector<glm::vec3> getAABBCorners();

  // returns the aab of the object
  BoundingBox * getAABB();

  // refreshes the axisaligned bounding box
  void refreshAABB();

  // getter setter for extraction flagg for scene tree extraction | against multi extraction
  void setExtractionFlag();
  void unsetExtractionFlag();
  bool isExtractionFlagSet();

  // getter setter for static flag
  void setStaticFlag();
  void unsetStaticFlag();
  bool isStaticFlagSet();


  //using  Transform::setPosition;  
  using  Transform::getPosition;
  //using  Transform::setRotation;
  using  Transform::getRotationDegrees;
  //using  Transform::setScale;
  using  Transform::getScale;

  // setters
  void setPosition(glm::vec3 p);
  void setRotation(glm::vec3 r);
  void setScale(glm::vec3 s);

  // apply the drop
  void applyDrop(glm::vec3 drop);

  // sets the drop vector 
  void setDropVector(glm::vec3 value);

  // returns the current fall vector
  glm::vec3 getFallVector();

  // true if fall should be checked;
  bool shouldFallBeChecked();
  
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

  // true if falling should be prevented
  bool m_physics_static = false;

  // the duration of the animation
  float m_animation_time = 0.0f;

  // the identificator of the object
  unsigned int m_id;

  // the flag for static or dynamic rndering
  bool m_static;

  // a axis alinged bb for visibility etc...
  BoundingBox * m_aabb = nullptr;

  // a flag for better extraction from the scene tree  | against multi extraction
  bool m_extraction_flag = false; 

  // the fall vector
  glm::vec3 m_fall_vector;

  // true if fall should be checked
  bool m_needs_fall_check = true;

  // returns the angle to ...
  float getAngleTo(glm::vec3 dir, glm::vec3 lp , glm::vec3 pos);

};

#endif