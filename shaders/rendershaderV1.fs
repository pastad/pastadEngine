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
layout(binding=34) uniform sampler2DShadow ShadowData14; 
layout(binding=35) uniform sampler2DShadow ShadowData15; 

layout(binding=40) uniform samplerCube PointShadowData00; 
layout(binding=41) uniform samplerCube PointShadowData01; 
layout(binding=42) uniform samplerCube PointShadowData02; 
layout(binding=43) uniform samplerCube PointShadowData03; 
layout(binding=44) uniform samplerCube PointShadowData04; 
layout(binding=45) uniform samplerCube PointShadowData05; 
layout(binding=46) uniform samplerCube PointShadowData06; 
layout(binding=47) uniform samplerCube PointShadowData07; 
layout(binding=48) uniform samplerCube PointShadowData08; 
layout(binding=49) uniform samplerCube PointShadowData09; 



uniform int ColorOnly;
uniform vec4 Color;
uniform int NumDirectionalLights;
uniform int NumSpotLights;
uniform int NumPointLights;

uniform vec3 CameraPosition;
uniform int MaterialIndex;

uniform int ObjectId;

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
const int MAX_POINT_LIGHTS =  10;
const int MAX_SPOT_LIGHTS =  10;

const int MAX_POINT_SHADOWS =  10;
const int MAX_DIRECTIONAL_SHADOWS =  10;

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
    int         ShadowMapIndex;
};
struct PointLight
{
    BaseLight   Base;
    Attenuation Attenuation;
    vec3        Position;
    int         ShadowMapIndex;
};
struct SpotLight
{
    PointLight  Pointlight;
    vec3        Direction;
    float       CutoffAngle;
    int         ShadowMapIndex;
};

uniform DirectionalLight DirectionalLights[MAX_DIRECTIONAL_LIGHTS];
uniform PointLight       PointLights[MAX_POINT_LIGHTS];
uniform SpotLight        SpotLights[MAX_SPOT_LIGHTS];



// LIGHTS END --------------------------------------------------------

// SHADOWS    --------------------------------------------------------

uniform mat4 ShadowMatrices[MAX_DIRECTIONAL_SHADOWS+MAX_POINT_SHADOWS*6]; 
uniform int EnableShadows;

layout(binding=19) uniform sampler3D JitterTex;

float calcDirShadowPCF(vec3 pos,mat4 shadowMat,sampler2DShadow shadowMap)
{
  vec4 ShadowCoord = shadowMat * vec4(pos,1);  
  float sum = 1.0;

  vec2 texelSize = 1.0 / textureSize(shadowMap, 0); 

  if(ShadowCoord.z  >= 0)
  {    
    sum = 0.0;
    sum += textureProjOffset(shadowMap, ShadowCoord,ivec2(-1,-1));
    sum += textureProjOffset(shadowMap, ShadowCoord,ivec2(1,-1));
    sum += textureProjOffset(shadowMap, ShadowCoord,ivec2(-1,1));
    sum += textureProjOffset(shadowMap, ShadowCoord,ivec2(1,1));    
    sum *= 0.25; 
   // float sum2 = 0.0;
    //sum2 += textureProjOffset(shadowMap, ShadowCoord,ivec2(-2,-2)) *0.5;
    //sum2 += textureProjOffset(shadowMap, ShadowCoord,ivec2(2,-2))*0.5;
    //sum2 += textureProjOffset(shadowMap, ShadowCoord,ivec2(-2,2))*0.5;
    //sum2 += textureProjOffset(shadowMap, ShadowCoord,ivec2(2,2))*0.5;    
    //sum2 *= 0.25; 
    //sum+= sum2;

    //if(sum < 1.0)
    //  sum = 0.5;
    //if(sum > 1.0)
    //  sum = 0.5;
  }

  return sum;
}
float calcDirShadowRandomSampling(vec3 pos,mat4 shadowMat,sampler2DShadow shadowMap)
{
  float sum = 0.0;
  float ergeb = 1.0;
  float softness =  2.0 / 4096.0; // lower ->lower radius
  int sizeX = 4;
  int sizeY = 8;
  int sizeZ = 8; 
  vec4 ShadowCoord = shadowMat * vec4(pos,1);
  if(ShadowCoord.z  >= 0)
  { 
    ivec3 offset;
    offset.xy = ivec2(mod( gl_FragCoord.xy, vec2(sizeX,sizeY) ) );

    for( int i = 0 ; i < 4; i++ ) 
    {
      vec4 tShadowCoord = ShadowCoord;
      offset.z = i;
      vec4 jitteredOffset = texelFetch(JitterTex,offset,0) * softness * ShadowCoord.w;

      tShadowCoord.xy = ShadowCoord.xy + jitteredOffset.xy;
      sum += textureProj(shadowMap, tShadowCoord);
      tShadowCoord.xy = ShadowCoord.xy + jitteredOffset.zw;
      sum += textureProj(shadowMap, tShadowCoord);
    }
    ergeb= sum /(4.0 *2.0);

    if( (ergeb != 0.0) && (ergeb != 1.0) ) 
    {
      for(int i = 4; i < sizeZ; i++) 
      {    
        vec4 tShadowCoord = ShadowCoord;
        offset.z = i;
        vec4 jitteredOffset = texelFetch(JitterTex, offset,0) * softness * ShadowCoord.w;

        tShadowCoord.xy = ShadowCoord.xy + jitteredOffset.xy;
        sum += textureProj(shadowMap, tShadowCoord);
        tShadowCoord.xy = ShadowCoord.xy + jitteredOffset.zw;
        sum += textureProj(shadowMap, tShadowCoord);
      }
      ergeb = sum / float(sizeZ * 2.0);
    }
    if(ergeb < 1.0)
      ergeb = 0.5;
    if(ergeb > 1.0)
      ergeb = 0.5;
  }

  return ergeb;
}

float calcSingleDirectionalShadow(vec3 pos,mat4 shadowMat,sampler2DShadow shadowMap)
{ 
  float sum = 1.0;

  vec4 ShadowCoord = shadowMat * vec4(pos,1);
  if( EnableShadows > 0 )
  {
    if(EnableShadows == 2 )
    {
      sum = calcDirShadowPCF(pos,shadowMat, shadowMap);
    }
    if(EnableShadows == 3)
    {
      sum = calcDirShadowRandomSampling(pos,shadowMat, shadowMap);
    }
    if(EnableShadows == 1)
    { 
      if(ShadowCoord.z  >= 0)
      { 
        vec4 ShadowCoord = shadowMat * vec4(pos,1);  
        sum = textureProj(shadowMap, ShadowCoord );        
      }
    }
    
  }  
   
  return sum;
}

float calcPointShadowPCF(vec3 lightPos,vec3 fragPos,samplerCube sam)
{
  float shadow = 1.0;
  float step = 0.005; // do not go higher 
  for(float x = -step; x < step; x += step  )
  {
    for(float y = -step; y < step; y += step )
    {
      for(float z = -step; z < step; z += step )
      {
        vec3 lightDir = lightPos - (fragPos  + vec3(x,y,z)) ;
        float lightDistance = length(lightDir);
        float closestDepth = texture(sam, -lightDir).r; 
        if(lightDistance+0.001 >= closestDepth)
          shadow +=0.5f;
      }
    }
  }
  shadow /= 8;
  return 1.0 - shadow;
}

float calcSinglePointShadow(vec3 lightPos,vec3 fragPos, samplerCube sam)
{ 
  float shadow =1.0f;


  if( EnableShadows > 0 )
  {
    if(EnableShadows == 1)
    {       
      vec3 lightDir = lightPos - fragPos ;
      float lightDistance = length(lightDir);

      float closestDepth = texture(sam, -lightDir).r; 
      if(lightDistance+0.001 >= closestDepth)
        shadow =0.5f;
    }
    if(EnableShadows == 2 )
    { 
      // PCF goes here
      shadow = calcPointShadowPCF(lightPos, fragPos, sam);
    }
    if(EnableShadows == 3)
    {
      // RS goes here
    }
  }

  return shadow;
}



float calcPointShadowfactor(int idx,vec3 lightPos, vec3 fragPos)
{
  vec3 pos = vec3( texture( Tex1, TexCoord ) );
  float ret =0.0;
  if( EnableShadows > 0 )
  {
    if( idx == 0 )
      ret += calcSinglePointShadow(lightPos,fragPos,PointShadowData00);
    if( idx == 1 )
      ret += calcSinglePointShadow(lightPos,fragPos,PointShadowData01);
    if( idx == 2 )
      ret += calcSinglePointShadow(lightPos,fragPos,PointShadowData02);
    if( idx ==  3 )
      ret += calcSinglePointShadow(lightPos,fragPos,PointShadowData03);
    if( idx ==  4 )
      ret += calcSinglePointShadow(lightPos,fragPos,PointShadowData04);
    if( idx ==  5 )
      ret += calcSinglePointShadow(lightPos,fragPos,PointShadowData05);
    if( idx ==  6 )
      ret += calcSinglePointShadow(lightPos,fragPos,PointShadowData06);
    if( idx ==  7 )
      ret += calcSinglePointShadow(lightPos,fragPos,PointShadowData07);
    if( idx ==  8 )
      ret += calcSinglePointShadow(lightPos,fragPos,PointShadowData08);
    if( idx ==  9 )
      ret += calcSinglePointShadow(lightPos,fragPos,PointShadowData09);
  }
  else
    ret =1.0;
  return ret;
}  

float calcSpotShadowFactor(int idx)
{
  float ret = 0.0;
  vec3 pos = vec3( texture( Tex1, TexCoord ) );

  if( EnableShadows > 0 )
  {
    if(idx == 0 )  
      ret += calcSingleDirectionalShadow(pos, ShadowMatrices[0], ShadowData0);  
    if(idx == 1 )
      ret += calcSingleDirectionalShadow(pos, ShadowMatrices[1], ShadowData1);  
    if(idx == 2 )
      ret += calcSingleDirectionalShadow(pos, ShadowMatrices[2], ShadowData2);  
    if(idx == 3 )
      ret += calcSingleDirectionalShadow(pos, ShadowMatrices[3], ShadowData3);
    if(idx == 4 )
      ret += calcSingleDirectionalShadow(pos, ShadowMatrices[4], ShadowData4);
    if(idx == 5 )
      ret += calcSingleDirectionalShadow(pos, ShadowMatrices[5], ShadowData5);
    if(idx == 6 )
      ret += calcSingleDirectionalShadow(pos, ShadowMatrices[6], ShadowData6);
    if(idx == 7 )
      ret += calcSingleDirectionalShadow(pos, ShadowMatrices[7], ShadowData7);
    if(idx == 8 )
      ret += calcSingleDirectionalShadow(pos, ShadowMatrices[8], ShadowData8);
    if(idx == 9 )
      ret += calcSingleDirectionalShadow(pos, ShadowMatrices[9], ShadowData9);
    if(idx == 10 )
      ret += calcSingleDirectionalShadow(pos, ShadowMatrices[10], ShadowData10);
    if(idx == 11 )
      ret += calcSingleDirectionalShadow(pos, ShadowMatrices[11], ShadowData11);
    if(idx == 12 )
      ret += calcSingleDirectionalShadow(pos, ShadowMatrices[12], ShadowData12);
    if(idx == 13 )
      ret += calcSingleDirectionalShadow(pos, ShadowMatrices[13], ShadowData13);
    if(idx == 14 )
      ret += calcSingleDirectionalShadow(pos, ShadowMatrices[14], ShadowData14);
    if(idx == 15 )
      ret += calcSingleDirectionalShadow(pos, ShadowMatrices[15], ShadowData15);    
  }
  else
    ret =1.0;

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

 
  float shadowFactor = 1.0f;
  //if(length( vec2(pos.x,pos.z) - vec2(CameraPosition.x,CameraPosition.z )) <10 )
   shadowFactor = calcSpotShadowFactor(l.ShadowMapIndex);
  return vec4(diffuse* shadowFactor * l.Base.Intensity + ambient* l.Base.Intensity + specular* l.Base.Intensity,1) ;
}

// pointLight calculation
vec4 calcPointLights(int idx, Material mat, vec3 pos, vec3 normal)
{  
  PointLight l = PointLights[idx];
  vec3 norm = normalize(normal);
  vec3 lightDirection = l.Position - pos.xyz;
  vec3 lightDir = normalize(lightDirection);
  float distance   = length(l.Position - pos.xyz);

  float diff = max(dot(norm, lightDir), 0.0);
  vec3 diffuse = diff * l.Base.DiffuseColor * mat.DiffuseColor;

  vec3 ambient = l.Base.AmbientColor * mat.AmbientColor;

  vec3 specular = calcSpecularColor(lightDir, l.Base.SpecularColor, mat,pos,normal);

  float attenuation = 1.0f / (l.Attenuation.Constant + l.Attenuation.Linear * distance + l.Attenuation.Quadratic * (distance * distance));

  return vec4( (diffuse* l.Base.Intensity * calcPointShadowfactor(l.ShadowMapIndex,l.Position, pos.xyz) + ambient* 1* l.Base.Intensity + specular* l.Base.Intensity ) * attenuation,1) ;
}

// spotLight calculationds
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

    color =vec4( (diffuse* l.Pointlight.Base.Intensity *calcSpotShadowFactor(l.ShadowMapIndex) * spotFactor + ambient* l.Pointlight.Base.Intensity + specular* l.Pointlight.Base.Intensity  * spotFactor ) * attenuation,1) ;
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
  MaterialData = vec3(MaterialIndex,ObjectId,0);
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
  int matIdx = int(materialIndex.x);

  vec4 color = vec4(diffColor,1.0);
  vec4 light = vec4(0,0,0,0);
 
  // prevent flicker because of not set texture
  if( (norm.x==0) && (norm.y==0) && (norm.z==0) )
   FragColor = vec4(0,0,0,0);
  else
  {
    Material mat = Materials[matIdx];

    for(int x=0; x< NumDirectionalLights; x++ )
      light += calcDirectionalLight(x,mat,pos,norm);
    for(int x=0; x< NumPointLights; x++ )
      light += calcPointLights(x  ,mat,pos,norm);
    for(int x=0; x< NumSpotLights; x++ )
      light += calcSpotLight(x,mat,pos,norm);

    FragColor = color * light ;
    //FragColor = vec4(0,shadow,0,1.0);
 }
 if(matIdx ==99999)
  FragColor = color;  
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