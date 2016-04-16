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

layout(binding=0) uniform sampler2D Tex1; 
layout(binding=1) uniform sampler2D Tex2; 
layout(binding=2) uniform sampler2D Tex3; 

uniform int ColorOnly;
uniform vec4 Color;
uniform int NumDirectionalLights;
uniform int NumSpotLights;
uniform int NumPointLights;

uniform vec3 CameraPosition;

// MATERIAL -----------------------------------------------------------

struct MaterialS
{
  vec3  DiffuseColor;
  vec3  AmbientColor;
  vec3  SpecularColor;
  float Shininess;
  float Opacity;
};
uniform MaterialS Material;

// MATERIAL END  ------------------------------------------------------

//  LIGHTS ------------------------------------------------------------

const int MAX_DIRECTIONAL_LIGHTS =  5;
const int MAX_POINT_LIGHTS =  20;
const int MAX_SPOT_LIGHTS =  20;

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

// LIGHT CALCULATION -------------------------------------------------

// calculates the specular part
vec3 calcSpecularColor(vec3 lightDirection, vec3 specularColor)
{
  vec3 norm =       normalize(Normal);
  vec3 viewDir =    normalize(CameraPosition - ModelPos.xyz);
  vec3 reflectDir = reflect(-lightDirection, norm);
  float spec =      pow(max(dot(viewDir, reflectDir), 0.0), Material.Shininess);

  vec3 specular =   specularColor * spec * Material.SpecularColor;
  return specular;
}

// directionLight calculation
vec4 calcDirectionalLight(int idx)
{
  DirectionalLight l = DirectionalLights[idx];
  vec3  norm     = normalize(Normal);
  vec3  lightDir = normalize(-l.Direction);
  float diff     = max(dot(norm, lightDir), 0.0);
  vec3 diffuse = diff * l.Base.DiffuseColor;

  vec3 ambient   = l.Base.AmbientColor * Material.AmbientColor;
  
  vec3  specular   = calcSpecularColor(lightDir, l.Base.SpecularColor);

  return vec4(diffuse* l.Base.Intensity + ambient* l.Base.Intensity + specular* l.Base.Intensity,1) ;
}

// pointLight calculation
vec4 calcPointLights(int idx)
{  
  PointLight l = PointLights[idx];
  vec3 norm = normalize(Normal);
  vec3 lightDir = normalize(l.Position - ModelPos.xyz);
  float distance   = length(l.Position - ModelPos.xyz);

  float diff = max(dot(norm, lightDir), 0.0);
  vec3 diffuse = diff * l.Base.DiffuseColor * Material.DiffuseColor;

  vec3 ambient = l.Base.AmbientColor * Material.AmbientColor;

  vec3 specular = calcSpecularColor(lightDir, l.Base.SpecularColor);

  float attenuation = 1.0f / (l.Attenuation.Constant + l.Attenuation.Linear * distance + l.Attenuation.Quadratic * (distance * distance));

  return vec4( (diffuse* l.Base.Intensity + ambient* l.Base.Intensity + specular* l.Base.Intensity ) * attenuation,1) ;
}

// spotLight calculation
vec4 calcSpotLight(int idx)
{
  SpotLight l = SpotLights[idx];
  vec3 lightToPixel = normalize( l.Pointlight.Position - ModelPos.xyz);
  float angle = acos(dot(-lightToPixel,l.Direction));
  float cutoff = radians (clamp (l.CutoffAngle, 0.0 ,90.0));
  float spotFactor =pow (dot( -lightToPixel, l.Direction),0.1);

  float distance   = length(l.Pointlight.Position - ModelPos.xyz);

  vec3 ambient = l.Pointlight.Base.AmbientColor * Material.AmbientColor;
  float attenuation = 1.0f / (l.Pointlight.Attenuation.Constant + l.Pointlight.Attenuation.Linear * distance + l.Pointlight.Attenuation.Quadratic * (distance * distance));

  if (angle < cutoff)
  {
    vec4 color;
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(l.Pointlight.Position - ModelPos.xyz);   

    float diff = max(dot(lightToPixel, norm), 0.0);
    vec3 diffuse = diff * l.Pointlight.Base.DiffuseColor * Material.DiffuseColor ;

    vec3 v = normalize( vec3(-ModelPos.xyz));
    vec3 h = normalize( v +lightToPixel );
    float spec = pow(max(dot(h, norm), 0.0), Material.Shininess);

    vec3 specular =    l.Pointlight.Base.SpecularColor * spec * Material.SpecularColor;

    color =vec4( (diffuse* l.Pointlight.Base.Intensity  * spotFactor + ambient* l.Pointlight.Base.Intensity + specular* l.Pointlight.Base.Intensity  * spotFactor ) * attenuation,1) ;
    return color;
  }
  else
  {
    return  vec4(ambient,1.0) *l.Pointlight.Base.Intensity * attenuation;
  }
}
// LIGHT CALCULATION END ---------------------------------------------

// PASS 1     --------------------------------------------------------

subroutine (RenderPassType)
void pass1()
{
  vec4 color;
  vec4 light = vec4(0,0,0,0);

  if(ColorOnly == 1)
    color = vec4(Material.DiffuseColor,1.0);
  else
    color = vec4( vec3(texture(Tex1, TexCoord)) ,1.0 );

  for(int x=0; x< NumDirectionalLights; x++ )
    light += calcDirectionalLight(x);
  for(int x=0; x< NumPointLights; x++ )
    light += calcPointLights(x);
  for(int x=0; x< NumSpotLights; x++ )
    light += calcSpotLight(x);
  FragColor = color * light;


}
// PASS 1 END    -----------------------------------------------------

// PASS 2     --------------------------------------------------------

subroutine (RenderPassType)
void pass2()
{
  FragColor = vec4(Position,1.0);
}
// PASS 2 END    -----------------------------------------------------

// MAIN    -----------------------------------------------------------

void main()
{
  RenderPass();  
}

// MAIN END ----------------------------------------------------------