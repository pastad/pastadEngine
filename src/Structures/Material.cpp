#include "Material.h"
#include "Texture.h"
#include "Log.h"
#include "Engine.h"
#include "RenderSubsystem.h"
#include "RenderShader.h"

#include "RessourceManager.h"

Material::Material()
{
}

Material::~Material()
{
    for(std::map<int,Texture *>::iterator iterator = m_textures.begin(); iterator != m_textures.end(); iterator++)
    {
        delete iterator->second;
    }
}

Material::Material(aiMaterial * pMaterial, std::string dir)
{
  aiString name;
  pMaterial->Get(AI_MATKEY_NAME,name);

  // TODO:must be reimplemented to include all textures
  std::string n(name.C_Str());
  Engine::getLog()->log("Material",n);

  // if mat color is a texture load texture
  if (pMaterial->GetTextureCount(aiTextureType_DIFFUSE) > 0)
  {
    aiString Path;

    // if a texture can be found
    if (pMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &Path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS)
    {
      std::string fi(Path.data);
      std::string::size_type slashpos2 = fi.find_last_of("/");
      std::string p2 ;
      if (slashpos2 != std::string::npos)
          p2 = fi.substr(slashpos2+1, fi.size());
      else
          p2 = fi;

      std::string path = dir + "/" + p2;

      // load it and store it in our mapping
      Texture * tex = RessourceManager::loadTexture(path);
      m_textures.insert(m_textures.begin(),std::pair<int ,Texture *>(0,tex));
      m_diffuse_texture_count++;
    }
  }
  else
  {
    aiColor4D diffuse,specular,ambient;
    float opacity,shininess;
    aiGetMaterialColor(pMaterial, AI_MATKEY_COLOR_DIFFUSE, &diffuse);
    aiGetMaterialColor(pMaterial, AI_MATKEY_COLOR_SPECULAR, &specular);
    aiGetMaterialColor(pMaterial, AI_MATKEY_COLOR_AMBIENT, &ambient);
    aiGetMaterialFloat(pMaterial, AI_MATKEY_OPACITY, &opacity);
    aiGetMaterialFloat(pMaterial, AI_MATKEY_SHININESS, &shininess);
    m_material_specs.m_opacity = opacity;
    m_material_specs.m_shininess = shininess;

    m_material_specs.m_diffuse_color = glm::vec3(diffuse.r,diffuse.g,diffuse.b);
 
    m_material_specs.m_ambient_color  = glm::vec3(ambient.r,ambient.g,ambient.b);
    if( (m_material_specs.m_ambient_color .x == 0) && (m_material_specs.m_ambient_color .y == 0) 
      && (m_material_specs.m_ambient_color .z == 0) )
    {
        m_material_specs.m_ambient_color = m_material_specs.m_diffuse_color ;
    }

    m_material_specs.m_specular_color = glm::vec3(specular.r,specular.g,specular.b);

  }
}

void Material::bind(int unit)
{
  if(m_diffuse_texture_count!=0)
      m_textures.at(0)->bind(unit);
  else
  {
      Engine::getRenderSubsystem()->getRenderShader()->setColorOnly(true); 
  }
  Engine::getRenderSubsystem()->getRenderShader()->setMaterial(m_material_specs); 

    //    RenderSystem::getInstance()->getShader()->setColorOnly(1);
   // RenderSystem::getInstance()->getShader()->setMaterial(m_diffuse_color,m_ambient_color,
    //m_specular_color,m_shininess,m_opacity);
}
bool Material::isTransparent()
{
  if(m_material_specs.m_opacity != 1.0)
    return true;
  return false;
}

