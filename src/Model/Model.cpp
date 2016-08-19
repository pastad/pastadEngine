#include "Model.h"

#include "Object.h"
#include "Material.h"

#include "Log.h"
#include "Mesh.h"
#include "BoundingBox.h"
#include "Helper.h"
#include "AnimationMesh.h"
#include "RenderShader.h"
#include "RenderBaseShader.h"
#include "RessourceManager.h"

#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/Importer.hpp>

#include <iostream>
#include <fstream>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

Model::Model(std::string path, bool instanced):m_path(path), m_instanced(instanced)
{	
}

Model::~Model()
{
	for(std::vector<Object *>::iterator it = m_instances.begin(); it != m_instances.end();it++)
		delete (*it);	
  for(std::map<int,Material *>::iterator iterator = m_materials.begin(); iterator != m_materials.end(); iterator++)
    delete iterator->second;
    
}




//  render -------------------------------------------------

void Model::render(RenderBaseShader * render_shader, bool with_material)
{
  render(render_shader, m_instances, with_material);
}

void Model::render(RenderBaseShader * render_shader, std::vector<Object *> objects, bool with_material)
{
/*  std::vector<glm::mat4> matrices;
  for(std::vector<Object *>::iterator it = objects.begin(); it != objects.end();it++)
  {
    if( (*it)->isVisible() )
      matrices.insert(matrices.end(),  (*it)->getModelMatrix()  );
  }*/
  //if(m_instanced_matrices_refresh_needed)
    refreshBufferedMatrices(objects);

  if( !m_animated )
  {
    if(m_instanced)
    {
      render_shader->setInstanced();
      for(std::vector<Mesh *>::iterator it = m_meshes.begin(); it != m_meshes.end(); it++)
      {       
        if( with_material )
          m_materials.at((*it)->getMaterialIndex())->bind(0, (RenderShader *) render_shader);

        (*it)->render(m_num_instanced_buffered_matrices); 
      }
    }
    else
    {
      for(std::vector<Mesh *>::iterator it = m_meshes.begin(); it != m_meshes.end(); it++)
      {
        if( with_material )
          m_materials.at((*it)->getMaterialIndex())->bind(0, (RenderShader *) render_shader);     
        for(std::vector<Object *>::iterator it2 = objects.begin(); it2 != objects.end();it2++)
        {
          if( (*it2)->isVisible() )
          {
            render_shader->setNotInstanced( (*it2)->getModelMatrix() );
            render_shader->setIndentifcation((*it2)->getId());
            (*it)->render();
          }
        }
      }
    }
  }
  else
  {
    //animated object
    
    render_shader->setAnimation(); 

    glm::mat4 Identity = glm::mat4();

    for(std::vector<Object *>::iterator it_objs = m_instances.begin(); it_objs != m_instances.end();it_objs++)
    {
      if( (*it_objs)->isVisible() )
      {
        render_shader->setNotInstanced((*it_objs)->getModelMatrix() );
        render_shader->setIndentifcation((*it_objs)->getId());

        readNodeHeirarchy( (*it_objs)->getAnimationTime(), m_scene->mRootNode, Identity);
        
        for(std::vector<AnimationMesh *>::iterator it = m_animation_meshes.begin(); it != m_animation_meshes.end(); it++)
        {
          std::vector<glm::mat4> transforms;
          transforms.resize((*it)->getNumBones());
          (*it)->updateTransforms(&transforms, render_shader);

          if( with_material )
            m_materials.at((*it)->getMaterialIndex())->bind(0, (RenderShader *) render_shader); 

          (*it)->render(); 
        } 
      }
    }
    render_shader->unsetAnimation();
  }
}


//  loading/processing/traversing -------------------------------------------------

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
 // m_scene = m_importer.GetOrphanedScene();

  
  if( !m_scene)
  {
    Engine::getLog()->log("Model","Loading error", m_importer.GetErrorString());
    return false;
  }

  m_GlobalInverseTransform = m_scene->mRootNode->mTransformation;
  m_GlobalInverseTransform.Inverse();

  processScene();
	return true;
}

void Model::processScene()
{
  int vertexes = 0;
  int indices =0;

  if(m_scene->HasAnimations())
  {
    Engine::getLog()->log("Model", "is animated"); 
    std::vector<BoundingBox *> bbs;
    for (unsigned int i = 0 ; i <m_scene->mNumMeshes ; i++)
    {
      const aiMesh* mesh = m_scene->mMeshes[i];
      AnimationMesh * n = nullptr;

      n= new AnimationMesh(mesh,mesh->mMaterialIndex);
      if(n == nullptr)
        Engine::getLog()->log("Model","AnimationMesh is null");
      else
      {        
        m_animation_meshes.push_back(n);   
        bbs.push_back(n->getBoundingBox());
      }
    }
    m_bounding_box = new BoundingBox(bbs);
    m_animated = true;
  }
  else
  {
    std::vector<BoundingBox *> bbs;
    for (unsigned int i = 0 ; i <m_scene->mNumMeshes ; i++)
    {
      const aiMesh* mesh = m_scene->mMeshes[i];
      Mesh * n = nullptr;

      n= new Mesh(mesh,mesh->mMaterialIndex);
      if(n == nullptr)
        Engine::getLog()->log("Model","Mesh is null");
      else
      {        
        m_meshes.push_back(n);  
        bbs.push_back(n->getBoundingBox());
      }    
    }
    m_bounding_box = new BoundingBox(bbs);
    m_animated = false;
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

void Model::readNodeHeirarchy(float animation_time, const aiNode* pNode, const glm::mat4& parentTransform)
{
  std::string NodeName(pNode->mName.data);

  const aiAnimation* pAnimation = m_scene->mAnimations[0];

  glm::mat4 NodeTransformation(Helper::toGlm(pNode->mTransformation));

  const aiNodeAnim* pNodeAnim = findNodeAnim(pAnimation, NodeName);
  if (pNodeAnim)
  {
    // Interpolate scaling and generate scaling transformation matrix
    aiVector3D Scaling;
    calcInterpolatedScaling(Scaling, animation_time, pNodeAnim);
    glm::mat4 ScalingM (1.0)  ;
    ScalingM[0].x =Scaling.x;ScalingM[1].y =  Scaling.y;ScalingM[2].z = Scaling.z;

    // Interpolate rotation and generate rotation transformation matrix
    aiQuaternion RotationQ;
    calcInterpolatedRotation(RotationQ, animation_time, pNodeAnim);
    glm::mat4 RotationM = Helper::toGlm(RotationQ.GetMatrix());
    //std::cout<<glm::to_string(RotationM)<<std::endl;

    // Interpolate translation and generate translation transformation matrix
    aiVector3D Translation;
    calcInterpolatedPosition(Translation, animation_time, pNodeAnim);
    glm::mat4 TranslationM = glm::translate( glm::mat4( 1.0f ), glm::vec3( Translation.x,Translation.y,Translation.z ) );
 
    // Combine the above transformations
    NodeTransformation =   TranslationM * RotationM * ScalingM; 
  }

  glm::mat4   GlobalTransformation =    parentTransform   *NodeTransformation   ;
  glm::mat4 inverse = Helper::toGlm(m_GlobalInverseTransform);

  for(std::vector<AnimationMesh *>::iterator it = m_animation_meshes.begin(); it != m_animation_meshes.end(); ++it)
  {
    (*it)->applyTransform(NodeName,inverse,GlobalTransformation);
  }

  for (unsigned int i = 0 ; i < pNode->mNumChildren ; i++)
  {
    readNodeHeirarchy(animation_time, pNode->mChildren[i], GlobalTransformation);
  }

}

/*
<<<<<<< HEAD:src/Structures/Model.cpp
unsigned int Model::findPosition(float animation_time, const aiNodeAnim* pNodeAnim)
{
  for (unsigned int i = 0 ; i < pNodeAnim->mNumPositionKeys - 1 ; i++)
  {
    if (animation_time < (float)pNodeAnim->mPositionKeys[i + 1].mTime)
      return i;
    
  }
  assert(0);

  return 0;
}

unsigned int Model::findRotation(float animation_time, const aiNodeAnim* pNodeAnim)
{
  assert(pNodeAnim->mNumRotationKeys > 0);

  for (unsigned int i = 0 ; i < pNodeAnim->mNumRotationKeys - 1 ; i++)
  {
    if (animation_time < (float)pNodeAnim->mRotationKeys[i + 1].mTime)
      return i;    
  }
  assert(0);

  return 0;
}

unsigned int Model::findScaling(float animation_time, const aiNodeAnim* pNodeAnim)
{
  assert(pNodeAnim->mNumScalingKeys > 0);

  for (unsigned int i = 0 ; i < pNodeAnim->mNumScalingKeys - 1 ; i++)
  {
    if (animation_time < (float)pNodeAnim->mScalingKeys[i + 1].mTime)
      return i;    
  }
  assert(0);

  return 0;
}

=======
//  calculations -------------------------------------------------
>>>>>>> master:src/Model/Model.cpp */

void Model::calcInterpolatedPosition(aiVector3D& out, float animation_time, const aiNodeAnim* pNodeAnim)
{
  if (pNodeAnim->mNumPositionKeys == 1)
  {
    out = pNodeAnim->mPositionKeys[0].mValue;
    return;
  }
  unsigned int PositionIndex = findPosition(animation_time, pNodeAnim);
  unsigned int NextPositionIndex = (PositionIndex + 1);
  assert(NextPositionIndex < pNodeAnim->mNumPositionKeys);
  float DeltaTime = (float)(pNodeAnim->mPositionKeys[NextPositionIndex].mTime - pNodeAnim->mPositionKeys[PositionIndex].mTime);
  float Factor = (animation_time - (float)pNodeAnim->mPositionKeys[PositionIndex].mTime) / DeltaTime;
  assert(Factor >= 0.0f && Factor <= 1.0f);
  const aiVector3D& Start = pNodeAnim->mPositionKeys[PositionIndex].mValue;
  const aiVector3D& End = pNodeAnim->mPositionKeys[NextPositionIndex].mValue;
  aiVector3D Delta = End - Start;
  out = Start + Factor * Delta;
}

void Model::calcInterpolatedRotation(aiQuaternion& out, float animation_time, const aiNodeAnim* pNodeAnim)
{
  // we need at least two values to interpolate...
  if (pNodeAnim->mNumRotationKeys == 1)
  {
    out = pNodeAnim->mRotationKeys[0].mValue;
    return;
  }
  unsigned int RotationIndex = findRotation(animation_time, pNodeAnim);
  unsigned int NextRotationIndex = (RotationIndex + 1);
  assert(NextRotationIndex < pNodeAnim->mNumRotationKeys);
  float DeltaTime = (float)(pNodeAnim->mRotationKeys[NextRotationIndex].mTime - pNodeAnim->mRotationKeys[RotationIndex].mTime);
  float Factor = (animation_time - (float)pNodeAnim->mRotationKeys[RotationIndex].mTime) / DeltaTime;
  assert(Factor >= 0.0f && Factor <= 1.0f);
  const aiQuaternion& StartRotationQ = pNodeAnim->mRotationKeys[RotationIndex].mValue;
  const aiQuaternion& EndRotationQ   = pNodeAnim->mRotationKeys[NextRotationIndex].mValue;
  aiQuaternion::Interpolate(out, StartRotationQ, EndRotationQ, Factor);
  out = out.Normalize();
}

void Model::calcInterpolatedScaling(aiVector3D& Out, float animation_time, const aiNodeAnim* pNodeAnim)
{
  if (pNodeAnim->mNumScalingKeys == 1)
  {
    Out = pNodeAnim->mScalingKeys[0].mValue;
    return;
  }
  unsigned int ScalingIndex = findScaling(animation_time, pNodeAnim);
  unsigned int NextScalingIndex = (ScalingIndex + 1);
  assert(NextScalingIndex < pNodeAnim->mNumScalingKeys);
  float DeltaTime = (float)(pNodeAnim->mScalingKeys[NextScalingIndex].mTime - pNodeAnim->mScalingKeys[ScalingIndex].mTime);
  float Factor = (animation_time - (float)pNodeAnim->mScalingKeys[ScalingIndex].mTime) / DeltaTime;
  assert(Factor >= 0.0f && Factor <= 1.0f);
  const aiVector3D& Start = pNodeAnim->mScalingKeys[ScalingIndex].mValue;
  const aiVector3D& End   = pNodeAnim->mScalingKeys[NextScalingIndex].mValue;
  aiVector3D Delta = End - Start;
  Out = Start + Factor * Delta;
}


//  finders -------------------------------------------------

const aiNodeAnim* Model::findNodeAnim(const aiAnimation* pAnimation, const std::string nodeName)
{
  for (unsigned int i = 0 ; i < pAnimation->mNumChannels ; i++)
  {
    const aiNodeAnim* pNodeAnim = pAnimation->mChannels[i];

    if (std::string(pNodeAnim->mNodeName.data) == nodeName)
    {
      return pNodeAnim;
    }
  }

  return nullptr;
}

unsigned int Model::findPosition(float animation_time, const aiNodeAnim* pNodeAnim)
{
  for (unsigned int i = 0 ; i < pNodeAnim->mNumPositionKeys - 1 ; i++)
  {
    if (animation_time < (float)pNodeAnim->mPositionKeys[i + 1].mTime)
      return i;
    
  }
  assert(0);

  return 0;
}

unsigned int Model::findRotation(float animation_time, const aiNodeAnim* pNodeAnim)
{
  assert(pNodeAnim->mNumRotationKeys > 0);

  for (unsigned int i = 0 ; i < pNodeAnim->mNumRotationKeys - 1 ; i++)
  {
    if (animation_time < (float)pNodeAnim->mRotationKeys[i + 1].mTime)
      return i;    
  }
  assert(0);

  return 0;
}

unsigned int Model::findScaling(float animation_time, const aiNodeAnim* pNodeAnim)
{
  assert(pNodeAnim->mNumScalingKeys > 0);

  for (unsigned int i = 0 ; i < pNodeAnim->mNumScalingKeys - 1 ; i++)
  {
    if (animation_time < (float)pNodeAnim->mScalingKeys[i + 1].mTime)
      return i;    
  }
  assert(0);

  return 0;
}



// getter/setters -------------------------------------------------

std::vector<Object *> Model::getObjects()
{
  return m_instances;
}

BoundingBox * Model::getBoundingBox()
{
  return m_bounding_box;
}

std::vector<Mesh*> Model::getMeshes()
{
  return m_meshes;
}

const aiScene * Model::getAssimpScene()
{
  return m_scene;
}

bool Model::isAnimated()
{
  return m_animated;
}

Object * Model::getInstance()
{
  Object * obj = new Object(m_path, this);

  m_instances.insert(m_instances.end(),obj);
  
  return obj;
}
void Model::removeInstance( Object * obj)
{
  for(std::vector<Object *>::iterator it = m_instances.begin(); it != m_instances.end();)
  {
    if( (*it) == obj)
    {
      m_instances.erase(it);
      it = m_instances.end();
    }
    else
      it++;
  }
}

std::map<int,Material *> Model::getMaterials()
{
  return m_materials;
}

// refreshers

void Model::refreshBufferedMatrices(std::vector<Object *> instances)
{
  std::vector<glm::mat4> matrices;
  for(std::vector<Object *>::iterator it = instances.begin(); it != instances.end();it++)
  {
    if( (*it)->isVisible() )
      matrices.insert(matrices.end(),  (*it)->getModelMatrix()  );
  }
  if( !m_animated )
  {
    if(m_instanced)
    {
      for(std::vector<Mesh *>::iterator it = m_meshes.begin(); it != m_meshes.end(); it++)
      {       
        (*it)->bufferModelMatrices(&matrices);
      }
      m_num_instanced_buffered_matrices = matrices.size();
     // Engine::getLog()->log("Model","Rebuffered instanced matrices");
    }

  }
}
