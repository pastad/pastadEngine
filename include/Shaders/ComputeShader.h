#pragma once
#include "Shader.h"


class ComputeShader :  public Shader
{
public:
  ComputeShader();
  ~ComputeShader();


  // loads the shader
  bool load(const std::string path);

  // ready query
  using Shader::isReady;
  using Shader::addShader;
  using Shader::linkAndValidate;
  using Shader::bind;
  using Shader::setUniform;
  using Shader::setRenderPassSubroutine;
  using Shader::checkError;
  using Shader::checkUniformError;


  void use();
  void compute(int x, int y, int z);
};

