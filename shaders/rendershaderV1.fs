#version 440


subroutine void RenderPassType();
subroutine uniform RenderPassType RenderPass;

in vec3 Position;
in vec3 Normal;
in vec2 TexCoord;
in vec4 ModelPos;

layout(location = 0 ) out vec4 FragColor;
layout(location = 1 ) out vec3 PositionData;
layout(location = 2 ) out vec3 NormalData;
layout(location = 3 ) out vec3 ColorData;
layout(location = 4 ) out vec3 MaterialData;

layout(binding=0) uniform sampler2D Tex1; 
layout(binding=1) uniform sampler2D Tex2; 
layout(binding=2) uniform sampler2D Tex3; 
layout(binding=3) uniform sampler2D Tex4; 
layout(binding=4) uniform sampler2D Tex5; 

layout(binding=20) uniform sampler2DShadow ShadowData0; 
layout(binding=21) uniform sampler2DShadow ShadowData1; 
layout(binding=22) uniform sampler2DShadow ShadowData2; 
layout(binding=23) uniform sampler2DShadow ShadowData3; 
layout(binding=24) uniform sampler2DShadow ShadowData4; 
layout(binding=25) uniform sampler2DShadow ShadowData5; 
layout(binding=26) uniform sampler2DShadow ShadowData6; 
layout(binding=27) uniform sampler2DShadow ShadowData7; 
layout(binding=28) uniform sampler2DShadow ShadowData8; 
layout(binding=29) uniform sampler2DShadow ShadowData9; 
layout(binding=30) uniform sampler2DShadow ShadowData10; 
layout(binding=31) uniform sampler2DShadow ShadowData11; 
layout(binding=32) uniform sampler2DShadow ShadowData12; 
layout(binding=33) uniform sampler2DShadow ShadowData13; 



uniform int ColorOnly;
uniform vec4 Color;
uniform int NumDirectionalLights;
uniform int NumSpotLights;
uniform int NumPointLights;

uniform vec3 CameraPosition;
uniform int MaterialIndex;

// MATERIAL -----------------------------------------------------------

const int MAX_NUM_MATERIALS =  20;

struct Material
{
  vec3  DiffuseColor;
  vec3  AmbientColor;
  vec3  SpecularColor;
  float Shininess;
  float Opacity;
};
uniform Material Materials[MAX_NUM_MATERIALS];

// MATERIAL END  ------------------------------------------------------

//  LIGHTS ------------------------------------------------------------

const int MAX_DIRECTIONAL_LIGHTS =  3;
const int MAX_POINT_LIGHTS =  20;
const int MAX_SPOT_LIGHTS =  10;

struct BaseLight
{
    vec3  AmbientColor;
    vec3  DiffuseColor;
    vec3  SpecularColor;
    float Intensity;
};
struct Attenuation
{
  float Constant;
  float Linear;
  float Quadratic;
};

struct DirectionalLight
{
    BaseLight Base;
    vec3      Direction;
};
struct PointLight
{
    BaseLight   Base;
    Attenuation Attenuation;
    vec3        Position;
};
struct SpotLight
{
    PointLight  Pointlight;
    vec3        Direction;
    float       CutoffAngle;
};

uniform DirectionalLight DirectionalLights[MAX_DIRECTIONAL_LIGHTS];
uniform PointLight       PointLights[MAX_POINT_LIGHTS];
uniform SpotLight        SpotLights[MAX_SPOT_LIGHTS];



// LIGHTS END --------------------------------------------------------

// SHADOWs    --------------------------------------------------------

uniform mat4 DirectionalShadowMat[MAX_SPOT_LIGHTS+MAX_DIRECTIONAL_LIGHTS]; 
uniform int DirectionalShadowMatCount;
uniform int EnablePCF;
uniform int EnableStandardShadows;

float calcSingleDirectionalShadow(vec3 pos,mat4 shadowMat,sampler2DShadow shadowMap)
{ 
  float sum = 0;
  if( EnableStandardShadows )
  {
    vec4 ShadowCoord = shadowMat * vec4(pos,1);

    
    if(ShadowCoord.z  < 0)
      sum = 1;
    else
    {
      if(EnablePCF == 1)
      {
        sum += textureProjOffset(shadowMap, ShadowCoord,ivec2(-1,-1));
        sum += textureProjOffset(shadowMap, ShadowCoord,ivec2(1,-1));
        sum += textureProjOffset(shadowMap, ShadowCoord,ivec2(-1,1));
        sum += textureProjOffset(shadowMap, ShadowCoord,ivec2(1,1));
        sum *= 0.25; 
      }
      else
        sum = textureProj(shadowMap, ShadowCoord);
    }
  }
  else
    sum = 1;
   
  return sum;
}


float calcShadowFactor()
{
  float ret = 0.0;
  vec3 pos = vec3( texture( Tex1, TexCoord ) );

  if(DirectionalShadowMatCount > 0 )  
    ret += calcSingleDirectionalShadow(pos, DirectionalShadowMat[0], ShadowData0);
  
  if(DirectionalShadowMatCount > 1 )
    ret += calcSingleDirectionalShadow(pos, DirectionalShadowMat[1], ShadowData1);
  
  if(DirectionalShadowMatCount > 2 )
    ret += calcSingleDirectionalShadow(pos, DirectionalShadowMat[2], ShadowData2);
  
  if(DirectionalShadowMatCount > 3 )
    ret += calcSingleDirectionalShadow(pos, DirectionalShadowMat[3], ShadowData3);

  if(DirectionalShadowMatCount > 4 )
    ret += calcSingleDirectionalShadow(pos, DirectionalShadowMat[4], ShadowData4);

  if(DirectionalShadowMatCount > 5 )
    ret += calcSingleDirectionalShadow(pos, DirectionalShadowMat[5], ShadowData5);

  if(DirectionalShadowMatCount > 6 )
    ret += calcSingleDirectionalShadow(pos, DirectionalShadowMat[6], ShadowData6);

  if(DirectionalShadowMatCount > 7 )
    ret += calcSingleDirectionalShadow(pos, DirectionalShadowMat[7], ShadowData7);

  if(DirectionalShadowMatCount > 8 )
    ret += calcSingleDirectionalShadow(pos, DirectionalShadowMat[8], ShadowData8);

  if(DirectionalShadowMatCount > 9 )
    ret += calcSingleDirectionalShadow(pos, DirectionalShadowMat[9], ShadowData9);

  if(DirectionalShadowMatCount > 10 )
    ret += calcSingleDirectionalShadow(pos, DirectionalShadowMat[10], ShadowData10);

  if(DirectionalShadowMatCount > 11 )
    ret += calcSingleDirectionalShadow(pos, DirectionalShadowMat[11], ShadowData11);

  if(DirectionalShadowMatCount > 12 )
    ret += calcSingleDirectionalShadow(pos, DirectionalShadowMat[12], ShadowData12);

  
  return ret;
}

// SHADOWS END  ------------------------------------------------------

// LIGHT CALCULATION -------------------------------------------------

// calculates the specular part
vec3 calcSpecularColor(vec3 lightDirection, vec3 specularColor, Material mat, vec3 pos, vec3 normal)
{
  vec3 norm =       normalize(normal);
  vec3 viewDir =    normalize(CameraPosition - pos.xyz);
  vec3 reflectDir = reflect(-lightDirection, norm);
  float spec =      pow(max(dot(viewDir, reflectDir), 0.0), mat.Shininess);

  vec3 specular =   specularColor * spec * mat.SpecularColor;
  return specular;
}

// directionLight calculation
vec4 calcDirectionalLight(int idx, Material mat, vec3 pos, vec3 normal)
{
  DirectionalLight l = DirectionalLights[idx];
  vec3  norm     = normalize(normal);
  vec3  lightDir = normalize(-l.Direction);
  float diff     = max(dot(norm, lightDir), 0.0);
  vec3 diffuse = diff * l.Base.DiffuseColor;

  vec3 ambient   = l.Base.AmbientColor * mat.AmbientColor;
  
  vec3  specular   = calcSpecularColor(lightDir, l.Base.SpecularColor, mat,pos,normal);

  return vec4(diffuse* l.Base.Intensity *calcShadowFactor()+ ambient* l.Base.Intensity + specular* l.Base.Intensity,1) ;
}

// pointLight calculation
vec4 calcPointLights(int idx, Material mat, vec3 pos, vec3 normal)
{  
  PointLight l = PointLights[idx];
  vec3 norm = normalize(normal);
  vec3 lightDir = normalize(l.Position - pos.xyz);
  float distance   = length(l.Position - pos.xyz);

  float diff = max(dot(norm, lightDir), 0.0);
  vec3 diffuse = diff * l.Base.DiffuseColor * mat.DiffuseColor;

  vec3 ambient = l.Base.AmbientColor * mat.AmbientColor;

  vec3 specular = calcSpecularColor(lightDir, l.Base.SpecularColor, mat,pos,normal);

  float attenuation = 1.0f / (l.Attenuation.Constant + l.Attenuation.Linear * distance + l.Attenuation.Quadratic * (distance * distance));

  return vec4( (diffuse* l.Base.Intensity *calcShadowFactor() + ambient* l.Base.Intensity + specular* l.Base.Intensity ) * attenuation,1) ;
}

// spotLight calculation
vec4 calcSpotLight(int idx, Material mat, vec3 pos, vec3 normal)
{
  SpotLight l = SpotLights[idx];
  vec3 lightToPixel = normalize( l.Pointlight.Position - pos.xyz);
  float angle = acos(dot(-lightToPixel,l.Direction));
  float cutoff = radians (clamp (l.CutoffAngle, 0.0 ,90.0));
  float spotFactor =pow (dot( -lightToPixel, l.Direction),0.1);

  float distance   = length(l.Pointlight.Position - pos.xyz);

  vec3 ambient = l.Pointlight.Base.AmbientColor * mat.AmbientColor;
  float attenuation = 1.0f / (l.Pointlight.Attenuation.Constant + l.Pointlight.Attenuation.Linear * distance + l.Pointlight.Attenuation.Quadratic * (distance * distance));

  if (angle < cutoff)
  {
    vec4 color;
    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(l.Pointlight.Position - pos.xyz);   

    float diff = max(dot(lightToPixel, norm), 0.0);
    vec3 diffuse = diff * l.Pointlight.Base.DiffuseColor * mat.DiffuseColor ;

    vec3 v = normalize( vec3(-pos.xyz));
    vec3 h = normalize( v +lightToPixel );
    float spec = pow(max(dot(h, norm), 0.0), mat.Shininess);

    vec3 specular =    l.Pointlight.Base.SpecularColor * spec * mat.SpecularColor;

    color =vec4( (diffuse* l.Pointlight.Base.Intensity *calcShadowFactor() * spotFactor + ambient* l.Pointlight.Base.Intensity + specular* l.Pointlight.Base.Intensity  * spotFactor ) * attenuation,1) ;
    return color;
  }
  else
  {
    return vec4(ambient,1.0) *l.Pointlight.Base.Intensity * attenuation;
  }
}
// LIGHT CALCULATION END ---------------------------------------------

// PASS 1     --------------------------------------------------------

subroutine (RenderPassType)
void pass1()
{
  vec4 color;
  vec4 light = vec4(0,0,0,0);
  Material mat = Materials[MaterialIndex]; 

  if(ColorOnly == 1)
    color = vec4(mat.DiffuseColor,1.0);
  else
    color = texture(Tex1, TexCoord);

  PositionData = Position;
  NormalData = normalize(Normal);
  ColorData = vec3(color);
  MaterialData = vec3(MaterialIndex,0,0);
}
// PASS 1 END    -----------------------------------------------------


// PASS 2     --------------------------------------------------------
subroutine (RenderPassType)
void pass2()
{
  vec3 pos = vec3( texture( Tex1, TexCoord ) );
  vec3 norm = vec3( texture( Tex2, TexCoord ) );
  vec3 diffColor = vec3( texture(Tex3, TexCoord) );
  vec3 materialIndex = vec3( texture(Tex4, TexCoord) );

  vec4 color = vec4(diffColor,1.0);
  vec4 light = vec4(0,0,0,0);

  // prevent flicker because of not set texture
  if( (norm.x==0) && (norm.y==0) && (norm.z==0) )
    FragColor = vec4(0,0,0,0);
  else
  {
    Material mat = Materials[int(materialIndex.x)];

    //for(int x=0; x< NumDirectionalLights; x++ )
   //   light += calcDirectionalLight(x,mat,pos,norm);
   // for(int x=0; x< NumPointLights; x++ )
   //   light += calcPointLights(x,mat,pos,norm);
    for(int x=0; x< NumSpotLights; x++ )
      light += calcSpotLight(x,mat,pos,norm);

    FragColor = color * light ;
    //FragColor = vec4(0,shadow,0,1.0);
  }
}
// PASS 2 END    -----------------------------------------------------


// PASS LIGHT   ------------------------------------------------------
subroutine (RenderPassType)
void pass3()
{
  //just the z buffer
}
// PASS LIGHT END  ---------------------------------------------------

// MAIN    -----------------------------------------------------------

void main()
{
  RenderPass();  
}

// MAIN END ----------------------------------------------------------