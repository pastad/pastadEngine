#version 440

layout( location = 0 ) in vec3 VertexPosition;
layout( location = 1 ) in vec2 VertexCoord;
layout( location = 2 ) in vec3 VertexNormal;
layout( location = 4 ) in mat4 ModelMatrix;

out vec3 Position;
out vec3 Normal;
out vec2 TexCoord;
out vec4 ModelPos;
out vec4 ShadowCoord;

uniform mat4 ViewMat;
uniform mat3 NormalMat;
uniform mat4 ProjectionMat;

uniform mat4 ShadowMat; //TEST

uniform int RenderThrew;

void main()
{  
  mat4 VP      = ProjectionMat * ViewMat ;    
  Position = VertexPosition;
  gl_Position = (VP * vec4(VertexPosition,1.0f) ).xyww;
  
  ShadowCoord = ShadowMat * vec4(VertexPosition,1.0); // test maybe model missing
  Normal   = normalize(VertexNormal);
  TexCoord = VertexCoord;
}
