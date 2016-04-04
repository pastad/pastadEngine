#include "Shader.h"

// to remove deprecation errors
#define GLM_FORCE_RADIANS

#include <glm/gtc/type_ptr.hpp>

#include "Log.h"
#include "Engine.h"
#include "IOSubsystem.h"



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
  m_shaders[type] = sh;

  return true;
}  

bool Shader::linkAndValidate()
{
  glLinkProgram(m_program);
  checkError(GL_LINK_STATUS, "ERROR: Programm failed to link" );

  glValidateProgram(m_program);
  checkError( GL_VALIDATE_STATUS, "ERROR: Programm validation to link" );
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
  glUniformMatrix4fv(glGetUniformLocation(m_program, name.c_str()),1,GL_FALSE,glm::value_ptr(v) );
}

void Shader::setUniform(const std::string name, float v)
{
  glUniform1f(glGetUniformLocation(m_program, name.c_str()),v);
}

void Shader::setUniform(const std::string name, int v)
{
  glUniform1i(glGetUniformLocation(m_program, name.c_str()), v);
}

bool Shader::checkError(GLuint flag, std::string msg)
{
  GLint success = 0;
  glGetProgramiv(m_program, flag, &success);

  if(success == GL_FALSE)
  {
    GLchar error[1024] = { 0 };
    glGetProgramInfoLog(m_program, sizeof(error), NULL, error);
    Engine::getLog()->log("Shader", msg);  
    Engine::getLog()->log("Shader", error);    

    return true;
  }

  return false;
}

GLuint Shader::createShader(const std::string text, GLenum shader_type)
{
    GLuint shader = glCreateShader(shader_type);

    if(shader == 0)
      Engine::getLog()->log("Shader","ERROR: Shader creation failed");

    const GLchar* shader_source_strings[1];
    GLint shader_source_strings_length[1];

    shader_source_strings[0] = text.c_str();
    shader_source_strings_length[0] = text.length();

    glShaderSource(shader, 1, shader_source_strings, shader_source_strings_length);
    glCompileShader(shader);

    checkError(GL_COMPILE_STATUS, "ERROR: Shader compilation failed" );

    return shader;
}