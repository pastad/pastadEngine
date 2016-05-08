#version 440

layout( location = 0 ) in vec3 VertexPosition;
layout( location = 1 ) in vec2 VertexCoord;
layout( location = 2 ) in vec3 VertexNormal;
layout( location = 3 ) in mat4 ModelMatrix;
layout( location = 4 ) in ivec4 BoneIDs;
layout( location = 5 ) in vec4  Weights;


uniform mat4 ViewMat;
uniform mat3 NormalMat;
uniform mat4 ProjectionMat;

uniform mat4 SingleModelMatrix;
uniform int Instanced;

uniform int Animation; 
uniform mat4 Bones[100];

void main()
{
  mat4 ModelMat = SingleModelMatrix ;    
  vec4 VertexPos = vec4(VertexPosition,1.0f);
  mat4 BTrans = mat4(1.0);

  if( Instanced == 1 )      // check if we render instanced or not
  {
    ModelMat = ModelMatrix;
  }
  if( Animation == 1 )
  {
    mat4 BoneTransform = Bones[BoneIDs[0]] * Weights[0];
    BoneTransform     += Bones[BoneIDs[1]] * Weights[1];
    BoneTransform     += Bones[BoneIDs[2]] * Weights[2];
    BoneTransform     += Bones[BoneIDs[3]] * Weights[3];
    BTrans =BoneTransform;
  }
    

  mat4 MVMatrix = ViewMat* ModelMat;
  mat4 MVP      = ProjectionMat * MVMatrix ;  
  vec4 Position =  MVP * BTrans* vec4(VertexPosition, 1.0f);
  gl_Position =  Position;
 
}
