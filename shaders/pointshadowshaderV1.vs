#version 440

layout( location = 0 ) in vec3 VertexPosition;
layout( location = 1 ) in vec2 VertexCoord;
layout( location = 2 ) in vec3 VertexNormal;
layout( location = 4 ) in mat4 ModelMatrix;

uniform mat4 ViewMat;
uniform mat3 NormalMat;
uniform mat4 ProjectionMat;

out vec3 Pos;

void main()
{  
  mat4 MVMatrix = ViewMat* ModelMatrix;
  mat4 MVP      = ProjectionMat * MVMatrix ;  
  vec4 Position =  MVP * vec4(VertexPosition, 1.0f);
  gl_Position =  Position;
  Pos = ( ModelMatrix * vec4(VertexPosition, 1.0f) ).xyz;
 
}
