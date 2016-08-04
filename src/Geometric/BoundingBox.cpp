#include "BoundingBox.h"

#include "Ray.h"

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
  }
  m_anchor_min = glm::vec3(min_x, min_y, min_z);
  m_anchor_max = glm::vec3(max_x, max_y, max_z);
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
}

BoundingBox::~BoundingBox()
{  
}


// getter/setters -------------------------------------------------

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

void BoundingBox::setBoundingPoints(float x_min,float x_max, float y_min, float y_max, float z_min, float z_max)
{
  m_anchor_min = glm::vec3(x_min,y_min,z_min);
  m_anchor_max = glm::vec3(x_max, y_max, z_max); 
}


// queries -------------------------------------------------

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

  return (m_anchor_min.x <= anchor_max_2.x && m_anchor_max.x >= anchor_min_2.x) &&
         (m_anchor_min.y <= anchor_max_2.y && m_anchor_max.y >= anchor_min_2.y) &&
         (m_anchor_min.z <= anchor_max_2.z && m_anchor_max.z >= anchor_min_2.z);
}

// inspired by : hhttp://www.cs.utah.edu/~awilliam/box/box.pdf (Smits method)
bool BoundingBox::intersectRay(Ray * ray)
{ 
  glm::vec3 min_anchor = getMinAnchor();
  glm::vec3 max_anchor = getMaxAnchor();


  glm::vec3 direction =  ray->getDirection();
  glm::vec3 origin = ray->getOrigin();

  float x_min =  (min_anchor.x - origin.x) / direction.x; 
  float x_max =  (max_anchor.x - origin.x) / direction.x; 
  if( direction.x < 0.0f)
  {
    float t= x_min;
    x_min = x_max;
    x_max = t;
  }

  float y_min =  (min_anchor.y - origin.y) / direction.y; 
  float y_max =  (max_anchor.y - origin.y) / direction.y; 
  if( direction.y < 0.0f )
  {
    float t= y_min;
    y_min = y_max;
    y_max = t;
  }

  if (   ( x_min > y_max ) || ( y_min > x_max )   )     
    return false; 

  if( y_min > x_min ) 
    x_min = y_min; 

  if( y_max < x_max ) 
    x_max = y_max; 

  float z_min = (min_anchor.z - origin.z)/ direction.z; 
  float z_max = (max_anchor.z - origin.z)/ direction.z; 

  if( direction.z < 0.0f )
  {
    float t= z_min;
    z_min = z_max;
    z_max = t;
  }

  if (  ( x_min > z_max ) || ( z_min > x_max )  ) 
    return false; 

  float b1 = ray->getLowerBound();
  float b2 = ray->getUpperBound();

  if(b1 ==-1.0f &&  b2 ==-1.0f )
    return true;
  
  // range check if needed
    if (z_min > x_min)
  x_min = z_min;
  if (z_max < x_max)
  x_max = z_max;

 return ( (x_min < b2) && (x_max > b1) );

  return true; 
}
