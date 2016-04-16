#ifndef RENDER_SHADER_H
#define RENDER_SHADER_H

// the shader that renders

#include <string>
#include <vector>

#include "Shader.h"
#include "Material.h"

class Light;

class RenderShader :  public Shader
{
public:

  RenderShader();
  ~RenderShader(); 

  // loads the shader
  bool load(const std::string path);

  // setters for matrices
  void setViewMatrix( glm::mat4 mvmatrix );
  void setProjectionMatrix( glm::mat4 projmatrix );
  void setNormalMatrix( glm::mat4 normalmatrix );

  // sets the render pass
  void setRenderPass(unsigned int pass);

  /// determines if material is only a color
  void setColorOnly(bool color_only);

  // sets the material that should be used
  void setMaterial(MaterialColorSpecs specs);

  // sets the lights that should be rendered
  void setLights(std::vector<Light*> * lights);

  // sets the camera for rendering effects
  void setCameraPosition(glm::vec3 pos);

  // binds the shader  
  void use();
  
private:

  // function for setting the lights
  void setDirectionalLight(Light* light, unsigned int pos);
  void setSpotLight(Light* light, unsigned int pos);
  void setPointLight(Light* light, unsigned int pos);

};

#endif