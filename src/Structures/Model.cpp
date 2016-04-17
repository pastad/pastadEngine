#include "Model.h"

#include "Object.h"
#include "Material.h"
#include "Engine.h"
#include "Log.h"
#include "Mesh.h"
#include "RessourceManager.h"

#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/Importer.hpp>

#include <iostream>
#include <fstream>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Model::Model(std::string path):m_path(path)
{	
}

Model::~Model()
{
	for(std::vector<Object *>::iterator it = m_instances.begin(); it != m_instances.end();it++)
		delete (*it);	
  for(std::map<int,Material *>::iterator iterator = m_materials.begin(); iterator != m_materials.end(); iterator++)
    delete iterator->second;
    
}

Object * Model::getInstance()
{
	Object * obj = new Object();
	m_instances.insert(m_instances.end(),obj);
  
	return obj;
}

void Model::render(RenderShader * render_shader)
{	
  std::vector<glm::mat4> matrices;
  for(std::vector<Object *>::iterator it = m_instances.begin(); it != m_instances.end();it++)
      matrices.insert(matrices.end(),  (*it)->getModelMatrix()  );

  for(std::vector<Mesh *>::iterator it = m_meshes.begin(); it != m_meshes.end(); it++)
  {
    m_materials.at((*it)->getMaterialIndex())->bind(0, render_shader);
    (*it)->bufferModelMatrices(&matrices);
    (*it)->render(matrices.size());
  }
}

bool Model::load()
{
  m_animated = false;
  std::ifstream fin(m_path.c_str());
  if(!fin.fail())
      fin.close();
  else
  {
    Engine::getLog()->log("Model", "Couldn't open file");
    return false;
  }

  m_scene = m_importer.ReadFile( m_path, aiProcessPreset_TargetRealtime_Quality);
  m_scene = m_importer.GetOrphanedScene();

  if( !m_scene)
  {
    Engine::getLog()->log("Model", m_importer.GetErrorString());
    return false;
  }

  m_GlobalInverseTransform = m_scene->mRootNode->mTransformation;
  m_GlobalInverseTransform.Inverse();

  processScene();
  //Engine::getLog()->log("Model", "loaded. Nr meshes:",std::to_string(m_meshes.size()));
	return true;
}

void Model::processScene()
{
  int vertexes = 0;
  int indices =0;
  for (unsigned int i = 0 ; i <m_scene->mNumMeshes ; i++)
  {
    const aiMesh* mesh = m_scene->mMeshes[i];
    Mesh * n = NULL;
    n= new Mesh(mesh,mesh->mMaterialIndex);
    if(n == NULL)
      Engine::getLog()->log("Model","Mesh is null");
    m_meshes.push_back(n);      
  }

  std::string::size_type slashpos = m_path.find_last_of("/");
  std::string dir;

  if (slashpos == std::string::npos)
      dir = ".";
  else if (slashpos == 0)
      dir = "/";
  else
      dir = m_path.substr(0, slashpos);
  processMaterials(dir);
}

void Model::processMaterials(std::string directory)
{
  for (unsigned int i = 0 ; i < m_scene->mNumMaterials ; i++)
  {
    aiMaterial* pMaterial = m_scene->mMaterials[i];
    Material * mat = RessourceManager::loadMaterial(directory, pMaterial);
    m_materials.insert(m_materials.begin(),std::pair<int ,Material*>(i,mat));
  }
}