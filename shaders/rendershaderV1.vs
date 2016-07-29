#version 430

layout( location = 0 ) in vec3  VertexPosition;
layout( location = 1 ) in vec2  VertexCoord;
layout( location = 2 ) in vec3  VertexNormal;
layout( location = 3 ) in mat4  ModelMatrix;
layout( location = 4 ) in ivec4 BoneIDs;
layout( location = 5 ) in vec4  Weights;

out vec3 Position;
out vec3 Normal;
out vec2 TexCoord;
out vec4 ModelPos;
out vec4 ShadowCoord;

// mats that are allways set
uniform mat4 ViewMat;
uniform mat4 ProjectionMat;

// specific mats

uniform mat4 SingleModelMatrix;
uniform int Instanced;

uniform mat4 ShadowMat; 

uniform int ScreenSpaceRender;

uniform int Animation; 
uniform mat4 Bones[100];

void main()
{
  if(ScreenSpaceRender == 0)  // check if we should render normaly or to our screen quad
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
    Position = vec3(ModelMat * BTrans* VertexPos  );
    ModelPos = ModelMat* BTrans * VertexPos;
    gl_Position =  MVP * BTrans * VertexPos;

    mat3 NormalMatrix = mat3( ModelMat* BTrans);
    Normal = normalize( NormalMatrix * VertexNormal);
  }
  else
  {
    mat4 VP   = ProjectionMat * ViewMat ;
    Position = vec3(VP * vec4(VertexPosition,1.0f)  );    
    gl_Position =  VP * vec4(VertexPosition, 1.0f);
  }

  ShadowCoord = ShadowMat *  vec4(VertexPosition,1.0); 
  TexCoord = VertexCoord;
}
