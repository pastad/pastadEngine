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

      // load it and store it in our mapping
      Texture * tex = RessourceManager::loadTexture(path);
      m_diffuse_textures.insert(m_diffuse_textures.end(),std::pair<int ,Texture *>(x,tex));
      m_diffuse_texture_strengths.insert(m_diffuse_texture_strengths.end(),std::pair<int ,int>(x,strength));
      m_base_texture_set = true;
    }
  }  
 
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
  m_material_specs.m_specular_color = glm::vec3(specular.r,specular.g,specular.b);

  // if ambient color is not set use the diffuse color
  if( (m_material_specs.m_ambient_color .x == 0) && (m_material_specs.m_ambient_color .y == 0) 
    && (m_material_specs.m_ambient_color .z == 0) )
  {
      m_material_specs.m_ambient_color = m_material_specs.m_diffuse_color ;
  }
}

void Material::bind(int unit, RenderShader * render_shader)
{
  if(m_base_texture_set)
  {    
    render_shader->setColorOnly(false); 
    for(int x=0; x < m_diffuse_textures.size();x++)
      m_diffuse_textures.at(x)->bind(unit+x, render_shader);
  }
  else
  {
    render_shader->setColorOnly(true); 
  }
  render_shader->setMaterial(m_name,m_material_specs); 
}
bool Material::isTransparent()
{
  if(m_material_specs.m_opacity != 1.0)
    return true;
  return false;
}

