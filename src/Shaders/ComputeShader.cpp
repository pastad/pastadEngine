#include "ComputeShader.h"

#include "Engine.h"
#include "Log.h"

#include <iostream>

ComputeShader::ComputeShader()
{
}


ComputeShader::~ComputeShader()
{
}

bool ComputeShader::load(std::string path)
{
  if (!addShader(path + ".cs", COMPUTE))
    return false;

  linkAndValidate();

  Engine::getLog()->log("ComputeShader", "loaded");
  return true;

}

//  use -------------------------------------------------

void ComputeShader::use()
{
  Shader::bind();
}

void ComputeShader::compute( int x , int y, int z)
{
  gl::DispatchCompute(x, y, z);
  gl::MemoryBarrier(  gl::SHADER_STORAGE_BARRIER_BIT);
}