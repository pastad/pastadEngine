#version 430


subroutine void RenderPassType();
subroutine uniform RenderPassType RenderPass;


in vec2 TexCoord;

layout(location = 0 ) out vec4 FragColor;
layout(location = 1 ) out vec3 PositionData;

layout(binding=0) uniform sampler2D Tex1; 
layout(binding=1) uniform sampler2D Tex2; 
layout(binding=2) uniform sampler2D Tex3; 
layout(binding=3) uniform sampler2D Tex4; 
layout(binding=4) uniform sampler2D Tex5; 
layout(binding=5) uniform sampler2D Tex6; 
layout(binding=6) uniform sampler2D Tex7; 
layout(binding=7) uniform sampler2D Tex8; 

layout(binding=10) uniform sampler2D TexJitter;

uniform vec2 TextureScale;
uniform float AverageLuminance;
uniform float Exposure;
uniform float BloomThreshold;

uniform float GaussKernel[10];

uniform vec3 SSAOSamples[64];

uniform int EnableFXAA;
uniform int EnableHDR;
uniform int EnableBloom;

uniform mat4 CameraProjection;
uniform mat4 CameraView;
uniform vec3 CameraPosition;


const int MAX_NUM_MATERIALS =  20;

uniform int Emissive[MAX_NUM_MATERIALS];

// inspired by https://www.youtube.com/watch?v=Z9bYzpwVINA

vec4 fxaa()
{
  float SPAN_MAX = 8.0;
  float REDUCE_MIN = 1.0/ 128.0; // 128.0f s
  float REDUCE_MUL = 1.0/ 8.0;

  vec3 lumi = vec3(0.299,0.587,0.114);

  float lTL = dot(lumi, (texture2D(Tex1, TexCoord + (vec2(-1.0,-1.0) * TextureScale ) )).xyz);
  float lBL = dot(lumi, (texture2D(Tex1, TexCoord + (vec2(-1.0,1.0) * TextureScale ))).xyz);
  float lM = dot(lumi, (texture2D(Tex1, TexCoord )).xyz);
  float lTR = dot(lumi, (texture2D(Tex1, TexCoord + (vec2(1.0,-1.0) * TextureScale  ))).xyz);
  float lBR = dot(lumi, (texture2D(Tex1, TexCoord + (vec2(1.0,1.0) * TextureScale ))).xyz);

  vec2 direction;
  direction.x = -((lTL+lTR) -  (lBL+lBR)); // invert due to flipped tex
  direction.y = ((lTL+lBL) -  (lTR+lBR));

  float red = max((lTL+lTR + lBL+ lBR) * ( REDUCE_MUL *0.25), REDUCE_MIN);
  float sc = 1.0 / ( min(abs(direction.x),abs(direction.y))+red) ; 

  // clamp
  direction = min(vec2(SPAN_MAX, SPAN_MAX), max(vec2(-SPAN_MAX,-SPAN_MAX), direction * sc) ) * 
  TextureScale; 

  vec3 res1 = (1.0/2.0) * ( texture2D(Tex1, TexCoord + (direction * vec2(1.0/3.0 -0.5) ) ).xyz +
    texture2D(Tex1, TexCoord + (direction * vec2(2.0/3.0 -0.5) ) ).xyz ); 
  
  vec3 res2 = res1 * (1.0/2.0) + (1.0/4.0)* ( texture2D(Tex1, TexCoord + (direction * vec2(0.0/3.0 -0.5) ) ).xyz +
    texture2D(Tex1, TexCoord + (direction * vec2(3.0/3.0 -0.5) ) ).xyz ); 

  float lMin = min(lM, min(min(lTL,lTR), min(lBL,lBR))  );
  float lMax = max(lM, max(max(lTL,lTR), max(lBL,lBR) )  );
  float lres2 = dot(lumi ,res2);
  if( lres2 < lMin || lres2 >lMax )
    return vec4(res1,1.0);
  else
   return vec4(res2,1.0);
}

uniform mat3 rgb2xyz = mat3( 
  0.4124564, 0.2126729, 0.0193339,
  0.3575761, 0.7151522, 0.1191920,
  0.1804375, 0.0721750, 0.9503041 );

uniform mat3 xyz2rgb = mat3(
  3.2404542, -0.9692660, 0.0556434,
  -1.5371385, 1.8760108, -0.2040259,
  -0.4985314, 0.0415560, 1.0572252 );


vec4 hdr(vec4 color)
{ 
  float White = 0.928;
  float exp  =0.1; // Exposure

  vec3 xyzCol = rgb2xyz * vec3(color);

  float xyzSum = xyzCol.x + xyzCol.y + xyzCol.z;
  vec3 xyYCol = vec3( xyzCol.x / xyzSum, xyzCol.y / xyzSum, xyzCol.y);

  float L = (exp * xyYCol.z) / AverageLuminance;
  L = (L * ( 1 + L / (White * White) )) / ( 1 + L );

  xyzCol.x = (L * xyYCol.x) / (xyYCol.y);
  xyzCol.y = L;
  xyzCol.z = (L * (1 - xyYCol.x - xyYCol.y))/xyYCol.y;

  return vec4( xyz2rgb * xyzCol, 1.0);
}
vec4 hdr_exposure(vec4 color)
{
  float exp_val = Exposure;
  return ( vec4(1.0) - exp(-color * exp_val)  ); 
}
vec4 hdr_reinhard(vec4 color)
{
  return (color / (color + vec4(1.0)) );
}

vec4 gamma_correction(vec4 color)
{
  return pow(color, vec4(1.0 / 2.2));
}

subroutine (RenderPassType)
void passStandard()
{
  vec4 color = texture2D(Tex1, TexCoord );
  vec4 lightblur = texture2D(Tex2, TexCoord );
  vec4 ssao = texture2D(Tex3, TexCoord );
  FragColor = color;
  if( EnableFXAA == 1 )
   color = fxaa();

  if( (EnableHDR == 1) )
   color = hdr_exposure(color);
 
  color = gamma_correction(color);  

  if( EnableBloom == 1 )
    FragColor =  color +lightblur;//
  else
    FragColor = color ;//

 // FragColor = ssao*0.3;
 
}

subroutine (RenderPassType)
void passBright()
{
  vec4 color = texture2D(Tex1, TexCoord );
  vec4 material = texture2D(Tex2, TexCoord );

  if(EnableHDR == 1)
    color = hdr_exposure(color);
  float brightness = dot(color.rgb, vec3(0.2126, 0.7152, 0.0722));

  if(  (brightness > BloomThreshold)   )
    FragColor = vec4(brightness)*0.1; // 0.1 for brightness scaling
  else
  {
    if(  Emissive[ int(material.x )] == 1 )
      FragColor = vec4(brightness);
    else
      FragColor = vec4(0,0,0,0);
  }
}


subroutine (RenderPassType)
void passBlur()
{
  vec4 color = texture2D(Tex1, TexCoord ) *GaussKernel[0];
  vec2 off = 1.0 / textureSize(Tex1, 0); 
  for(int i = 1; i < 4; i++)
  {
    color += texture2D(Tex1, TexCoord + vec2(off.x * i, 0.0)) * GaussKernel[i];
    color += texture2D(Tex1, TexCoord - vec2(off.x * i, 0.0)) * GaussKernel[i];
  }

  FragColor = color;
}
subroutine (RenderPassType)
void passBlur2()
{
  vec4 color = texture2D(Tex1, TexCoord )* GaussKernel[0];
  vec2 off = 1.0 / textureSize(Tex1, 0); 
  for(int i = 1; i < 4; i++)
  {
    color += texture2D(Tex1, TexCoord + vec2(0.0, off.y * i)) * GaussKernel[i];
    color += texture2D(Tex1, TexCoord - vec2(0.0, off.y * i)) * GaussKernel[i];
  }
  FragColor = color;
}
subroutine (RenderPassType)
void passSSAO()
{
  vec2 ws = 1.0 / TextureScale;
  const vec2 noiseScale = vec2(ws.x/6.0, ws.y/6.0); 
   
  vec3 pos = vec3( texture( Tex1, TexCoord ) );
 // if( texture(Tex7, TexCoord).xyz != vec3(0,0,0) )
 //   pos  =  texture(Tex8, TexCoord).xyz;


  vec3 norm = vec3( texture( Tex2, TexCoord ) );
  vec3 diffColor = vec3( texture(Tex3, TexCoord) );
  vec3 material = vec3( texture(Tex4, TexCoord) );
  vec3 noise = vec3( texture(Tex6, TexCoord* noiseScale) );

  vec3 tan = normalize(noise - norm* dot(noise, norm));
  vec3 bitan = cross(norm, tan);
  mat3 trans = mat3(tan, bitan, norm);  

  float occ = 0.0;

  vec4 fp = -1.0 * CameraView* vec4( pos ,1);
  

  for( int x=0; x< 64; x++)
  {
	  vec3 sam = trans * SSAOSamples[x];
    float radius = 1.0;
	  sam = fp.xyz+ sam *radius; // 1.0 radius

	  vec4 off = vec4(sam,1.0);
	  off = CameraProjection * off;
	  off.xyz /= off.w;
	  off.xyz = off.xyz *0.5 +0.5;

	  float depth = texture(Tex4, off.xy ).z; // the depth from the camera
    // TODO change depth if transparent
  //  if( texture(Tex7, off.xy).xyz != vec3(0,0,0) )
    //  depth  = distance (texture(Tex8, off.xy).xyz, CameraPosition);
   // else
   //   depth  = distance (texture(Tex1, off.xy).xyz, CameraPosition);
    
    depth+= 0.15;

	  float range = smoothstep(0.0,1.0, radius/ abs(fp.z-depth) );
	  occ += ( ( (depth >= sam.z) && ( (depth- sam.z)  < radius)  )  ? 1.0 : 0.0 ) *range;	
  }

  float depth = texture(Tex4, TexCoord ).y;
  occ = (occ /64.0);

  FragColor  = vec4(occ,occ,occ,1.0);
}



void main()
{
  RenderPass();    
}

