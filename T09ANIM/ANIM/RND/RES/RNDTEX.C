/* FILE NAME: RNDTEX.C
 * PROGRAMMER: VI6
 * DATE: 12.06.2020
 * PURPOSE: 3D animation project.
 *          Render system texture handle module.
 */

#include "rndres.h"

vi6TEXTURE VI6_RndTextures[VI6_MAX_TEXTURES];    /* Array of textures */
INT VI6_RndTexturesSize;                         /* Textures array store size */

/* Texture storage initialize function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID VI6_RndTexInit( VOID )
{
  VI6_RndTexturesSize = 0;
} /* End of 'VI6_RndTexInit' function */

/* Texture add from image function.
 * ARGUMENTS:
 *   - Image file name:
 *      - CHAR *Name;
 *   - Image sizes:
 *      - INT W, INT H;
 *   - Number of texture pixels color components:
 *      - INT C;
 *   - Pointer to pixels color data:
 *        - VOID *Bits;
 * RETURNS:
 *   - Number of texture in texture array:
 *      - INT VI6_RndTextureSize;
 */
INT VI6_RndTexAddImg( CHAR *Name, INT W, INT H, INT C, VOID *Bits )
{
  if (VI6_RndTexturesSize >= VI6_MAX_TEXTURES)
    return -1;
  
  strncpy(VI6_RndTextures[VI6_RndTexturesSize].Name, Name, VI6_STR_MAX - 1);
  VI6_RndTextures[VI6_RndTexturesSize].W = W;
  VI6_RndTextures[VI6_RndTexturesSize].H = H;
  glGenTextures(1, &VI6_RndTextures[VI6_RndTexturesSize].TexId);
  glBindTexture(GL_TEXTURE_2D, VI6_RndTextures[VI6_RndTexturesSize].TexId);
  glTexImage2D(GL_TEXTURE_2D, 0, C, W, H, 0,
    C == 3 ? GL_BGR : C == 4 ? GL_BGRA : GL_LUMINANCE,
    GL_UNSIGNED_BYTE, Bits);
  glGenerateMipmap(GL_TEXTURE_2D);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

  return VI6_RndTexturesSize++;
} /* End of 'VI6_RndTexAddImg' function */

/* Delete texture from stock function.
 * ARGUMENTS:
 *   - Texture number in stock:
 *      - INT TexNo;
 * RETURNS: None.
 */
VOID VI6_RndTexDelete( INT TexNo )
{
  INT textures[5], n, i;

  if (TexNo == 0)
    return;

  glGetAttachedShaders(TexNo, 5, &n, textures);
  for (i = 0; i < n; i++)
  {
    glDetachShader(TexNo, textures[i]);
    glDeleteShader(textures[i]);
  }
  glDeleteProgram(TexNo);
} /* End of 'VI6_RndTexDelete' function */

/* Texture add from file function.
 * ARGUMENTS:
 *   - File name:
 * RETURNS:
 *   - Number of texture in texture array:
 *      - INT VI6_RndTextureSize;
 */
INT VI6_RndTexAdd( CHAR *FileName )
{
  INT n;
  vi6IMAGE img;

  if (!VI6_RndImgLoad(&img, FileName))
    return -1;
  n = VI6_RndTexAddImg(FileName, img.W, img.H, 4, img.Pixels);
  VI6_RndImgFree(&img);
  return n;
} /* End of 'VI6_RndTexAdd' function */

/* Texture delete from stock function.
 * ARGUMENTS:
 *   - 
/* Texture storage deinitialize function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID VI6_RndTexClose( VOID )
{
  INT i;

  for (i = 0; i < VI6_RndTexturesSize; i++)
    VI6_RndTexDelete(VI6_RndTextures[i].TexId);
  VI6_RndTexturesSize = 0;
} /* End of 'VI6_RndTexClose' function */

/* Add new texture to texture store system by OpenGL format function.
 * ARGUMENTS:
 *   - texture name:
 *       CHAR *Name;
 *   - texture size:
 *       INT W, H;
 *   - OpenGL data type:
 *       INT GLType;
 * RETURNS:
 *   (INT) added texture table number.
 */
INT VI6_RndTexAddFmt( CHAR *Name, INT W, INT H, INT GLType )
{
  /* Check for free space */
  if (VI6_RndTexturesSize >= VI6_MAX_TEXTURES)
    return -1;

  strncpy(VI6_RndTextures[VI6_RndTexturesSize].Name, Name, VI6_STR_MAX - 1);
  VI6_RndTextures[VI6_RndTexturesSize].W = W;
  VI6_RndTextures[VI6_RndTexturesSize].H = H;
  glGenTextures(1, &VI6_RndTextures[VI6_RndTexturesSize].TexId);

  glBindTexture(GL_TEXTURE_2D, VI6_RndTextures[VI6_RndTexturesSize].TexId);

  glTexStorage2D(GL_TEXTURE_2D, 1, GLType, W, H);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  return VI6_RndTexturesSize++;
} /* End of 'VI6_RndTexAddFmt' function */

/* END OF 'RNDTEX.C' FILE */