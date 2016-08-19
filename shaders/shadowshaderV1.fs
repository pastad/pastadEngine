#version 440


layout (location = 0) out float FragColor;

in vec4 FragPos;

void main()
{ 
  //= 1.0 /gl_FragCoord.z ;
  FragColor = gl_FragCoord.z; 
}
