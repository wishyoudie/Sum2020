/* FILE NAME: RNDLIGHT.C
 * PROGRAMMER: VI6
 * DATE: 18.06.2020
 * PURPOSE: 3D animation project.
 *          Light and shadow handle module.
 */

#include "../anim.h"

/* Shadow map size */ 
#define VI6_RND_SHADOW_MAP_SIZE 16384

/* Shadow frame buffer identifier */ 
static INT VI6_RndShadowFBO;

/* Shadow map texture identifier */ 
INT VI6_RndShadowTexId;

/* Shadow map light source matrix */ 
MATR VI6_RndShadowMatr;

/* Flag for shadow drawing rendering pass */ 
BOOL VI6_RndShadowPassFlag;

/* Light source direction */ 
VEC VI6_RndLightDir;

/* Light source color */ 
VEC VI6_RndLightColor;

/* Light system initialization function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID VI6_RndLightInit( VOID )
{
  INT tex_no, status;

  /* Set default light source */
  VI6_RndLightDir = VecNormalize(VecSet(-0.690, 0.571, 0.593));
  VI6_RndLightColor = VecSet(1.000, 1.000, 1.000); // White

  /* Create shadow map texture */
  tex_no = VI6_RndTexAddFmt("ShadowMap", VI6_RND_SHADOW_MAP_SIZE, VI6_RND_SHADOW_MAP_SIZE, GL_DEPTH_COMPONENT32);
  VI6_RndShadowTexId = VI6_RndTextures[tex_no].TexId;

  /* Create shadow FBO */
  glGenFramebuffers(1, &VI6_RndShadowFBO);
  glBindFramebuffer(GL_FRAMEBUFFER, VI6_RndShadowFBO);

  /* Attach create depth texture */
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, VI6_RndShadowTexId, 0);
  /*
  glBindTexture(GL_TEXTURE_2D, VI6_RndShadowTexId);
  glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &w);
  glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &h);
  glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_INTERNAL_FORMAT, &fmt);
  */

  status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
  if (status != GL_FRAMEBUFFER_COMPLETE)
    OutputDebugStr("Error in framebuffer\n");
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glBindTexture(GL_TEXTURE_2D, 0);
} /* End of 'VI6_RndLightInit' function */

/* Light system deinitialization function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID VI6_RndLightClose( VOID )
{
  /* Remove FBO */
  glBindFramebuffer(GL_FRAMEBUFFER, VI6_RndShadowFBO);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, 0, 0);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glDeleteFramebuffers(1, &VI6_RndShadowFBO);
  VI6_RndShadowFBO = 0;
} /* End of 'VI6_RndLightClose' function */

/* Draw shadow map function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID VI6_RndLightShadow( VOID )
{
  INT i;
  FLT clear_depth = 1, size = 75, far_dist = 300;
  MATR m1, m2, m3;
  VEC
    RndCamLoc = VI6_RndCamLoc,
    RndCamAt = VI6_RndCamAt,
    RndCamRight = VI6_RndCamRight,
    RndCamUp = VI6_RndCamUp,
    RndCamDir = VI6_RndCamDir;


  /* Save camera data */
  m1 = VI6_RndMatrView;
  m2 = VI6_RndMatrProj;
  m3 = VI6_RndMatrVP;

  /* Set camera from light source */
  VI6_RndCamSet(VecSet(0, 8, 3), VecSet(0, 0, 0), VecSet(0, 1, 0));

  VI6_RndMatrProj = MatrOrtho(-size, size, -size, size, -size, far_dist);
  /* VI6_RndMatrView = MatrView(VI6_RndCamAt, VecSubVec(VI6_RndCamAt, VI6_RndLightDir), VecSet(0, 1, 0)); */
  /* VI6_RndMatrVP = MatrMulMatr(VI6_RndMatrView, VI6_RndMatrProj); */
  VI6_RndCamSet(RndCamAt, VecSubVec(RndCamAt, VI6_RndLightDir), VecSet(0, 1, 0));
  VI6_RndShadowMatr = VI6_RndMatrVP;

  glViewport(0, 0, VI6_RND_SHADOW_MAP_SIZE, VI6_RND_SHADOW_MAP_SIZE);

  /* Set shadow map frame buffer */
  glBindFramebuffer(GL_FRAMEBUFFER, VI6_RndShadowFBO);

  /* Clear depth */
  glClearBufferfv(GL_DEPTH, 0, &clear_depth);

  /* Redraw all scene */
  VI6_RndShadowPassFlag = TRUE;
  glEnable(GL_POLYGON_OFFSET_FILL);
  glPolygonOffset(4, 4);
  for (i = 0; i < VI6_Anim.NumOfUnits; i++)
    VI6_Anim.Units[i]->Render(VI6_Anim.Units[i], &VI6_Anim);
  VI6_RndShadowPassFlag = FALSE;
  glDisable(GL_POLYGON_OFFSET_FILL);

  /* Restore main frame buffer */
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glViewport(0, 0, VI6_RndFrameW, VI6_RndFrameH);

  /* Restore camera data */
  VI6_RndMatrView = m1;
  VI6_RndMatrProj = m2;
  VI6_RndMatrVP = m3;
  VI6_RndCamLoc = RndCamLoc;
  VI6_RndCamAt = RndCamAt;
  VI6_RndCamRight = RndCamRight;
  VI6_RndCamUp = RndCamUp;
  VI6_RndCamDir = RndCamDir;
} /* End of 'VI6_RndLightShadow' function */

/* END OF 'RNDLIGHT.C' FILE */
