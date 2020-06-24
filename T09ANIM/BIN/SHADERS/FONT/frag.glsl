/* FILE NAME: frag.glsl
 * PROGRAMMER: VI6
 * DATE: 11.06.2020
 * PURPOSE: 3D animation project.
 *	    Fragment shader.
 */
#version 420

layout(location = 0) out vec4 OutColor;

in vec4 DrawColor;
in vec3 DrawPos;
in vec3 DrawNormal;
in vec2 DrawTexCoord;

uniform vec3 Ka, Kd, Ks;
uniform float Ph;
uniform float Trans;

layout(binding = 0) uniform sampler2D Texture0;
uniform bool IsTexture0;

uniform float Time;
uniform float GlobalTime;
uniform vec3 CamLoc;

/* Shader entry point */
void main( void )
{
  vec4 tc = texture(Texture0, DrawTexCoord);
  if (tc.a < 0.5)
    discard;
  OutColor = vec4(tc.rgb, 1);
} /* End of 'main' function */