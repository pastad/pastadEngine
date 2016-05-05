#version 440


subroutine void RenderPassType();
subroutine uniform RenderPassType RenderPass;

in vec3 Position;
in vec3 Normal;
in vec2 TexCoord;
in vec4 ModelPos;

layout(location = 0 ) out vec4 FragColor;
layout(location = 1 ) out vec3 PositionData;
layout(location = 2 ) out vec3 NormalData;
layout(location = 3 ) out vec3 ColorData;
layout(location = 4 ) out vec3 MaterialData;



layout(binding=0) uniform samplerCube cubeTexture;


uniform int MaterialIndex;

// MAIN    -----------------------------------------------------------

void main()
{
  vec4 color;
 
  color = texture(cubeTexture, Position);

  PositionData = Position;
  NormalData = vec3(99,99,99);
  ColorData = color.xyz;
  MaterialData = vec3(99999,0,0); 
  FragColor = vec4(1,0,0,0);
}

// MAIN END ----------------------------------------------------------