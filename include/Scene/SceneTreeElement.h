#ifndef SCENETREEELEMENT
#define SCENETREEELEMENT_H

// class represents the scene tree elements

#include <vector>
#include <string>
#include <map>

#define GLM_FORCE_RADIANS

#include <glm/glm.hpp>


#define STOP_SIZE 20

class Model;
class Object;
class BoundingBox;
class Camera;

class SceneTreeElement
{
public:


  SceneTreeElement(unsigned int size, glm::vec3 center);
  ~SceneTreeElement();

  SceneTreeElement(const SceneTreeElement& other) = delete;
  SceneTreeElement& operator=(const SceneTreeElement& other) = delete;

  // returns true if pos is inside cube
  bool inside(glm::vec3 pos);

  // inserts object into scene tree element
  void insert(Object * obj);

  // removes an object from the scene tree element
  void remove(Object * obj);

  // retrieves object in a certain angle
  void getObjects(glm::vec3 spos, float angle_s, glm::vec3 dir, std::map< std::string , std::vector<Object *> > * object_list);

  //  returns the children of the scene tree
  std::vector<SceneTreeElement *> getChildren(); 

  // returns all objects
  std::vector<Object *> getObjects();

  // returns the bb of the scene tree element
  BoundingBox * getBoundingBox();

  // retrieves the objects in the frustrum
  void getObjectsInFrustrum(Camera* cam, std::map<std::string, std::vector<Object *>> * objects, bool fully_inside);

private:

  // the size of the cube
  unsigned int m_size;

  // the center of the cube
  glm::vec3 m_center;

  // the subelements of the scene
  SceneTreeElement * m_top_front_left = nullptr;
  SceneTreeElement * m_top_front_right = nullptr;
  SceneTreeElement * m_top_back_left = nullptr;
  SceneTreeElement * m_top_back_right = nullptr;
  SceneTreeElement * m_bottom_front_left = nullptr;
  SceneTreeElement * m_bottom_front_right = nullptr;
  SceneTreeElement * m_bottom_back_left = nullptr;
  SceneTreeElement * m_bottom_back_right = nullptr;

  // contains the aabb for the containment
  BoundingBox * m_aabb = nullptr;

  // true if it is the last leaf
  bool m_last_element = false;


  // the objects of the scene tree element
  std::vector<Object *> m_objects;

  // internal insert 
  void insert(SceneTreeElement ** se , Object * obj, glm::vec3 center);


};

#endif // SCENETREEELEMENT_H