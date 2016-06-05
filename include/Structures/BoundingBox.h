#ifndef BOUNDING_BOX
#define BOUNDING_BOX_H


#include <string>
#include <vector>

#include <glm/glm.hpp>

// the bounding box for visibility and simple intersection  tests

class BoundingBox
{
public:
  BoundingBox(std::vector<BoundingBox*> sub_bbs);
  BoundingBox(float x_min,float x_max, float y_min, float y_max, float z_min, float z_max);
  ~BoundingBox(); 
  BoundingBox(const BoundingBox& other) = delete;
  BoundingBox& operator=(const BoundingBox& other) = delete;

  // getters for min and max anchor
  glm::vec3 getMinAnchor();
  glm::vec3 getMaxAnchor();

  // getter for corner points of bb
  std::vector<glm::vec3> getCorners();

private:

  // holds the anchor where x,y,z is at min
  glm::vec3 m_anchor_min;

  // holds the anchor where x,y,z is at max
  glm::vec3 m_anchor_max;



};

#endif // BOUNDING_BOX