#version 440

layout( location = 0 ) in vec3  VertexPosition;
layout( location = 1 ) in vec2  VertexCoord;
layout( location = 2 ) in vec3  VertexNormal;
layout( location = 3 ) in mat4  ModelMatrix;

out vec3 Position;
out vec3 Normal;
out vec2 TexCoord;
out vec4 ModelPos;

// mats that are allways set
uniform mat4 ViewMat;
uniform mat4 ProjectionMat;

// specific mats

uniform mat4 SingleModelMatrix;
uniform int Instanced;


float generationFunction(float x, float y)
{
  
  return 0;//return +sin(x)+cos(y)-1;
}

void main()
{ 
  mat4 ModelMat = SingleModelMatrix ;    
  vec4 VertexPos = vec4(VertexPosition,1.0f);
  VertexPos.y += generationFunction(VertexPos.x,VertexPos.z);

  mat4 MVMatrix = ViewMat* ModelMat;
  mat4 MVP      = ProjectionMat * MVMatrix ;    
  Position = vec3(ModelMat *  VertexPos  );
  ModelPos = ModelMat* VertexPos;
  gl_Position =  MVP *  VertexPos;

  mat3 NormalMatrix = mat3( ModelMat);
  Normal = normalize( NormalMatrix * VertexNormal);  

  TexCoord = VertexCoord;
}
