/* FILE NAME: frag.glsl
 * PROGRAMMER: VI6
 * DATE: 11.06.2020
 * PURPOSE: 3D animation project.
 *          Fragment shader.
 */

#version 420

layout(location = 0) out vec4 OutColor; // --> gl_FragColor;

in vec4 DrawColor;
in vec3 DrawPos;
in vec3 DrawNormal;
in vec2 DrawTexCoord;

uniform vec3 Ka, Kd, Ks;
uniform float Ph;
uniform float Trans;

uniform mat4 ShadowMatr;
uniform vec3 LightDir, LightColor;

uniform float Addon0, Addon1, Addon2;

layout(binding = 0) uniform sampler2D Texture0;
layout(binding = 8) uniform sampler2D ShadowMap;
uniform bool IsTexture0;

uniform float GlobalTime;
uniform float Time;
uniform vec3 CamLoc;

vec3 Shade( vec3 P, vec3 N, vec3 L, vec3 LColor )
{
  vec3 color = vec3(0);
  vec3 V = normalize(P - CamLoc);
  L = normalize(L);
  N = normalize(N);

  // Ambient
  color += min(vec3(0.1), Ka);

  // Duffuse
  float nl = dot(N, L);
  if (nl > 0)
  {
    vec3 diff = LColor * Kd * nl;
    if (IsTexture0)
      diff *= texture(Texture0, vec2(1, -1) * DrawTexCoord).rgb;
    color += diff;

    // Specular
    vec3 R = reflect(V, N);
    float rl = dot(R, L);
    if (rl > 0)
      color += LColor * Ks * pow(rl, Ph);
  }
  return color;
}

/* Shader entry point */
void main( void )
{
  vec3 color = DrawColor.rgb * Shade(DrawPos, DrawNormal, LightDir, LightColor);

  vec4 p = ShadowMatr * vec4(DrawPos, 1) * 0.5 + 0.5;
  float sh = 1;
  if (p.x >= 0 && p.y >= 0 && p.x <= 1 && p.y <= 1)
  {
    sh = 0.8;
    float s = texture(ShadowMap, p.xy).r;
    if (p.z > s)
     sh = 0.47;
  }


  OutColor = vec4(color * sh, Trans);
} /* End of 'main' function */
