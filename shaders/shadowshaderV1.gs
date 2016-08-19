#version 430 core
layout (triangles) in;
layout (triangle_strip, max_vertices=18) out;

uniform mat4 ViewMatrices[6];


flat in int LightT;


out vec4 FragPos;
out int LightTy;

void main()
{
  LightTy = LightT;
  if(LightT == 0)
  {
    for(int i=0; i<3; i++)
    {
      gl_Position = gl_in[i].gl_Position;
      EmitVertex();
    }
    EndPrimitive();
  }
  else
  {
    for(int face = 0; face < 6; ++face)
    {
      gl_Layer = face; // built-in variable that specifies to which face we render.
      for(int i = 0; i < 3; ++i) // for each triangle's vertices
      {
          FragPos = gl_in[i].gl_Position;
          gl_Position = ViewMatrices[face] * FragPos;
          EmitVertex();
      }    
      EndPrimitive();
    }
  }
}  