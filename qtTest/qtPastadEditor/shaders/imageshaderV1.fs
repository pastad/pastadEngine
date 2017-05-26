#version 440

in vec2 TexCoord;

layout(location = 0 ) out vec4 FragColor;
layout(location = 1 ) out vec3 PositionData;

layout(binding=0) uniform sampler2D Tex1; 

uniform vec3 MixColor;
uniform float MixValue;

void main()
{
  //FragColor = vec4( vec3( texture(Tex1, vec2(TexCoord.x, 1.0 - TexCoord.y ) )),1);
  FragColor =  mix( texture2D(Tex1, vec2(TexCoord.x, 1-  TexCoord.y ) ) , vec4(MixColor,1.0) , MixValue)  ;

}

