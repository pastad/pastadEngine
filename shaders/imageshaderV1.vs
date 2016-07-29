#version 440

layout( location = 0 ) in vec3 VertexPosition;
layout( location = 1 ) in vec2 VertexCoord;


out vec2 TexCoord;

uniform vec2 ModelPosition;
uniform vec2 ScreenSize;
uniform vec2 ModelSize;


void main()
{
  vec2 corrSize = ScreenSize / 2.0f;
  vec2 halfModel= ModelSize / 2.0f;
  vec2 scale = 1.0 / corrSize; 
  vec2 vpos =  VertexPosition.xy * scale;
  vpos *=  halfModel; 
  vec2 pos_scale = ModelPosition * scale;
  pos_scale += vec2(-1,-1);
  vpos += pos_scale;
  vpos += (halfModel *scale);
  gl_Position = vec4(vpos,0, 1.0f);
  TexCoord = VertexCoord;
}
