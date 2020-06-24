/* FILE NAME: IMAGE.C
 * PROGRAMMER: VI6
 * DATE: 14.06.2020
 * PURPOSE: 3D animation project.
 *          Render system images handle module.
 */

#include "rndres.h"

/* Fill image variable structure function
 * ARGUMENTS:
 *   - Image:
 *      - vi6IMAGE *Im;
 *   - Image size:
 *      - INT W, INT H;
 * RETURNS: TRUE/FALSE.
 */
BOOL VI6_ImgCreate( vi6IMAGE *Im, INT W, INT H )
{
  memset(Im, 0, sizeof(vi6IMAGE));

  if ((Im->Pixels = malloc(W * H * 4)) == NULL)
    return FALSE;
  Im->W = W;
  Im->H = H;
  Im->PixelsDW = (DWORD *)Im->Pixels;
  Im->PixelsC = (BYTE (*)[4])Im->Pixels;
  return TRUE;
} /* End of 'VI6_ImgCreate' function */

/* Load image from BMP file function
 * ARGUMETNS:
 *   - Image pointer:
 *      - vi6IMAGE *Im;
 *   - Image file name:
 *      - CHAR *FileName;
 * RETURNS: TRUE/FALSE.
 */
BOOL VI6_RndImgLoad( vi6IMAGE *Im, CHAR *FileName )
{
  BITMAP bm;
  BYTE *ptr;
  INT x, y, BPL, type = 0, mulx[3] = {1, 3, 4}, rgb[3][3] = {{0, 0, 0}, {0, 1, 2}, {0, 1, 2}};

  Im->hBm = LoadImage(NULL, FileName, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);

  if (Im->hBm == NULL)
  {
    /* Try to read G32 image file */
    FILE *F;

    if ((F = fopen(FileName, "rb")) != NULL)
    {
      INT w = 0, h = 0;

      fread(&w, 2, 1, F);
      fread(&h, 2, 1, F);
      fseek(F, 0, SEEK_END);
      if (ftell(F) == w * h * 4 + 4)
      {
        if (VI6_ImgCreate(Im, w, h))
        {
          rewind(F);
          fread(Im->Pixels, 4, w * h, F);
        }
      }
      fclose(F);
      return TRUE;
    }
    return FALSE;
  }

  GetObject(Im->hBm, sizeof(bm), &bm);
  ptr = bm.bmBits;
  BPL = bm.bmWidthBytes;

  if (!VI6_ImgCreate(Im, bm.bmWidth, bm.bmHeight))
    return FALSE;

  if (bm.bmBitsPixel != 8 && bm.bmBitsPixel != 24 && bm.bmBitsPixel != 32)
  {
    DeleteObject(Im->hBm);
    return FALSE;
  }

  type = bm.bmBitsPixel == 8 ? 0 : bm.bmBitsPixel == 24 ? 1 : 2;

  for (y = 0; y < bm.bmHeight; y++)
    for (x = 0; x < bm.bmWidth; x++)
    {
      BYTE
        b = ptr[bm.bmWidthBytes * y + x * mulx[type] + rgb[type][0]],
        g = ptr[bm.bmWidthBytes * y + x * mulx[type] + rgb[type][1]],
        r = ptr[bm.bmWidthBytes * y + x * mulx[type] + rgb[type][2]],
        a = 255;

      Im->Pixels[(y * Im->W + x) * 4 + 0] = b;
      Im->Pixels[(y * Im->W + x) * 4 + 1] = g;
      Im->Pixels[(y * Im->W + x) * 4 + 2] = r;
      Im->Pixels[(y * Im->W + x) * 4 + 3] = a;
    }
    
  Im->W = bm.bmWidth;
  Im->H = bm.bmHeight;
  return TRUE;
} /* End of 'VI6_RndImgLoad' function */

/* Forget image function.
 * ARGUMENTS:
 *   - Image pointer:
 *      - vi6IMAGE *Im;
 * RETURNS: None.
 */
VOID VI6_RndImgFree( vi6IMAGE *Im )
{
  DeleteObject(Im->hBm);
  memset(Im, 0, sizeof(vi6IMAGE));
} /* End of 'VI6_RndImgFree' function */

/* END OF 'IMAGE.C' FILE */