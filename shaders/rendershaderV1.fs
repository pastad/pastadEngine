#version 440

subroutine void RenderPassType();
subroutine uniform RenderPassType RenderPass;

in vec3 Position;
in vec3 Normal;
in vec2 TexCoord;

struct MaterialS
{
  vec3 m_diffuse_color;
  vec3 m_ambient_color;
  vec3 m_specular_color;
  float m_shininess;
  float m_opacity;
};
uniform MaterialS Material;

layout(location = 0 ) out vec4 FragColor;
layout(location = 1 ) out vec3 PositionData;
layout(location = 2 ) out vec3 NormalData;
layout(location = 3 ) out vec3 ColorData;

layout(binding=0) uniform sampler2D PositionTex;
layout(binding=1) uniform sampler2D NormalTex;
layout(binding=2) uniform sampler2D ColorTex;

uniform int ColorOnly;
uniform vec4 Color;

subroutine (RenderPassType)
void pass1()
{
  if(ColorOnly == 1)
    FragColor = vec4(Material.m_diffuse_color,1.0);
  else
    FragColor = vec4(1,1,1,1.0);
}
subroutine (RenderPassType)
void pass2()
{
  FragColor = vec4(Position,1.0);
}

void main()
{
  RenderPass();  
}