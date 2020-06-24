/* FILE NAME: RNDMTL.C
 * PROGRAMMER: VI6
 * DATE: 12.06.2020
 * PURPOSE: 3D animation project.
 *          Render system material handle module.
 */

#include "rndres.h"

vi6MATERIAL VI6_RndMaterials[VI6_MAX_MATERIALS]; /* Array of materials */
INT VI6_RndMaterialsSize;                        /* Materials array store size */

/* Material storage initialize function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID VI6_RndMtlInit( VOID )
{
  vi6MATERIAL mtl;
  INT i;

  VI6_RndMaterialsSize = 0;

  strncpy(mtl.Name, "Default", VI6_STR_MAX - 1);
  memset(&mtl, 0, sizeof(mtl));
  mtl.Ka = VecSet1(0.1);
  mtl.Kd = VecSet1(0.9);
  mtl.Ks = VecSet1(0.0);
  mtl.Ph = 30;
  mtl.Trans = 1;
  for (i = 0; i < 8; i++)
    mtl.Tex[i] = -1;
  mtl.ShdNo = 0;
  VI6_RndMtlAdd(&mtl);
} /* End of 'VI6_RndMtlInit' function */

/* Add new material to material store system deinitialization function.
 * ARGUMENTS:
 *   - Material data structure pointer:
 *      - vi6MATERIAL *Mtl;
 * RETURNS:
 *   - Number of material in material array:
 *      - INT VI6_RndMaterialSize;
 */
INT VI6_RndMtlAdd( vi6MATERIAL *Mtl )
{
  if (VI6_RndMaterialsSize >= VI6_MAX_MATERIALS)
    return -1;
  
  VI6_RndMaterials[VI6_RndMaterialsSize] = *Mtl;
  return VI6_RndMaterialsSize++;
} /* End of 'VI6_RndMtlAdd' function */

/* Material storage deinitialize function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID VI6_RndMtlClose( VOID )
{
  VI6_RndMaterialsSize = 0;
} /* End of 'VI6_RndMtlClose' function */

/* Apply material function
 * ARGUMENTS:
 *   - Material number:
 *      - INT MtlNo;
 * RETURNS:
 *   - Shader progid: (INT) prg.
 */
INT VI6_RndMtlApply( INT MtlNo )
{
  INT prg, loc, i;
  vi6MATERIAL *mtl;

  /* Set material pointer */
  if (MtlNo < 0 || MtlNo >= VI6_RndMaterialsSize)
    MtlNo = 0;
  mtl = &VI6_RndMaterials[MtlNo];

  /* Set shader program Id */
  prg = mtl->ShdNo;
  if (prg < 0 || prg >= VI6_RndShadersSize)
    prg = 0;
  prg = VI6_RndShaders[prg].ProgId;

  glUseProgram(prg);
  /* Set shading parameters */
  if ((loc = glGetUniformLocation(prg, "Ka")) != -1)
    glUniform3fv(loc, 1, &mtl->Ka.X);
  if ((loc = glGetUniformLocation(prg, "Kd")) != -1)
    glUniform3fv(loc, 1, &mtl->Kd.X);
  if ((loc = glGetUniformLocation(prg, "Ks")) != -1)
    glUniform3fv(loc, 1, &mtl->Ks.X);
  if ((loc = glGetUniformLocation(prg, "Ph")) != -1)
    glUniform1f(loc, mtl->Ph);
  if ((loc = glGetUniformLocation(prg, "Trans")) != -1)
    glUniform1f(loc, mtl->Trans);

  if ((loc = glGetUniformLocation(prg, "LightDir")) != -1)
    glUniform3fv(loc, 1, &VI6_RndLightDir.X);
  if ((loc = glGetUniformLocation(prg, "LightColor")) != -1)
    glUniform3fv(loc, 1, &VI6_RndLightColor.X);
  if ((loc = glGetUniformLocation(prg, "ShadowMatr")) != -1)
    glUniformMatrix4fv(loc, 1, FALSE, &VI6_RndShadowMatr.A[0]);
  /* Set textures */
  for (i = 0; i < 8; i++)
  {
    CHAR tname[] = "IsTexture0";

    tname[9] = '0' + i;
    if (mtl->Tex[i] != -1)
    {
      glActiveTexture(GL_TEXTURE0 + i);
      glBindTexture(GL_TEXTURE_2D, VI6_RndTextures[mtl->Tex[i]].TexId);
    }
    if ((loc = glGetUniformLocation(prg, tname)) != -1)
      glUniform1i(loc, mtl->Tex[i] != -1);
  }

  glActiveTexture(GL_TEXTURE0 + 8);
  glBindTexture(GL_TEXTURE_2D, VI6_RndShadowTexId);

  return prg;
} /* End of 'VI6_RndMtlApply' function */

/* Get default material function
 * ARGUMENTS: None.
 * RETURNS: Default material
 */
vi6MATERIAL VI6_RndMtlGetDef( VOID )
{
  return VI6_RndMaterials[0];
} /* End of 'VI6_RndMtlGetDef' */

/* Give material pointer from material number in stock function
 * ARGUMENTS:
 *   - Material number:
 *      - INT MtlNo;
 * RETURNS:
 *   - Material pointer:
 *      - vi6MATERIAL *.
 */
vi6MATERIAL * VI6_RndMtlGet( INT MtlNo )
{
  /* Get material pointer */
  if (MtlNo < 0 || MtlNo >= VI6_RndMaterialsSize)
    MtlNo = 0;
  return &VI6_RndMaterials[MtlNo];
} /* End of 'VI6_RndMtlGet' function */

/* END OF 'RNDMTL.C' FILE */