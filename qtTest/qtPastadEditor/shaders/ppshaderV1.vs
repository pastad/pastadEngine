#version 440

layout( location = 0 ) in vec3 VertexPosition;
layout( location = 1 ) in vec2 VertexCoord;
layout( location = 2 ) in vec3 VertexNormal;
layout( location = 4 ) in mat4 ModelMatrix;

out vec2 TexCoord;
out mat4 ProjMatrix;

uniform mat4 ViewMat;
uniform mat4 ProjectionMat;

void main()
{
  mat4 VP   = ProjectionMat * ViewMat ;   
  gl_Position =  VP * vec4(VertexPosition, 1.0f);

  TexCoord = VertexCoord;
}
