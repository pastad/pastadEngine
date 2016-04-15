#include "Scene.h"

#include "RessourceManager.h"
#include "Model.h"
#include "Object.h"
#include "Camera.h"

Scene::Scene()
{
  m_camera = new Camera(0,0,-5);
  
}
Scene::~Scene()
{

}

void Scene::update()
{
  m_camera->update();
}

void Scene::render()
{  
	for(std::map<std::string, Model *>::iterator it = m_models.begin(); it != m_models.end();it++)
    it->second->render();
}

Object * Scene::addObject(std::string path)
{
  Model * m = RessourceManager::loadModel(path);
  Object * obj = m->getInstance();
  m_objects.insert(m_objects.end(),obj);
  m_models[path] = m;

  return obj;
}

Camera * Scene::getCamera()
{
  return m_camera;
}