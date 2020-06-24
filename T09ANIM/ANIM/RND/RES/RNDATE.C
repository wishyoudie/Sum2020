/* FILE NAME: RNDATE.C
 * PROGRAMMER: VI6
 * DATE: 17.06.2020
 * PURPOSE: 3D Animation project.
            Working with fonts functions.
 */

#include "rndres.h"

/* Font description structure */
typedef struct tavi6FONT
{
  DWORD LineH, BaseH;
  FLT AdvanceX[256];
} vi6FONT;

/* Current font description */

/* Font data */
static vi6FONT VI6_RndFntFont;

/* Font character primitives */
static vi6PRIM VI6_RndFntChars[256];

/* Font material */
static INT VI6_RndFntMtlNo;

/* Load font from .G3DF file function
 * ARGUMETNS:
 *   - font file name:
 *        - CHAR *FileName;
 * RETURNS: TRUE/FALSE.
 */
BOOL VI6_RndFntLoad( CHAR *FileName )
{
  FILE *F;
  DWORD Sign, W, H, *tex;
  vi6VERTEX Chars[256][4];
  INT i;
  vi6MATERIAL mtl;

  if ((F = fopen(FileName, "rb")) == NULL)
    return FALSE;
  fread(&Sign, 4, 1, F);
  if (Sign != *(DWORD *)"G3DF")
  {
    fclose(F);
    return FALSE;
  }
  fread(&VI6_RndFntFont, sizeof(vi6FONT), 1, F);
  fread(&Chars, sizeof(vi6VERTEX), 4 * 256, F);
  fread(&W, 4, 1, F);
  fread(&H, 4, 1, F);

  mtl = VI6_RndMtlGetDef();
  strncpy(mtl.Name, FileName, VI6_STR_MAX - 1);
  if ((tex = malloc(W * H * 4)))
  {
    fread(tex, 4, W * H, F);
    mtl.Tex[0] = VI6_RndTexAddImg(FileName, W, H, 4, tex);
    free(tex);
  }
  mtl.ShdNo = VI6_RndShdAdd("FONT");
  VI6_RndFntMtlNo = VI6_RndMtlAdd(&mtl);
  for (i = 0; i < 256; i++)
  {
    VI6_RndPrimFree(&VI6_RndFntChars[i]);
    VI6_RndPrimCreate(&VI6_RndFntChars[i], VI6_RND_PRIM_TRISTRIP, Chars[i], 4, NULL, 0);
    VI6_RndFntChars[i].MtlNo = VI6_RndFntMtlNo;
  }

  fclose(F);
  return TRUE;
} /* End of 'VI6_RndFntLoad' function */

/* Init font subsystem function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID VI6_RndFntInit( VOID )
{
  VI6_RndFntLoad("BIN/FONTS/Bookman.g3df");
} /* End of 'VI6_RndFntInit' function */

/* Close font subsystem function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID VI6_RndFntClose( VOID )
{
  INT i;

  for (i = 0; i < 256; i++)
    VI6_RndPrimFree(&VI6_RndFntChars[i]);
  memset(VI6_RndFntChars, 0, sizeof(VI6_RndFntChars));
} /* End of 'VI6_RndFntClose' function */

/* Draw string function.
 * ARGUMENTS:
 *   - String to draw:
 *      - CHAR *Str;
 * RETURNS: None.
 */
VOID VI6_RndFntDraw( CHAR *Str )
{
  VEC Pos = {0, 0, 0};

  while (*Str != 0)
  {
    if (*Str == '\n')
    {
      Pos.Y -= 1;
      Pos.X = 0;
    } 
    else
      if (VI6_RndFntFont.AdvanceX[(BYTE)*Str] != 0)
      {
        VI6_RndPrimDraw(&VI6_RndFntChars[(BYTE)*Str], MatrTranslate(Pos));
        Pos.X += VI6_RndFntFont.AdvanceX[(BYTE)*Str];
      }
      Str++;
  }
} /* End of 'VI6_RndFntDraw' function */