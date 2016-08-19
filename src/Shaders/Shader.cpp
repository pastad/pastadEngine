#include "Shader.h"

// to remove deprecation errors
#define GLM_FORCE_RADIANS

#include <glm/gtc/type_ptr.hpp>

#include "Log.h"
#include "Engine.h"
#include "IOSubsystem.h"
#include "glm/gtx/string_cast.hpp"
#include "glm/ext.hpp"

#include <iostream>

Shader::Shader():m_ready(false)
{
   m_program = gl::CreateProgram();
}

Shader::~Shader()
{
	
} 


//  creation/add/validation -------------------------------------------------

GLuint Shader::createShader(const std::string text, GLenum shader_type)
{
    GLuint shader = gl::CreateShader(shader_type);

    if(shader == 0)
      Engine::getLog()->log("Shader","Shader creation failed");

    const GLchar* shader_source_strings[1];
    GLint shader_source_strings_length[1];

    shader_source_strings[0] = text.c_str();
    shader_source_strings_length[0] = text.length();

    gl::ShaderSource(shader, 1, shader_source_strings, shader_source_strings_length);
    gl::CompileShader(shader);

    checkError(gl::COMPILE_STATUS, "compilation failed because of :", shader, false);

    return shader;
}

bool Shader::addShader(std::string path, ShaderType type)
{
  GLuint sh = createShader(IOSubsystem::readFile(path), type);
  gl::AttachShader(m_program, sh);
  m_shaders[type] = sh;

  return true;
} 

bool Shader::linkAndValidate()
{
  gl::BindAttribLocation(m_program, 0, "position");
  gl::BindAttribLocation(m_program, 1, "tex_coord");
  gl::BindAttribLocation(m_program, 2, "normal");

  gl::LinkProgram(m_program);
  checkError(gl::LINK_STATUS, "failed to link, because of :", m_program, true);

  gl::ValidateProgram(m_program);
  checkError( gl::VALIDATE_STATUS, "failed to vaildate, because of :", m_program, true);
  return true;
}


//  print -------------------------------------------------

void Shader::printUniforms()
{
  GLint num = 0;
  gl::GetProgramInterfaceiv(m_program, gl::UNIFORM, gl::ACTIVE_RESOURCES, &num);
  //Engine::getLog()->log("Shader","Nr active uniforms", std::string((int)num));
  GLenum properties[] = {gl::NAME_LENGTH, gl::TYPE,gl::LOCATION,gl::BLOCK_INDEX};
  for(int i=0;i<num; ++i)
  {
    GLint results[4];
    gl::GetProgramResourceiv(m_program,gl::UNIFORM, i,4,properties ,4,NULL,results);

    if(results[3] != -1)
      continue;
    GLint nameSize = results[0] + 1;
    char * name = new char[nameSize];
    gl::GetProgramResourceName(m_program, gl::UNIFORM, i, nameSize, NULL, name);
    std::cout <<results[2]<< "  "<<name<<results[1]<<std::endl;
    delete name;
  }
}


//  getter/setter -------------------------------------------------

void Shader::setUniform(const std::string name, glm::vec2 v)
{ 
  gl::Uniform2f(gl::GetUniformLocation(m_program, name.c_str()),v.x ,v.y);
}

void Shader::setUniform(const std::string name, glm::vec3 v)
{ 
  gl::Uniform3f(gl::GetUniformLocation(m_program, name.c_str()),v.x ,v.y ,v.z);
}

void Shader::setUniform(const std::string name, glm::vec4 v)
{
  gl::Uniform4f(gl::GetUniformLocation(m_program, name.c_str()),v.x ,v.y ,v.z, v.w);
}

void Shader::setUniform(const std::string name, glm::mat4 v)
{
  glm::mat4 t = v;
  //std::cout<<glm::to_string(t)<<std::endl;
  GLint handle = gl::GetUniformLocation(m_program, name.c_str());
  gl::UniformMatrix4fv(handle,1,gl::FALSE_,glm::value_ptr(t) );
}

void Shader::setUniform(const std::string name, float v)
{
  gl::Uniform1f(gl::GetUniformLocation(m_program, name.c_str()),v);
}

void Shader::setUniform(const std::string name, int v)
{
  gl::Uniform1i(gl::GetUniformLocation(m_program, name.c_str()), v);
}

void Shader::setRenderPassSubroutine(const std::string pass)
{
  GLuint p = gl::GetSubroutineIndex( m_program, gl::FRAGMENT_SHADER, pass.c_str());
  gl::UniformSubroutinesuiv( gl::FRAGMENT_SHADER, 1, &p);
}

bool Shader::isReady()
{
  return m_ready;
}


//  checks -------------------------------------------------

void Shader::checkUniformError(std::string place)
{
  GLenum err =gl::GetError();
  if(err!= gl::NO_ERROR_)
  {
    Engine::getLog()->log("Shader",place);
    if(err == gl::INVALID_ENUM)
        Engine::getLog()->log("Shader","GL_INVALID_ENUM");
    if(err == gl::INVALID_VALUE)
        Engine::getLog()->log("Shader","GL_INVALID_VALUE");
    if(err == gl::INVALID_OPERATION)
         Engine::getLog()->log("Shader","GL_INVALID_OPERATION");
    if(err == gl::INVALID_FRAMEBUFFER_OPERATION)
         Engine::getLog()->log("Shader","GL_INVALID_FRAMEBUFFER_OPERATION");

     Engine::errorShutDown();
  }
}

bool Shader::checkError(GLuint flag, std::string msg, GLuint shader, bool use_program)
{
  GLint success = 0;

  if(use_program)
    gl::GetProgramiv(shader, flag, &success);
  else
    gl::GetShaderiv(shader, flag, &success);

  if(success == gl::FALSE_)
  {
    GLchar error[1024] = { 0 };
    if(use_program)
      gl::GetProgramInfoLog(shader, sizeof(error), NULL, error);
    else
      gl::GetShaderInfoLog(shader, sizeof(error), NULL, error);
    Engine::getLog()->log("Shader", msg, error);     

    return true;
  }

  return false;
}


//  bind -------------------------------------------------
  
void Shader::bind()
{
  gl::UseProgram(m_program);
}