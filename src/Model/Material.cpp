#include "Material.h"
#include "Texture.h"
#include "Log.h"
#include "Engine.h"
#include "RenderSubsystem.h"
#include "RenderShader.h"

#include "RessourceManager.h"

#include <iostream>

Material::Material()
{
}

Material::~Material()
{  
}

Material::Material(aiMaterial * pMaterial, std::string dir)
{
  aiString name;  
  pMaterial->Get(AI_MATKEY_NAME,name);

  // TODO:must be reimplemented to include all textures
  std::string n(name.C_Str());
  //Engine::getLog()->log("Material",n);
  m_name = n;
  
  m_base_texture_set = false;

  unsigned int num_diffuse_textures = pMaterial->GetTextureCount(aiTextureType_DIFFUSE);
  unsigned int num_ambient_textures = pMaterial->GetTextureCount(aiTextureType_AMBIENT);
  unsigned int num_specular_textures = pMaterial->GetTextureCount(aiTextureType_SPECULAR);
  unsigned int num_normal_textures = pMaterial->GetTextureCount(aiTextureType_NORMALS);
  unsigned int num_unkown_textures = pMaterial->GetTextureCount(aiTextureType_HEIGHT);
  unsigned int num_none_textures = pMaterial->GetTextureCount(aiTextureType_NONE);
  unsigned int num_opacity_textures = pMaterial->GetTextureCount(aiTextureType_OPACITY);
  std::cout << "Loading "<< num_diffuse_textures <<" diffuse textures"<<std::endl;
  std::cout << "Listed " << num_ambient_textures << " ambient textures" << std::endl;
  std::cout << "Listed " << num_specular_textures << " specular textures" << std::endl;
  std::cout << "Listed " << num_normal_textures << " normal textures" << std::endl;
  std::cout << "Listed " << num_unkown_textures << " height textures" << std::endl;
  std::cout << "Listed " << num_opacity_textures << " opacity textures" << std::endl;
  std::cout << "Listed " << num_opacity_textures << " none textures" << std::endl;

  // load diffuse texturess
  for(int x=0; x< num_diffuse_textures;x++)
  {
    aiString Path;
    float strength;

    // if a texture can be found
    if (pMaterial->GetTexture(aiTextureType_DIFFUSE, x, &Path, NULL, NULL, &strength, NULL, NULL) == AI_SUCCESS)
    {
      std::string fi(Path.data);
      std::string::size_type slashpos2 = fi.find_last_of("/");
      std::string p2 ;
      if (slashpos2 != std::string::npos)
          p2 = fi.substr(slashpos2+1, fi.size());
      else
          p2 = fi;

      std::string path = dir + "/" + p2;

      std::string t("C:");

      if (p2.compare(0, t.length(), t) == 0)
      {
        path = p2;
      }

      std::cout << path << std::endl;

      // load it and store it in our mapping
      Texture * tex = RessourceManager::loadTexture(path);
      m_diffuse_textures.insert(m_diffuse_textures.end(),std::pair<int ,Texture *>(x,tex));
      m_diffuse_texture_strengths.insert(m_diffuse_texture_strengths.end(),std::pair<int ,int>(x,strength));
      m_base_texture_set = true;
    }
  }  

  aiString Path, Path2;
  float strength, strength2;


  if (pMaterial->GetTexture(aiTextureType_AMBIENT, 0, &Path, NULL, NULL, &strength, NULL, NULL) == AI_SUCCESS)
  {
    std::string fi(Path.data);
    std::string::size_type slashpos2 = fi.find_last_of("/");
    std::string p2;
    if (slashpos2 != std::string::npos)
      p2 = fi.substr(slashpos2 + 1, fi.size());
    else
      p2 = fi;

    std::string path = dir + "/" + p2;

    // load it and store it in our mapping
    m_ambient_texture = RessourceManager::loadTexture(path);
  }
  if (pMaterial->GetTexture(aiTextureType_SPECULAR, 0, &Path, NULL, NULL, &strength, NULL, NULL) == AI_SUCCESS)
  {
    std::string fi(Path.data);
    std::string::size_type slashpos2 = fi.find_last_of("/");
    std::string p2;
    if (slashpos2 != std::string::npos)
      p2 = fi.substr(slashpos2 + 1, fi.size());
    else
      p2 = fi;

    std::string path = dir + "/" + p2;

    // load it and store it in our mapping
    m_specular_texture = RessourceManager::loadTexture(path);
  }



  if (pMaterial->GetTexture(aiTextureType_NORMALS, 0, &Path, NULL, NULL, &strength, NULL, NULL) == AI_SUCCESS)
  {
    std::string fi(Path.data);
    std::string::size_type slashpos2 = fi.find_last_of("/");
    std::string p2;
    if (slashpos2 != std::string::npos)
      p2 = fi.substr(slashpos2 + 1, fi.size());
    else
      p2 = fi;

    std::string path = dir + "/" + p2;

    // load it and store it in our mapping
    m_normal_texture = RessourceManager::loadTexture(path);

  }

  if (pMaterial->GetTexture(aiTextureType_HEIGHT, 0, &Path, NULL, NULL, &strength, NULL, NULL) == AI_SUCCESS)
  {
    std::string fi(Path.data);
    std::string::size_type slashpos2 = fi.find_last_of("/");
    std::string p2;
    if (slashpos2 != std::string::npos)
      p2 = fi.substr(slashpos2 + 1, fi.size());
    else
      p2 = fi;

    std::string path = dir + "/" + p2;

    // load it and store it in our mapping
    m_normal_texture = RessourceManager::loadTexture(path); // because assimp programmers think HEIGHT == NORMALS ...
   
  }
  if (pMaterial->GetTexture(aiTextureType_OPACITY, 0, &Path2, NULL, NULL, &strength2, NULL, NULL) == AI_SUCCESS)
  {
    std::string fi(Path2.data);
    std::string::size_type slashpos2 = fi.find_last_of("/");
    std::string p2;
    if (slashpos2 != std::string::npos)
      p2 = fi.substr(slashpos2 + 1, fi.size());
    else
      p2 = fi;

    std::string path = dir + "/" + p2;

    // load it and store it in our mapping
    m_opacity_texture = RessourceManager::loadTexture(path);
    m_base_texture_set = true;
  }
 
  aiColor4D diffuse,specular,ambient,emmissive;
  float opacity,shininess;
  aiGetMaterialColor(pMaterial, AI_MATKEY_COLOR_DIFFUSE, &diffuse);
  aiGetMaterialColor(pMaterial, AI_MATKEY_COLOR_SPECULAR, &specular);
  aiGetMaterialColor(pMaterial, AI_MATKEY_COLOR_AMBIENT, &ambient);
  aiGetMaterialColor(pMaterial, AI_MATKEY_COLOR_EMISSIVE, &emmissive);
  aiGetMaterialFloat(pMaterial, AI_MATKEY_OPACITY, &opacity);
  aiGetMaterialFloat(pMaterial, AI_MATKEY_SHININESS, &shininess); 
  m_material_specs.m_opacity = opacity;
  m_material_specs.m_shininess = shininess;

  m_material_specs.m_diffuse_color = glm::vec3(diffuse.r,diffuse.g,diffuse.b);
  m_material_specs.m_ambient_color  = glm::vec3(ambient.r,ambient.g,ambient.b);  
  m_material_specs.m_specular_color = glm::vec3(specular.r,specular.g,specular.b);

  // if ambient color is not set use the diffuse color
  if( (m_material_specs.m_ambient_color .x == 0) && (m_material_specs.m_ambient_color .y == 0) 
    && (m_material_specs.m_ambient_color .z == 0) )
  {
      m_material_specs.m_ambient_color = m_material_specs.m_diffuse_color ;
  }
  m_material_specs.m_emmissive = 0.0f;
  if( (emmissive.r != 0.0f) || (emmissive.g != 0.0f) 
    || (emmissive.b != 0.0f) )  
  {
    std::cout <<"emissive material"<<std::endl;
    m_material_specs.m_emmissive = 1.0f;
  }
}


// bind -------------------------------------------------

void Material::bind(int unit, RenderShader * render_shader)
{
  if(m_base_texture_set)
  {
    render_shader->setColorOnly(false); 
    int c =0;

    for(int x=0; x < m_diffuse_textures.size() && (x <3) ;x++)
    {
      m_diffuse_textures.at(x)->bind(x);
      c++;
    }

    if (m_specular_texture != nullptr)
    {
      m_specular_texture->bind(9);
      render_shader->setSpecularMapActive(1);
      // std::cout << "set norm" <<std::endl; 
    }
    else
      render_shader->setSpecularMapActive(0);


    if( m_normal_texture != nullptr)
    {      
      m_normal_texture->bind(7);
      render_shader->setNormalMapActive(1);
     // std::cout << "set norm" <<std::endl; 
    }
    else
      render_shader->setNormalMapActive(0);

    if (m_height_texture != nullptr)
    {
      m_height_texture->bind(6);
      render_shader->setBumpMapActive(1);
      // std::cout << "set norm" <<std::endl; 
    }
    else
      render_shader->setBumpMapActive(0);

    if (m_opacity_texture != nullptr)
    {
      m_opacity_texture->bind(5);
      render_shader->setOpacityMapActive(1);
      // std::cout << "set norm" <<std::endl; 
    }
    else
      render_shader->setOpacityMapActive(0);
  }
  else
  {
    render_shader->setColorOnly(true); 
  }
  render_shader->setMaterial(m_name,m_material_specs); 
}


// getter/setters -------------------------------------------------

bool Material::isTransparent()
{
  if(  (m_material_specs.m_opacity != 1.0) || (m_opacity_texture != nullptr) )
    return true;
  return false;
}

void Material::setEmmissive(float val)
{
  m_material_specs.m_emmissive = val;
}
