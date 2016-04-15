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
   m_program = glCreateProgram();
}

Shader::~Shader()
{
	
}

bool Shader::isReady()
{
	return m_ready;
}

bool Shader::addShader(std::string path, ShaderType type)
{
  GLuint sh = createShader(IOSubsystem::readFile(path), type);
  glAttachShader(m_program, sh);
  m_shaders[type] = sh;

  return true;
}  

bool Shader::linkAndValidate()
{
  glBindAttribLocation(m_program, 0, "position");
  glBindAttribLocation(m_program, 1, "tex_coord");
  glBindAttribLocation(m_program, 2, "normal");

  glLinkProgram(m_program);
  checkError(GL_LINK_STATUS, "failed to link, because of :", m_program, true);

  glValidateProgram(m_program);
  checkError( GL_VALIDATE_STATUS, "failed to vaildate, because of :", m_program, true);
  return true;
}
  
void Shader::bind()
{
  glUseProgram(m_program);
}

void Shader::setUniform(const std::string name, glm::vec3 v)
{ 
  glUniform3f(glGetUniformLocation(m_program, name.c_str()),v.x ,v.y ,v.z);
}

void Shader::setUniform(const std::string name, glm::vec4 v)
{
  glUniform4f(glGetUniformLocation(m_program, name.c_str()),v.x ,v.y ,v.z, v.w);
}

void Shader::setUniform(const std::string name, glm::mat4 v)
{
  glm::mat4 t = v;
  //std::cout<<glm::to_string(t)<<std::endl;
  GLint handle = glGetUniformLocation(m_program, name.c_str());
  glUniformMatrix4fv(handle,1,GL_FALSE,glm::value_ptr(t) );
}

void Shader::setUniform(const std::string name, float v)
{
  glUniform1f(glGetUniformLocation(m_program, name.c_str()),v);
}

void Shader::setUniform(const std::string name, int v)
{
  glUniform1i(glGetUniformLocation(m_program, name.c_str()), v);
}
void Shader::setRenderPassSubroutine(const std::string pass)
{
  GLuint p = glGetSubroutineIndex( m_program, GL_FRAGMENT_SHADER, pass.c_str());
  glUniformSubroutinesuiv( GL_FRAGMENT_SHADER, 1, &p);
}

bool Shader::checkError(GLuint flag, std::string msg, GLuint shader, bool use_program)
{
  GLint success = 0;

  if(use_program)
    glGetProgramiv(shader, flag, &success);
  else
    glGetShaderiv(shader, flag, &success);

  if(success == GL_FALSE)
  {
    GLchar error[1024] = { 0 };
    if(use_program)
      glGetProgramInfoLog(shader, sizeof(error), NULL, error);
    else
      glGetShaderInfoLog(shader, sizeof(error), NULL, error);
    Engine::getLog()->log("Shader", msg, error);     

    return true;
  }

  return false;
}

GLuint Shader::createShader(const std::string text, GLenum shader_type)
{
    GLuint shader = glCreateShader(shader_type);

    if(shader == 0)
      Engine::getLog()->log("Shader","Shader creation failed");

    const GLchar* shader_source_strings[1];
    GLint shader_source_strings_length[1];

    shader_source_strings[0] = text.c_str();
    shader_source_strings_length[0] = text.length();

    glShaderSource(shader, 1, shader_source_strings, shader_source_strings_length);
    glCompileShader(shader);

    checkError(GL_COMPILE_STATUS, "compilation failed because of :", shader, false);

    return shader;
}
void Shader::checkUniformError(std::string place)
{
  GLenum err =glGetError();
  if(err!=GL_NO_ERROR)
  {
    Engine::getLog()->log("Shader",place);
    if(err == GL_INVALID_ENUM)
        Engine::getLog()->log("Shader","GL_INVALID_ENUM");
    if(err == GL_INVALID_VALUE)
        Engine::getLog()->log("Shader","GL_INVALID_VALUE");
    if(err == GL_INVALID_OPERATION)
         Engine::getLog()->log("Shader","GL_INVALID_OPERATION");
    if(err == GL_INVALID_FRAMEBUFFER_OPERATION)
         Engine::getLog()->log("Shader","GL_INVALID_FRAMEBUFFER_OPERATION");

     Engine::errorShutDown();
  }
}

void Shader::printUniforms()
{
  GLint num = 0;
  glGetProgramInterfaceiv(m_program, GL_UNIFORM, GL_ACTIVE_RESOURCES, &num);
  //Engine::getLog()->log("Shader","Nr active uniforms", std::string((int)num));
  GLenum properties[] = {GL_NAME_LENGTH, GL_TYPE,GL_LOCATION,GL_BLOCK_INDEX};
  for(int i=0;i<num; ++i)
  {
    GLint results[4];
    glGetProgramResourceiv(m_program,GL_UNIFORM, i,4,properties ,4,NULL,results);

    if(results[3] != -1)
      continue;
    GLint nameSize = results[0] + 1;
    char * name = new char[nameSize];
    glGetProgramResourceName(m_program, GL_UNIFORM, i, nameSize, NULL, name);
    std::cout <<results[2]<< "  "<<name<<results[1]<<std::endl;
    delete name;
  }
}