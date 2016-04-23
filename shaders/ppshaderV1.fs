#version 440

in vec2 TexCoord;

layout(location = 0 ) out vec4 FragColor;
layout(location = 1 ) out vec3 PositionData;

layout(binding=0) uniform sampler2D Tex1; 



uniform vec2 TextureScale;

uniform int EnableFXAA;

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

void main()
{
  vec4 color = texture2D(Tex1, TexCoord );

  if( EnableFXAA == 1 )
   color = fxaa();


  FragColor = color;
  
}

