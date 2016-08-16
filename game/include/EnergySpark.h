#ifndef ENERGYSPARK_H
#define ENERGYSPARK_H


#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

#include <vector>
#include <string>

class Scene;
class Object;

// represents a energy sparking a plant

class EnergySpark
{
public:
  EnergySpark( );
  ~EnergySpark();

  EnergySpark(const EnergySpark& other) = delete;
  EnergySpark& operator=(const EnergySpark& other) = delete;

  // inits the spark
  bool initialize(Scene * scene, glm::vec3 start_position, glm::vec3 target_position, unsigned int plant_type);
  bool initialize(Object * object,  glm::vec3 target_position, unsigned int plant_type);

  // update function 
  bool update(float delta);

  // returns the object
  Object * getObject();

  // returns the plant type
  unsigned int getPlantType();

  // returns the target pos
  glm::vec3 getTarget();

private:

  // the object 
  Object * m_object;

  // the target of the spark
  glm::vec3 m_target_position;

  // the plant the spark is going to spawn
  unsigned int m_plant_type;


};

#endif // ENERGYSPARK_H