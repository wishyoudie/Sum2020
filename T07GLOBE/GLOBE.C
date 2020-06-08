/* FILE NAME: GLOBE.C
 * PROGRAMMER VI6
 * DATE: 04.06.2020
 * PURPOSE: WinAPI Globe Drawing application functions
 */
#include <math.h>
#include <time.h>

#include <windows.h>

#include "GLOBE.H"
#include "TIMER.H"
#include "MTH.H"

/* Geometry data array */
static VEC Geom[GLOBE_H][GLOBE_W];
/* Globe center */
static INT CenterX, CenterY;

/* Rotate over Z axis function
 * ARGUMENTS:
 *   - Vector V to be rotated;
 *   - Angle to rotate;
 * RETURNS: 
 *   - Rotated vector r.
 */ 
VEC VecRotateZ ( VEC V, DBL Angle )
{
  VEC r;

  Angle *= PI / 180;
  r.X = V.X * cos(Angle) - V.Y * sin(Angle);
  r.Y = V.X * sin(Angle) + V.Y * cos(Angle);
  r.Z = V.Z;

  return r;
} /* End of 'VecRotateZ' function */

/* Rotate over X axis function
 * ARGUMENTS:
 *   - Vector V to be rotated;
 *   - Angle to rotate;
 * RETURNS: 
 *   - Rotated vector r.
 */
VEC VecRotateX ( VEC V, DBL Angle )
{
  VEC r;

  Angle *= PI / 180;
  r.Y = V.Y * cos(Angle) - V.Z * sin(Angle);
  r.Z = V.Y * sin(Angle) + V.Z * cos(Angle);
  r.X = V.X;

  return r;
} /* End of 'VecRotateX' function */

/* Set dots coordinates
 * ARGUMENTS:
 *   - globe center coordinates:
 *       INT Xc, Yc;
 *   - globe radius:
 *       DBL R;
 * RETURNS: None.
 */
VOID GlobeSet( DBL Xc, DBL Yc, DBL R )
{
  INT i, j;
  DBL phi, theta;

  CenterX = (INT)Xc;
  CenterY = (INT)Yc;
  for (i = 0, theta = 0; i < GLOBE_H; i++, theta += (PI / (GLOBE_H - 1)))
    for (j = 0, phi = 0; j < GLOBE_W; j++, phi += (2 * PI / (GLOBE_W - 1)))
    {
      Geom[i][j].X = R * sin(theta) * sin(phi);
      Geom[i][j].Y = R * cos(theta);
      Geom[i][j].Z = R * sin(theta) * cos(phi);
    }
} /* End of 'GlobeSet' function */

/* Draw the globed
 * ARGUMENTS:
 *   - HDC:
 *       hDC;
 * RETURNS: None.
 */
VOID GlobeDraw( HDC hDC )
{
  INT i, j, k, s = 3;
  static POINT pnts[GLOBE_H][GLOBE_W];
  MATR m;
  DBL Ws = CenterX * 2, Hs = CenterY * 2, Wp = 1.0, Hp = 1.0, ProjDist = 1.0;
  DBL Size = 1.0;

  if (Ws > Hs)
    Wp = Size * Ws / Hs, Hp = Size;
  else
    Hp = Size * Hs / Ws, Wp = Size;

  m = MatrMulMatr4(
                   MatrRotateX(30),
                   MatrRotateY(GLB_Time * 102), 
                   MatrView(VecSet(sin(GLB_Time * 2.26) * 3, sin(GLB_Time * 2) * 5, 5), VecSet(0, 0, 0), VecSet(0, 1, 0)),
                   //MatrView(VecSet(GLB_Time * 2.26, GLB_Time * 2, 5), VecSet(0, 0, 0), VecSet(0, 1, 0)),
                   MatrFrustum(-Wp / 2, Wp / 2, -Hp / 2, Hp / 2, Size, 300)
                   );
  
  /* Recalculate screen projection for all points */
  for (i = 0; i < GLOBE_H; i++)
    for (j = 0; j < GLOBE_W; j++)
    {
      VEC V;

      /* WorldViewProj Transformation */
      V = VecMulMatr(Geom[i][j], m);

      /* Parallel projection */
      pnts[i][j].x = (INT)((V.X + 1) * Ws / 2.0);
      pnts[i][j].y = (INT)((-V.Y + 1) * Hs / 2.0);
    }

  SelectObject(hDC, GetStockObject(DC_BRUSH));
  SetDCBrushColor(hDC, RGB(55, 146, 243)); 
  SelectObject(hDC, GetStockObject(BLACK_PEN));

  for (k = 1; k < 2; k++) // with back: for k = 0
  {
    for (i = 0; i < GLOBE_H - 1; i++)
      for (j = 0; j < GLOBE_W - 1; j++)
      {
        INT sign;
        POINT p[4];

        p[0] = pnts[i][j];
        p[1] = pnts[i][j + 1];
        p[2] = pnts[i + 1][j + 1];
        p[3] = pnts[i + 1][j];

        sign = 
          (p[0].x - p[1].x) * (p[0].y + p[1].y) +
          (p[1].x - p[2].x) * (p[1].y + p[2].y) +
          (p[2].x - p[3].x) * (p[2].y + p[3].y) +
          (p[3].x - p[0].x) * (p[3].y + p[0].y);
        if (/* k == 0 && sign <= 0 || */k == 1 && sign > 0)
          Polygon(hDC, p, 4);
      }

      SetDCPenColor(hDC, RGB(0, 0, 0));
      SelectObject(hDC, GetStockObject(NULL_BRUSH));
  }
} /* End of 'GlobeDraw' function */

/* END OF 'GLOBE.C' function */