#version 440

#define M_PI 3.1415926535897932384626433832795
#define MAX_WAVES 5

layout( location = 0 ) in vec3  VertexPosition;
layout( location = 1 ) in vec2  VertexCoord;
layout( location = 2 ) in vec3  VertexNormal;
layout( location = 3 ) in mat4  ModelMatrix;

out vec3 Position;
out vec3 Normal;
out vec2 TexCoord;
out vec4 ModelPos;

// mats that are allways set
uniform mat4 ViewMat;
uniform mat4 ProjectionMat;

// specific mats

uniform mat4 SingleModelMatrix;
uniform int Instanced;

uniform float Delta;
uniform float Step;

struct Wave
{
  vec2 Direction;
  float Amplitude;
  float Frequency;
  float PhaseConstant;
};

uniform Wave Waves[MAX_WAVES];
uniform int WaveAmount;

// equation from gpugems http://http.developer.nvidia.com/GPUGems/gpugems_ch01.html
float waveEquation(Wave wave, float x, float y)
{
  //float l = 1; 
  //float s = 0.001;
  //float amplitude = 0.2; 
  //float frequency = (2 *M_PI )/l; // w = 2p/L
  //float phaseConstant = s * frequency ; // S x 2p/L

  float height  = wave.Amplitude * sin( dot(wave.Direction, vec2(x,y) ) * wave.Frequency + Delta * wave.PhaseConstant    );

  return height;
}

float waveEquationNormalX(Wave wave, float x, float y)
{
  float ret =  wave.Frequency * wave.Direction.x * wave.Amplitude * cos(  dot( wave.Direction, vec2(x,y) ) * wave.Frequency + Delta * wave.PhaseConstant  ) ;
  
  return -ret;
}

float waveEquationNormalY(Wave wave, float x, float y)
{
  float ret =  wave.Frequency * wave.Direction.y * wave.Amplitude * cos(  dot( wave.Direction, vec2(x,y) ) * wave.Frequency + Delta * wave.PhaseConstant  ) ;
  
  return -ret;
}

vec3 generationFunctionNormal(float x, float y)
{
  vec3 ret =  vec3(0,1,0);

  for(int i=0; i< WaveAmount;i++)
  {
    ret.x+= waveEquationNormalX(Waves[i],x,y);
    ret.z+= waveEquationNormalY(Waves[i],x,y);
    ret.y+=1.0;
  }

  return ret;
}

float generationFunction(float x, float y)
{  
  float ret = 0.0f;

  for(int i=0; i< WaveAmount;i++)
  {  
    ret+= waveEquation(Waves[i],x,y);
  }
  return ret;//waveEquation(vec2(0.5,0.5), x,y)+waveEquation(vec2(-0.5,0.5), x,y);
  //return sin(x*Delta/1000.0*6)/10.0+cos(y*Delta/1000.0*6/2)/10.0;
}


void main()
{ 
  mat4 ModelMat = SingleModelMatrix ;    
  vec4 VertexPos = vec4(VertexPosition,1.0f);
  VertexPos.y += generationFunction(VertexPos.x,VertexPos.z);


//  vec3 n2 = vec3(VertexPosition.x+Step, VertexPosition.y+generationFunction (VertexPosition.x+Step,VertexPosition.z),VertexPosition.z );
//  vec3 n3 = vec3(VertexPos.x, VertexPosition.y+generationFunction (VertexPosition.x,VertexPosition.z+Step),VertexPos.z+Step );

 // vec3 d1 = n2 - VertexPosition;
 // vec3 d2 = n3 - VertexPosition;
 // vec3 norm = cross(d2,d1);

  mat4 MVMatrix = ViewMat* ModelMat;
  mat4 MVP      = ProjectionMat * MVMatrix ;    
  Position = vec3(ModelMat *  VertexPos  );
  ModelPos = ModelMat* VertexPos;
  gl_Position =  MVP *  VertexPos;

  mat3 NormalMatrix = mat3( ModelMat);
  Normal = normalize( generationFunctionNormal(VertexPos.x,VertexPos.z));  
  
  TexCoord = VertexCoord;
}
