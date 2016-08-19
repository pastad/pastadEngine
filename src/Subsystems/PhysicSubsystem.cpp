#include "PhysicSubsystem.h"

#include "Engine.h"
#include "Log.h"
#include "Scene.h"
#include "Camera.h"

#include "SceneTreeElement.h"
#include "Object.h"

#include "Model.h"
#include "Mesh.h"
#include "Ray.h"
#include "BoundingBox.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <limits>

#define EPSILON 0.000001

PhysicSubsystem::PhysicSubsystem():Subsystem("PhysicSubsystem")
{
}

PhysicSubsystem::~PhysicSubsystem()
{
}

// update methods ------------------------------------------

void PhysicSubsystem::updateScene(Scene * scene, float delta)
{
  SceneTreeElement * root = scene->getSceneRoot();
  traverseScene(root,scene, delta);
  Camera * cam = scene->getCamera();  
  if(cam != nullptr)
  {
    if(cam->isPhysicsApplied())
    {      
      glm::vec3 drop = glm::vec3(0,-98.1,0)* (delta/1000.0f);

      std::vector<Object *> objs = scene->getPhysicsStaticObjects();

      float distance = std::numeric_limits<float>::max();
      glm::vec3 p = cam->getPosition();//+ (cam->getFallVector()+drop );
      Ray ra(p,glm::vec3(0,-1.0f,0));
      for(int k =0; k < objs.size();k++)
      {
        Object * o  = objs[k];
        float dist;
        bool res = collisionRayObject( o, &ra , &dist);
        if( res )
        {
          //std::cout<< dist <<std::endl;
          if(dist < distance)
            distance =dist;
        }
      }
      if(  ( distance - cam->getBottomOffset() -0.1f ) > 0 )
      {
        cam->applyDrop(drop);  
      }
      else
        cam->setFallVector(glm::vec3(0,0,0));
    }
  }
  
  //scene->cameraMoved();
}

void PhysicSubsystem::traverseScene( SceneTreeElement * element , Scene * scene, float delta)
{
  std::vector< SceneTreeElement *> children = element->getChildren();
  std::vector< Object *> objects = element->getObjects();
  for( int x=0;  x< objects.size(); x++)
  {

    Object * obj = objects.at(x);
    //if(obj->isPhysicsApplied() &&  !obj->shouldFallBeChecked() && ! obj->isPhysicsStatic()) 
     // std::cout << obj->getIdentifier()<<std::endl;

    if( obj->shouldFallBeChecked() && obj->isPhysicsApplied())
    {

      if( ! obj->isPhysicsStatic() )
      {
        glm::vec3 drop = glm::vec3(0,-9.81,0)* (delta/1000.0f);
        
        glm::vec3 p = obj->getPosition();//+ (cam->getFallVector()+drop );

        Ray ra(p,glm::vec3(0.01f,-1.0f,0.01f));
        float distance;


        if(collisionRayScene(scene,&ra, &distance))
        {
         
          if(  ( distance - obj->getFallVector().y-drop.y -0.1f  ) > 0 )
          {
            //cam->applyDrop(drop);  
            obj->applyDrop(drop  );    
          }
          else
            obj->setDropVector(glm::vec3(0,0,0));
        }         
      }
    }
  }
  for( int x=0;  x< children.size(); x++)
  {
    traverseScene(children.at(x), scene, delta );
  }
}


// collision mehtods ------------------------------------------

bool PhysicSubsystem::collisionRayScene(Scene * scene, Ray * ray, float * distance)
{
  std::vector<Object *> objs = scene->getPhysicsStaticObjects();
  bool intersection = false;
  float min_dist = std::numeric_limits<float>::max();
  for(int x =0; x< objs.size();x++)
  {
    Object * o = objs[x];
    float d;
    if(collisionRayObject((o), ray, &d ) )
    {
      intersection = true;
      if(d < min_dist)
        min_dist =d;
    }
  }

  *distance = min_dist;
  return intersection;  
}


bool PhysicSubsystem::collisionRayObject(Object * obj, Ray * ray, float * distance)
{
  std::vector<Mesh*> meshes = obj->getModel()->getMeshes();
  bool found = false;
  float dista = -1.0f;

  bool res = obj->getAABB()->intersectRay(ray); 

  if( res)
  {    
   // std::cout <<obj->getIdentifier()<< " bb hit" <<std::endl;
    for( int x=0; x < meshes.size(); x++)
    {
      float dist = 0.0f;   
      if(collisionRayMesh(meshes[x],obj->getModelMatrix(), ray,&dist) )
      {
        if( (dista == -1.0f) || (dist < dista) )
          dista = dist;
        found = true;
      }
    } 
  }
  else
    return false;
  if(found)
    *distance = dista;
  return found;
}

bool PhysicSubsystem::collisionRayMesh( Mesh * m , glm::mat4 transform, Ray * ray ,float * distance)
{
  IndexedRepresentation * ir = m->getIndexedRepresentation();

  float max_dist =  std::numeric_limits<float>::max();
  bool intersection = false;
 // std::cout << ir->m_indices.size() /3.0f <<std::endl;
  for( int i =0; i < ir->m_indices.size(); i+=3)
  {

    glm::vec3 p1 =  glm::vec3( transform * glm::vec4(ir->m_positions[ ir->m_indices[i]],1.0f)) ;
    glm::vec3 p2 =  glm::vec3( transform * glm::vec4(ir->m_positions[ ir->m_indices[i+1]],1.0f))  ;
    glm::vec3 p3 =  glm::vec3( transform * glm::vec4(ir->m_positions[ ir->m_indices[i+2] ],1.0f))  ;
    float d;

    if(collisionRayTriangle(p1,p2,p3,ray, &d) )
    {
      if(max_dist > d)
        max_dist =d;
      intersection = true;     
    }
    
  }

  if(intersection)  
    *distance = max_dist;    
  

  return intersection;
}

// Möller–Trumbore algortihm
bool  PhysicSubsystem::collisionRayTriangle( glm::vec3 p1, glm::vec3 p2 , glm::vec3 p3, Ray * ray, float * distance )
{
  glm::vec3 e1;
  glm::vec3 e2;

  glm::vec3 p;
  glm::vec3 q;
  glm::vec3 t;
  float det, inv_det,u,v;
  float t_;

  e1 = p2-p1;
  e2 = p3-p1;

  p = glm::cross(ray->getDirection(),e2);
  det = glm::dot(e1,p);

  if(det > -EPSILON && det < EPSILON) 
    return false;

  inv_det = 1.0f / det;

  t = ray->getOrigin() - p1;

  u =glm::dot(t, p) * inv_det;
 
  if(u < 0.0f || u > 1.0f) 
    return false;

  q = glm::cross(t,e1);

  v = glm::dot(ray->getDirection(), q) * inv_det;
 
  if(v < 0.0f || u + v  > 1.0f)
   return false;

  t_ = glm::dot(e2, q) * inv_det;

  if(t_ > EPSILON)
  { 
    *distance = t_;
    return true;
  }


  return false;
}


Object *  PhysicSubsystem::collisionObjectObjects(std::vector<Object*> objs, Object * object)
{
  for(int x=0; x< objs.size();x++)
  {
    if(collisionObjectObject(object, objs[x]))
      return objs[x];
  }

  return nullptr;
}

bool PhysicSubsystem::collisionObjectObject(Object * object1, Object * object2)
{
  BoundingBox * bb1 = object1->getAABB();
  BoundingBox * bb2 = object2->getAABB();

  return (bb1->intersectAABBs(bb2));
    
}


bool PhysicSubsystem::collisionMovement(Scene * scene, glm::vec3 pos, glm::vec3 dir , float step, float surrounding_offset, float bottom_offset, glm::vec3 * new_pos)
{  

  float bottom_off_halfed = 0.01f;
  if( bottom_offset > 0.0f)
    bottom_off_halfed = bottom_offset /2.0f;

  glm::vec3 npos =  pos + dir * step;
  if( (scene != nullptr) )
  {
    Ray r(pos-glm::vec3(0,bottom_offset-0.1f,0), dir);
    float distance;
    if( ! collisionRayScene(scene, &r, &distance))
    {        
      // if no collision update  
      *new_pos = npos;      
      //std::cout << "no col"<< dir.x<<" , "<< dir.y<<" , "<< dir.z<<" , " <<std::endl;
      return true;
    }
    else
    {
      // .. also update if there is enough space in front
      if(distance -step > 0.0f)
      {
        *new_pos = npos;
        return true;
      }
      else
      {
        Ray r2(pos-glm::vec3(0,bottom_off_halfed,0), dir, 0.0f, surrounding_offset);
        float distance2 =0.0f;
        bool res = collisionRayScene(scene, &r2, &distance2);

        // otherwise check if we can make a step forward an upwards
        if( !res || (distance2 > surrounding_offset) )
        {       
          Ray r3(pos-glm::vec3(0,bottom_off_halfed,0)+dir*surrounding_offset ,glm::vec3(0,-1,0));
          float distance3;

          // how far we need to step upward ?? -> distance3
          if(  collisionRayScene(scene, &r3, &distance3))
          {
            if((bottom_off_halfed- distance3) < 0.2f && ((bottom_off_halfed- distance3) >0.0f))
            {
              *new_pos = pos +  glm::vec3(0,  (bottom_off_halfed- distance3),0);
              return true;
            }
            //else
           //   std::cout <<distance3<<std::endl;
          }
        }
        return false;     
      }

      std::cout << distance<<std::endl;
    }
  }
  else
  {
  /*  if(scene == nullptr)
      std::cout << "no scene"<<std::endl;
    if(physics_system == nullptr)
      std::cout << "no phy sys"<<std::endl;*/
    *new_pos = npos;
    return true;

  }
  return false;
}