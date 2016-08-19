#ifndef SHADER_
#define SHADER_

#include <string>
#include <map>

#include "gl_core_4_.hpp"
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>


// basic shader class

// type of shaders that can be added
enum ShaderType
{
	VERTEX = gl::VERTEX_SHADER,
	FRAGMENT =  gl::FRAGMENT_SHADER,
	GEOMETRY =  gl::GEOMETRY_SHADER,
	COMPUTE =  gl::COMPUTE_SHADER,
	TESS_CONTROL =  gl::TESS_CONTROL_SHADER,
	TESS_EVAL =  gl::TESS_EVALUATION_SHADER

};

class Shader
{
public:
	Shader();
	~Shader();

protected:

	// ready query
	bool isReady();

	// adds a shader
	bool addShader(std::string path, ShaderType type);
	
	// links and vaildates the program
	bool linkAndValidate();	

	// binds the program for usage
	void bind();

	// uniform and subroutone setters
	void setUniform(const std::string name, glm::vec2 v);
	void setUniform(const std::string name, glm::vec3 v);
	void setUniform(const std::string name, glm::vec4 v);
	void setUniform(const std::string name, glm::mat4 v);
	void setUniform(const std::string name, float v);
	void setUniform(const std::string name, int v);
	void setRenderPassSubroutine(const std::string pass);

	// checks for program errors (true if failed)
	bool checkError(GLuint flag, std::string msg, GLuint shader,  bool use_program);
	
	//checks uniform errors
	void checkUniformError(std::string place);

	// prints the active uniforms
	void printUniforms();

private:

	// the program handle
	GLuint m_program;

	// determines if program is ready to use
	bool m_ready;

	// holds the shaders 
	std::map<ShaderType, GLuint> m_shaders;

	// creates the specific shader
	GLuint createShader(const std::string text, GLenum shader_type);

};

#endif