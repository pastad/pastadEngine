#include "BoundingBox.h"

BoundingBox::BoundingBox(std::vector<BoundingBox*> sub_bbs)
{  
  float min_x = -1;
  float max_x = -1;
  float min_y = -1;
  float max_y = -1;
  float min_z = -1;
  float max_z = -1;


  for( std::vector<BoundingBox*>::iterator it = sub_bbs.begin(); it != sub_bbs.end();it++)
  {
    glm::vec3 p_1 = (*it)->getMinAnchor();
    glm::vec3 p_2 = (*it)->getMaxAnchor();
    if( (min_x == -1) || (min_x > p_1.x) ) 
      min_x = p_1.x;
    if( (min_y == -1) || (min_y > p_1.y) ) 
      min_y = p_1.y;
    if( (min_z == -1) || (min_z > p_1.z) ) 
      min_z = p_1.z;

    if( (max_x == -1) || (max_x < p_1.x) ) 
      max_x = p_1.x;
    if( (max_y == -1) || (max_y < p_1.y) ) 
      max_y = p_1.y;
    if( (max_z == -1) || (max_z < p_1.z) ) 
      max_z = p_1.z;


    if( (min_x == -1) || (min_x > p_2.x) ) 
      min_x = p_2.x;
    if( (min_y == -1) || (min_y > p_2.y) ) 
      min_y = p_2.y;
    if( (min_z == -1) || (min_z > p_2.z) ) 
      min_z = p_2.z;

    if( (max_x == -1) || (max_x < p_1.x) ) 
      max_x = p_2.x;
    if( (max_y == -1) || (max_y < p_2.y) ) 
      max_y = p_2.y;
    if( (max_z == -1) || (max_z < p_2.z) ) 
      max_z = p_2.z;
  }
  m_anchor_min = glm::vec3(min_x, min_y, min_z);
  m_anchor_max = glm::vec3(max_x, max_y, max_z);

}
BoundingBox::BoundingBox(float x_min,float x_max, float y_min, float y_max, float z_min, float z_max)
{
  m_anchor_min = glm::vec3(x_min,y_min,z_min);
  m_anchor_max = glm::vec3(x_max, y_max, z_max);
}
BoundingBox::~BoundingBox()
{  
}

glm::vec3 BoundingBox::getMinAnchor()
{
  return m_anchor_min;
}
glm::vec3 BoundingBox::getMaxAnchor()
{
  return m_anchor_max;
}
std::vector<glm::vec3> BoundingBox::getCorners()
{
  std::vector<glm::vec3> corners;
  corners.push_back(m_anchor_min);
  corners.push_back(m_anchor_max);

  glm::vec3 pb_x = m_anchor_min;
  pb_x.x =m_anchor_max.x;
  glm::vec3 pb_y = m_anchor_min;
  pb_y.y =m_anchor_max.y;
  glm::vec3 pb_xy = m_anchor_min;
  pb_xy.x =m_anchor_max.x;
  pb_xy.y =m_anchor_max.y;

  glm::vec3 pt_x = m_anchor_max;
  pt_x.x =m_anchor_min.x;
  glm::vec3 pt_y = m_anchor_max;
  pt_y.y =m_anchor_min.y;
  glm::vec3 pt_xy = m_anchor_max;
  pt_xy.y =m_anchor_min.y;
  pt_xy.x =m_anchor_min.x;

  corners.push_back(pt_x);
  corners.push_back(pt_y);
  corners.push_back(pt_xy);
  corners.push_back(pb_xy);
  corners.push_back(pb_x);
  corners.push_back(pb_y);
  return corners;
}