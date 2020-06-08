/* FILE NAME: RNDBASE.C
 * PROGRAMMER: VI6
 * DATE: 06.06.2020
 * PURPOSE: 3D animation project.
 *          Render system base functions module.
 */

#include "RND.H"

/* Render initiate function
 * ARGUMENTS:
 *   - Window:
 *       HWND hWnd;
 * RETURNS: None.
 */
VOID VI6_RndInit( HWND hWnd )
{
  HDC hDC;

  /* Store window handle */
  VI6_hRndWnd = hWnd;

  /* Create buffer */
  hDC = GetDC(hWnd);
  VI6_hRndDCFrame = CreateCompatibleDC(hDC);
  ReleaseDC(hWnd, hDC);
  VI6_hRndBmFrame = NULL;

  VI6_RndFrameW = 47;
  VI6_RndFrameH = 47;

  VI6_RndProjSize = VI6_RndProjDist = 0.1;
  VI6_RndProjFarClip = 300;

  VI6_RndProjSet();
  VI6_RndCamSet(VecSet(3, 3, 3), VecSet(0, 0, 0), VecSet(0, 1, 0));
} /* End of 'VI6_RndInit' function */

/* Render close function
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID VI6_RndClose( VOID )
{
  DeleteDC(VI6_hRndDCFrame);
  DeleteObject(VI6_hRndBmFrame);
} /* End of 'VI6_RndClose' function */

/* Window resize function
 * ARGUMENTS:
 *   - Window:
 *       HWND hWnd;
 * RETURNS: None.
 */
VOID VI6_RndResize( INT W, INT H )
{
  HDC hDC = GetDC(VI6_hRndWnd);

  if (VI6_hRndBmFrame)
    DeleteObject(VI6_hRndBmFrame);
  VI6_hRndBmFrame = CreateCompatibleBitmap(hDC, W, H);
  ReleaseDC(VI6_hRndWnd, hDC);		
  SelectObject(VI6_hRndDCFrame, VI6_hRndBmFrame);

  /* Save size */
  VI6_RndFrameW = W;
  VI6_RndFrameH = H;

  /* Recalculate projection */
  VI6_RndProjSet();
} /* End of 'VI6_RndResize' function */

/* Frame copy function
 * ARGUMENTS:
 *   - Window:
 *       HWND hWnd;
 * RETURNS: None.
 */
VOID VI6_RndCopyFrame( HDC hDC )
{
  BitBlt(hDC, 0, 0, VI6_RndFrameW, VI6_RndFrameH,
    VI6_hRndDCFrame, 0, 0, SRCCOPY);
} /* End of 'VI6_RndCopyFrame' function */

/* Start render function
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID VI6_RndStart( VOID )
{
 SelectObject(VI6_hRndDCFrame, GetStockObject(WHITE_BRUSH));
 SelectObject(VI6_hRndDCFrame, GetStockObject(NULL_PEN));
 Rectangle(VI6_hRndDCFrame, 0, 0, VI6_RndFrameW + 1, VI6_RndFrameH + 1); 

 SelectObject(VI6_hRndDCFrame, GetStockObject(NULL_BRUSH));
 SelectObject(VI6_hRndDCFrame, GetStockObject(BLACK_PEN));
} /* End of 'VI6_RndStart' function */

/* End render function
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID VI6_RndEnd( VOID )
{
} /* End of 'VI6_RndEnd' function */

/* Render projection setting function
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID VI6_RndProjSet( VOID )
{
  DBL rx, ry;

  rx = ry = VI6_RndProjSize;

  /* Correct aspect ration */
  if (VI6_RndFrameW > VI6_RndFrameH)
    rx *= (DBL)VI6_RndFrameW / VI6_RndFrameH;
  else
    ry *= (DBL)VI6_RndFrameH / VI6_RndFrameW;


  VI6_RndMatrProj =
    MatrFrustum(-rx / 2, rx / 2, -ry / 2, ry / 2,
      VI6_RndProjDist, VI6_RndProjFarClip);
  VI6_RndMatrVP = MatrMulMatr(VI6_RndMatrView, VI6_RndMatrProj);
} /* End of 'VI6_RndProjSet' function */

/* Render camera setting function
 * ARGUMENTS:
 *   - Where-to-look parameters:
 *      VEC Loc, VEC At, VEC Up;
 * RETURNS: None.
 */
VOID VI6_RndCamSet( VEC Loc, VEC At, VEC Up )
{
  VI6_RndMatrView = MatrView(Loc, At, Up);
  VI6_RndMatrVP = MatrMulMatr(VI6_RndMatrView, VI6_RndMatrProj);
} /* End of 'VI6_RndCamSet' function */

/* END OF 'RNDBASE.C' FILE */