#version 440

layout( location = 0 ) in vec3 VertexPosition;
layout( location = 1 ) in vec2 VertexCoord;
layout( location = 2 ) in vec3 VertexNormal;
layout( location = 4 ) in mat4 ModelMatrix;

out vec3 Position;
out vec3 Normal;
out vec2 TexCoord;

uniform mat4 ViewMat;
uniform mat3 NormalMat;
uniform mat4 ProjectionMat;



void main()
{
  mat4 MVMatrix = ViewMat* ModelMatrix;
  mat4 MVP = ProjectionMat * MVMatrix ;

  Normal = normalize(VertexNormal);

  gl_Position =  MVP * vec4(VertexPosition, 1.0f);
  Position = vec3(MVMatrix * vec4(VertexPosition,1.0f)  );
  TexCoord = VertexCoord;
}
