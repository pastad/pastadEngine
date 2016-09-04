#version 440

in vec3 Position;
in vec3 Normal;
in vec2 TexCoord;
in vec4 ModelPos;

layout(location = 0 ) out vec4 FragColor;
layout(location = 1 ) out vec3 PositionData;
layout(location = 2 ) out vec3 NormalData;
layout(location = 3 ) out vec3 ColorData;
layout(location = 4 ) out vec3 MaterialData;

uniform int MaterialIndex;


// MAIN    -----------------------------------------------------------

void main()
{
  PositionData = Position;
  NormalData = normalize(Normal);
  ColorData = vec3(0,1,1);
  MaterialData = vec3(1,0,0);
  FragColor = vec4(0,1,1,1);
}

// MAIN END ----------------------------------------------------------