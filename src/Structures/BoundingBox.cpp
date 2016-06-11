#include "BoundingBox.h"

#include <iostream>

BoundingBox::BoundingBox(std::vector<BoundingBox*> sub_bbs)
{  
  float min_x = 0;
  float max_x = 0;
  float min_y = 0;
  float max_y = 0;
  float min_z = 0;
  float max_z = 0;
  bool f = true;


  for( std::vector<BoundingBox*>::iterator it = sub_bbs.begin(); it != sub_bbs.end();it++)
  {
    glm::vec3 p_1 = (*it)->getMinAnchor();
    glm::vec3 p_2 = (*it)->getMaxAnchor();
    if( f|| (min_x > p_1.x) ) 
      min_x = p_1.x;
    if( f|| (min_y > p_1.y) ) 
      min_y = p_1.y;
    if( f|| (min_z > p_1.z) ) 
      min_z = p_1.z;

    if( f || (max_x < p_1.x) ) 
      max_x = p_1.x;
    if( f || (max_y < p_1.y) ) 
      max_y = p_1.y;
    if( f|| (max_z < p_1.z) ) 
      max_z = p_1.z;


    if( (min_x > p_2.x) ) 
      min_x = p_2.x;
    if(  (min_y > p_2.y) ) 
      min_y = p_2.y;
    if(  (min_z > p_2.z) ) 
      min_z = p_2.z;

    if( (max_x < p_2.x) ) 
      max_x = p_2.x;
    if(  (max_y < p_2.y) ) 
      max_y = p_2.y;
    if((max_z < p_2.z) ) 
      max_z = p_2.z;
    f= false;
    //std::cout << "bbmin comb"<< p_1.x<<","<< p_1.y<<","<< p_1.z<<std::endl;
   // std::cout << "bbmax comb"<< p_2.x<<","<< p_2.y<<","<< p_2.z<<std::endl;
  }
  m_anchor_min = glm::vec3(min_x, min_y, min_z);
  m_anchor_max = glm::vec3(max_x, max_y, max_z);

 // std::cout << "bbmin comb"<< min_x<<","<< min_y<<","<< min_z<<std::endl;
  //std::cout << "bbmax comb"<< max_x<<","<< max_y<<","<< max_z<<std::endl;

}
BoundingBox::BoundingBox(float x_min,float x_max, float y_min, float y_max, float z_min, float z_max)
{
  m_anchor_min = glm::vec3(x_min,y_min,z_min);
  m_anchor_max = glm::vec3(x_max, y_max, z_max);
  if(x_min == x_max)
    m_anchor_max.x+=0.05f;
  if(y_min == y_max)
    m_anchor_max.y+=0.05f;
  if(z_min == z_max)
    m_anchor_max.z+=0.05f;

  //std::cout << "bbmin"<< x_min<<","<< y_min<<","<< z_min<<std::endl;
  //std::cout << "bbmax"<< x_max<<","<< y_max<<","<< z_max<<std::endl;
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

  
  /*std::cout << "p"<< pb_x.x<<","<< pb_x.y<<","<<pb_x.z<<std::endl;
  std::cout << "p"<< pb_y.x<<","<< pb_y.y<<","<<pb_y.z<<std::endl;
  std::cout << "p"<< pb_xy.x<<","<< pb_xy.y<<","<<pb_xy.z<<std::endl;
  std::cout << "pmin"<< m_anchor_min.x<<","<< m_anchor_min.y<<","<<m_anchor_min.z<<std::endl;

  std::cout << "p"<< pt_x.x<<","<< pt_x.y<<","<<pt_x.z<<std::endl;
  std::cout << "p"<< pt_y.x<<","<< pt_y.y<<","<<pt_y.z<<std::endl;
  std::cout << "p"<< pt_xy.x<<","<< pt_xy.y<<","<<pt_xy.z<<std::endl;
  std::cout << "pmax"<< m_anchor_max.x<<","<< m_anchor_max.y<<","<<m_anchor_max.z<<std::endl;*/

  return corners;
}
void BoundingBox::setBoundingPoints(float x_min,float x_max, float y_min, float y_max, float z_min, float z_max)
{
  m_anchor_min = glm::vec3(x_min,y_min,z_min);
  m_anchor_max = glm::vec3(x_max, y_max, z_max);
  //std::cout << "bbmin"<< x_min<<","<< y_min<<","<< z_min<<std::endl;
  //std::cout << "bbmax"<< x_max<<","<< y_max<<","<< z_max<<std::endl;
}

bool BoundingBox::contains(BoundingBox * bb)
{
  glm::vec3 anchor_min_2 = bb->getMinAnchor();
  glm::vec3 anchor_max_2 = bb->getMaxAnchor();

  return (anchor_min_2.x >= m_anchor_min.x && anchor_max_2.x <= m_anchor_max.x) &&
          (anchor_min_2.y >= m_anchor_min.y && anchor_max_2.y <= m_anchor_max.y) &&
          (anchor_min_2.z >= m_anchor_min.z && anchor_max_2.z <= m_anchor_max.z);
}

bool BoundingBox::intersectAABBs(BoundingBox * bb)
{
  glm::vec3 anchor_min_2 = bb->getMinAnchor();
  glm::vec3 anchor_max_2 = bb->getMaxAnchor();
 // std::cout << "amin"<< anchor_min_2.x<<","<< anchor_min_2.y<<","<< anchor_min_2.z<<std::endl;
 // std::cout << "amax"<< anchor_max_2.x<<","<< anchor_max_2.y<<","<< anchor_max_2.z<<std::endl;
 // std::cout << "amin h"<< m_anchor_min.x<<","<< m_anchor_min.y<<","<< m_anchor_min.z<<std::endl;
 // std::cout << "amin h"<< m_anchor_max.x<<","<< m_anchor_max.y<<","<< m_anchor_max.z<<std::endl;


  return (m_anchor_min.x <= anchor_max_2.x && m_anchor_max.x >= anchor_min_2.x) &&
         (m_anchor_min.y <= anchor_max_2.y && m_anchor_max.y >= anchor_min_2.y) &&
         (m_anchor_min.z <= anchor_max_2.z && m_anchor_max.z >= anchor_min_2.z);
}