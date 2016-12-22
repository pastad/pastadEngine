// shadertype=glsl
#version 430


subroutine void RenderPassType();
subroutine uniform RenderPassType RenderPass;

in vec3 Position;
in vec3 Normal;
in vec2 TexCoord;
in vec4 ModelPos;

layout(location = 0 ) out vec4 FragColor;
layout(location = 1 ) out vec3 PositionData;
layout(location = 2 ) out vec3 NormalData;
layout(location = 3 ) out vec4 ColorData;
layout(location = 4 ) out vec3 MaterialData;

layout(binding=0) uniform sampler2D Tex1; 
layout(binding=1) uniform sampler2D Tex2; 
layout(binding=2) uniform sampler2D Tex3; 
layout(binding=3) uniform sampler2D Tex4; 
layout(binding=4) uniform sampler2D Tex5; 
layout(binding=5) uniform sampler2D Tex6; 
layout(binding=6) uniform sampler2D Tex7; 
layout(binding=7) uniform sampler2D Tex8; 
layout(binding=8) uniform sampler2D Tex9; 
layout(binding=9) uniform sampler2D Tex10; 
layout(binding=10) uniform sampler2D Tex11; 

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

layout(binding=40) uniform samplerCube PointShadowData00; 
layout(binding=41) uniform samplerCube PointShadowData01; 
layout(binding=42) uniform samplerCube PointShadowData02; 
layout(binding=43) uniform samplerCube PointShadowData03; 
layout(binding=44) uniform samplerCube PointShadowData04; 
layout(binding=45) uniform samplerCube PointShadowData05; 
layout(binding=46) uniform samplerCube PointShadowData06; 
layout(binding=47) uniform samplerCube PointShadowData07; 

uniform vec3 FogColor;
uniform float FogFactor;
uniform float FogOffset;

uniform int ColorOnly;
uniform vec4 Color;
uniform int NumDirectionalLights;
uniform int NumSpotLights;
uniform int NumPointLights;
uniform int NormalMapActive;
uniform int OpacityMapActive;

uniform vec3 CameraPosition;
uniform int MaterialIndex;

uniform int ObjectId;

uniform vec2 GBufferSize;

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
const int MAX_POINT_LIGHTS =  8;
const int MAX_SPOT_LIGHTS =  10;

const int MAX_POINT_SHADOWS =  10;
const int MAX_DIRECTIONAL_SHADOWS =  10;



struct BaseLight
{
    vec3  AmbientColor;
    vec3  DiffuseColor;
    vec3  SpecularColor;
    float Intensity;
  //  float Emmissive;
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

float LinearizeDepth(float depth)
{
  float far_plane_dist = 500.0f;
  float near_plane_dist = 0.1f;
  float z = depth *2.0 -1.0; 
  return (2.0 * near_plane_dist * far_plane_dist) / (far_plane_dist + near_plane_dist - z * (far_plane_dist - near_plane_dist));  
}

// SHADOWS    --------------------------------------------------------

uniform mat4 ShadowMatrices[MAX_DIRECTIONAL_SHADOWS+MAX_POINT_SHADOWS*6]; 
uniform int EnableShadowsDirectional;
uniform int EnableShadowsPoint;
uniform int EnableSSAO;

layout(binding=19) uniform sampler3D JitterTex;


float calcDirShadowPCF(vec3 pos,mat4 shadowMat,sampler2DShadow shadowMap, float bias, bool spot)
{
  vec4 ShadowCoord = shadowMat * vec4(pos,1);  
  float sum = 1.0;

  vec2 texelSize = 1.0 / textureSize(shadowMap, 0); 


  if(ShadowCoord.z  >= 0)
  {    
    float offset = 1;
    //float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.015);
    sum = 0.0;
    float ts = textureProjOffset(shadowMap, ShadowCoord,ivec2(1,0));   
    vec4 tShadowCoord = ShadowCoord;

    tShadowCoord.xy +=   vec2(-offset, -offset) * texelSize;
    ts = textureProj(shadowMap, tShadowCoord );
    if(spot)
      ts = LinearizeDepth(ts);
    if(! (tShadowCoord.z-bias  > ts) )
        sum += 1;    

    tShadowCoord = ShadowCoord;
    tShadowCoord.xy +=  vec2(offset, -offset) * texelSize;
    ts = textureProj(shadowMap, tShadowCoord );
    if(spot)
      ts = LinearizeDepth(ts);
    if(spot)
      ts = LinearizeDepth(ts);
    if(! (tShadowCoord.z+bias  > ts) )
       sum += 1;    

    tShadowCoord = ShadowCoord;
    tShadowCoord.xy +=  vec2(-offset, offset) * texelSize;
    ts = textureProj(shadowMap, tShadowCoord );
    if(spot)
      ts = LinearizeDepth(ts);
    if(! (tShadowCoord.z+bias  > ts) )
        sum += 1;   

    tShadowCoord = ShadowCoord;
    tShadowCoord.xy +=  vec2(offset, offset) * texelSize;
    ts = textureProj(shadowMap, tShadowCoord );
    if(spot)
      ts = LinearizeDepth(ts);
    if(! (tShadowCoord.z+bias  > ts) )
        sum += 1; 

    tShadowCoord = ShadowCoord;
    tShadowCoord.xy +=  vec2(0, 0) * texelSize;
    ts = textureProj(shadowMap, tShadowCoord );
    if(spot)
      ts = LinearizeDepth(ts);
    if(! (tShadowCoord.z+bias  > ts) )
        sum += 1;      

    sum *= 0.20; 
  }

  return sum;
}
float calcDirShadowRandomSampling(vec3 pos,mat4 shadowMat,sampler2DShadow shadowMap, float bias, bool spot)
{
  float sum = 0.0;
  float ergeb = 1.0;
  float softness =  1.0 / 8096.0; // lower ->lower radius
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
      float ts =  textureProj(shadowMap, tShadowCoord);
      if(spot)
        ts = LinearizeDepth(ts);
      if(! (tShadowCoord.z+bias  > ts) )
        sum+=1;
      tShadowCoord.xy = ShadowCoord.xy + jitteredOffset.zw;
      ts =  textureProj(shadowMap, tShadowCoord);
      if(spot)
        ts = LinearizeDepth(ts);
      if(! (tShadowCoord.z+bias  > ts) )
        sum+=1;
    }
    ergeb= sum /(4.0 *2.0);
 
    for(int i = 4; i < sizeZ; i++) 
    {    
      vec4 tShadowCoord = ShadowCoord;
      offset.z = i;
      vec4 jitteredOffset = texelFetch(JitterTex, offset,0) * softness * ShadowCoord.w;

      tShadowCoord.xy = ShadowCoord.xy + jitteredOffset.xy;       
      float ts =  textureProj(shadowMap, tShadowCoord);
      if(spot)
        ts = LinearizeDepth(ts);
      if(! (tShadowCoord.z+bias  > ts) )
        sum+=1;
      tShadowCoord.xy = ShadowCoord.xy + jitteredOffset.zw;
      ts =  textureProj(shadowMap, tShadowCoord);
      if(spot)
        ts = LinearizeDepth(ts);
      if(! (tShadowCoord.z+bias  > ts) )
        sum+=1;
 
      ergeb = sum / float(sizeZ * 2.0);
    }
  }

  return sum / 16;
}


float calcSingleDirectionalShadow(vec3 pos,mat4 shadowMat,sampler2DShadow shadowMap, float bias, bool spot)
{ 
  float sum = 1.0;

  vec4 ShadowCoord = shadowMat * vec4(pos,1);

  if( EnableShadowsDirectional > 0 )
  {
    if(EnableShadowsDirectional == 2 )
    {
      sum = calcDirShadowPCF(pos,shadowMat, shadowMap, bias, spot);  
    }
    if(EnableShadowsDirectional == 3)
    {
      sum = calcDirShadowRandomSampling(pos,shadowMat, shadowMap,bias, true);       
    }
    if(EnableShadowsDirectional == 1)
    { 
    //  if(ShadowCoord.z  >= 0)
     // { 
        sum =  textureProj(shadowMap, ShadowCoord ) ;  
        if(spot)
          sum = LinearizeDepth(sum);
         if(ShadowCoord.z+bias > sum )
           sum = 0;   
         else
           sum = 1;  
    //  }
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


  if( EnableShadowsPoint > 0 )
  {
    if(EnableShadowsPoint == 1)
    {       
      vec3 lightDir = lightPos - fragPos ;
      float lightDistance = length(lightDir);

      float closestDepth = texture(sam, -lightDir).r; 
      if(lightDistance-0.001 >= closestDepth)
        shadow =0.5f;
    }
    if(EnableShadowsPoint == 2 )
    { 
      // PCF goes here
      shadow = calcPointShadowPCF(lightPos, fragPos, sam);
    }
  }

  return shadow;
}



float calcPointShadowfactor(int idx,vec3 lightPos, vec3 fragPos)
{
  vec3 pos = vec3( texture( Tex1, TexCoord ) );
  float ret =0.0;
  if( EnableShadowsPoint > 0 )
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
  }
  else
    ret =1.0;
  return ret;
}  

float calcSpotShadowFactor(int idx, float bias, bool spot)
{
  float ret = 0.0;
  vec3 pos = vec3( texture( Tex1, TexCoord ) );

  if( EnableShadowsDirectional > 0 )
  {
    if(idx == 0 )  
      ret += calcSingleDirectionalShadow(pos, ShadowMatrices[0], ShadowData0,bias, spot);  
    if(idx == 1 )
      ret += calcSingleDirectionalShadow(pos, ShadowMatrices[1], ShadowData1,bias, spot);  
    if(idx == 2 )
      ret += calcSingleDirectionalShadow(pos, ShadowMatrices[2], ShadowData2,bias, spot);  
    if(idx == 3 )
      ret += calcSingleDirectionalShadow(pos, ShadowMatrices[3], ShadowData3,bias, spot);
    if(idx == 4 )
      ret += calcSingleDirectionalShadow(pos, ShadowMatrices[4], ShadowData4,bias, spot);
    if(idx == 5 )
      ret += calcSingleDirectionalShadow(pos, ShadowMatrices[5], ShadowData5,bias, spot);
    if(idx == 6 )
      ret += calcSingleDirectionalShadow(pos, ShadowMatrices[6], ShadowData6,bias, spot);
    if(idx == 7 )
      ret += calcSingleDirectionalShadow(pos, ShadowMatrices[7], ShadowData7,bias, spot);
    if(idx == 8 )
      ret += calcSingleDirectionalShadow(pos, ShadowMatrices[8], ShadowData8,bias, spot);
    if(idx == 9 )
      ret += calcSingleDirectionalShadow(pos, ShadowMatrices[9], ShadowData9,bias, spot);
    if(idx == 10 )
      ret += calcSingleDirectionalShadow(pos, ShadowMatrices[10], ShadowData10,bias, spot); 
  }
  else
    ret = 1.0;
  
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

  vec3 specular =   specularColor * spec * 0.1;
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

  float bias = max(0.05 * (1.0 - dot(norm, lightDir)), 0.015);

  vec3 ambient =  l.Base.AmbientColor   ;//l.Base.AmbientColor * mat.AmbientColor;
  if(EnableSSAO == 1)
   ambient =  (texture(Tex6, TexCoord).xyz  )  ;
  
  vec3  specular   = calcSpecularColor(lightDir, l.Base.SpecularColor, mat,pos,normal);

 
  float shadowFactor = 1.0f;
 // if(length( vec2(pos.x,pos.z) - vec2(CameraPosition.x,CameraPosition.z )) <10 )


  if(l.ShadowMapIndex != -1)
     shadowFactor = calcSpotShadowFactor(l.ShadowMapIndex, bias, false);

  return vec4(diffuse* shadowFactor * l.Base.Intensity  +ambient* l.Base.Intensity + specular* shadowFactor * l.Base.Intensity,1) ;
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
  vec3 diffuse = diff * l.Base.DiffuseColor ;

  vec3 ambient =  l.Base.AmbientColor  * 0.1   ;//l.Base.AmbientColor * mat.AmbientColor;
  if(EnableSSAO == 1 && (texture(Tex7, TexCoord).xyz == vec3(0,0,0) ) )
   ambient =  (texture(Tex6, TexCoord).xyz  )  ;

  vec3 specular = calcSpecularColor(lightDir, l.Base.SpecularColor, mat,pos,normal);

  float attenuation = 1.0f / (l.Attenuation.Constant + l.Attenuation.Linear * distance + l.Attenuation.Quadratic * (distance * distance));

  float  shadowFactor  = 1.0;
  if(l.ShadowMapIndex != -1)
      shadowFactor = calcPointShadowfactor(l.ShadowMapIndex,l.Position, pos.xyz);

  return vec4( (diffuse* l.Base.Intensity* shadowFactor  + specular* shadowFactor * l.Base.Intensity   ) * attenuation + ambient* l.Base.Intensity,1)  ;
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

  vec3 ambient =  l.Pointlight.Base.AmbientColor * 0.2  ;//l.Base.AmbientColor * mat.AmbientColor;

  if(EnableSSAO == 1 && (texture(Tex7, TexCoord).xyz == vec3(0,0,0) ) )
   ambient =  (texture(Tex6, TexCoord).xyz  )  ;

  
  float attenuation = 1.0f / (l.Pointlight.Attenuation.Constant + l.Pointlight.Attenuation.Linear * distance + l.Pointlight.Attenuation.Quadratic * (distance * distance));

  if (angle < cutoff)
  {
    vec4 color;
    vec3 norm = normalize(normal);       

    vec3 diffuse =  spotFactor* l.Pointlight.Base.DiffuseColor  ; 

    vec3 v = normalize( vec3(-pos.xyz));
    vec3 h = normalize( v +lightToPixel );
    float spec = pow(max(dot(h, norm), 0.0), mat.Shininess);

    vec3 specular =    l.Pointlight.Base.SpecularColor * spec ;
    float bias = 0.01; //  max(0.05 * (1.0 - dot(norm, lightToPixel)), 0.1);
    float shadowFactor = 1.0;

    if(l.ShadowMapIndex != -1)
      shadowFactor = calcSpotShadowFactor(l.ShadowMapIndex,bias, true);


    float delta = cutoff - angle; // maybe use different smoothing

    color = vec4( (diffuse* l.Pointlight.Base.Intensity* shadowFactor  * delta+ ambient* l.Pointlight.Base.Intensity * spotFactor* 0.1 + specular* shadowFactor * delta* l.Pointlight.Base.Intensity  * spotFactor ) *attenuation  ,1) ;
    
    return color;
  }
  else
  {
    return vec4( ambient,1.0) *0.1*l.Pointlight.Base.Intensity * attenuation; // 0.1 just to scale down
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

  if(NormalMapActive == 1 )
  {
    NormalData= vec3( texture( Tex5, TexCoord ) );
    NormalData = normalize(NormalData * 2.0 - 1.0  );   
  }
  if(OpacityMapActive == 1 )
  {    
    color.w = texture( Tex6, TexCoord ).x;    
  }

  ColorData = color;
  MaterialData = vec3(MaterialIndex,ObjectId,  LinearizeDepth(gl_FragCoord.z)); // if there is a /1000 delete it , just for testing
}
// PASS 1 END    -----------------------------------------------------



// PASS 2     --------------------------------------------------------
subroutine (RenderPassType)
void pass2()
{
  vec3 pos = vec3( texture( Tex1, TexCoord ) );
  vec3 norm = vec3( texture( Tex2, TexCoord ) );
  vec4 diffColor = texture(Tex3, TexCoord) ;
  vec3 materialIndex = vec3( texture(Tex4, TexCoord) );
  int matIdx = int(materialIndex.x);

  if( texture(Tex7, TexCoord).xyz != vec3(0,0,0) )
   pos =  texture(Tex8, TexCoord).xyz ;

  vec4 color = diffColor;
  vec4 light = vec4(0,0,0,0);

  if( texture(Tex7, TexCoord).xyz != vec3(0,0,0) )
  {
    color = diffColor * (1 - texture(Tex7, TexCoord).w);
    color  =  texture(Tex7, TexCoord)* texture(Tex7, TexCoord).w; 
    color.w = 1.0;
  }
 
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

   
    color =color *light ; 
    FragColor = color  ;
    

    float len = length(CameraPosition - pos);
    if( len > FogOffset )
    {      
      vec3 fac = FogColor *len*len  *FogFactor;

      FragColor+=vec4(fac.x,fac.y,fac.z,diffColor.w);
    }

    //FragColor = vec4(0,shadow,0,1.0);
 }

 //FragColor =  texture(Tex6, TexCoord);

 //color.w = diffColor.w;

 if(matIdx ==99999)
  FragColor = color;  



 //FragColor = vec4(pos,1);
// FragColor  =  texture(Tex6, TexCoord);



}
// PASS 2 END    -----------------------------------------------------

// PASS TRANSPARENT     --------------------------------------------------------

subroutine (RenderPassType)
void passTransparent()
{
  vec4 color;
  vec4 light = vec4(0,0,0,0);
  Material mat = Materials[MaterialIndex]; 

  if(ColorOnly == 1)
    color = vec4(mat.DiffuseColor,1.0);
  else
    color = texture(Tex1, TexCoord);
  
  vec2 cc = gl_FragCoord.xy;
 
  cc = vec2(gl_FragCoord.x/GBufferSize.x,gl_FragCoord.y/GBufferSize.y);

  vec3 pos = vec3( texture( Tex7, cc) );
  vec3 materialIndex = vec3( texture(Tex10, cc) );

  float d1 = distance(CameraPosition, pos);
  float d2 = distance(CameraPosition, Position);

  if(OpacityMapActive == 1 )
  {      
    color.w =  texture( Tex6, TexCoord ).x;
  }
  else
    color.w =0.0;

  if(d1 < d2)
    color.x =color.y =color.z = color.w = 0;

  if(color.a < 0.2)
    discard;
  else
  {
    PositionData = Position;
  }

  ColorData = color;
  NormalData = color.xyz;
  FragColor = color;
}
// PASS TRANSPARENT END    -----------------------------------------------------


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