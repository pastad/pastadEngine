#version 330 core
in vec2 texCoords;
out vec4 color;

uniform sampler2D Text;
uniform vec3 TextColor;

void main()
{    
    vec4 sampled = vec4(1.0, 1.0, 1.0, texture(Text, texCoords).r);
    color = vec4(TextColor, 1.0) * sampled;
}  
