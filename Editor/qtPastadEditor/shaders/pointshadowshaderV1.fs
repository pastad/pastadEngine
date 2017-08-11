#version 440


layout (location = 0) out float FragColor;

in vec3 Pos;

uniform vec3 LightPosition;

void main()
{ 
  float distance = length(Pos-LightPosition) ;
  FragColor = distance ;
}
