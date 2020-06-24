/* FILE NAME: ANIM.C
 * PROGRAMMER VI6
 * DATE: 09.06.2020
 * PURPOSE: 3D Animation functions
 */

/* Defines, structures and global variables */

#include "ANIM.H"

/* Global variable */
vi6ANIM VI6_Anim;

/* Functions */

/* Animation system initialization function 
 * ARGUMENTS:
 *   - Work window:
 *      - HWND hWnd;
 * RETURNS: None.
 */
VOID VI6_AnimInit( HWND hWnd )
{
  memset(&VI6_Anim, 0, sizeof(vi6ANIM));

  VI6_Anim.hWnd = hWnd;
  VI6_RndInit(hWnd);
  VI6_Anim.hDC = VI6_hRndDC;

  /* Timer initialization */
  VI6_TimerInit();

  /* Input devices initialization */
  VI6_AnimInputInit();

} /* End of 'VI6_AnimInit' function */

/* Animation system deinitialization function
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID VI6_AnimClose( VOID )
{
  INT i;

  for (i = 0; i < VI6_Anim.NumOfUnits; i++)
  {
    VI6_Anim.Units[i]->Close(VI6_Anim.Units[i], &VI6_Anim);
    free(VI6_Anim.Units[i]);
  }
  VI6_Anim.NumOfUnits = 0;
  
  /* Close render */
  VI6_RndClose();
} /* End of 'VI6_AnimClose' function */

/* Animation system resize window handle function
 * ARGUMENTS:
 *   - Screen width & height:
 *      - INT W, INT H;
 * RETURNS: None.
 */
VOID VI6_AnimResize( INT W, INT H )
{
  VI6_Anim.W = W;
  VI6_Anim.H = H;
  VI6_RndResize(W, H);
} /* End of 'VI6_AnimResize' function */

VOID VI6_AnimCopyFrame( VOID )
{
  VI6_RndCopyFrame();
} /* End of 'VI6_AnimCopyFrame' function */

VOID VI6_AnimRender( VOID )
{
  INT i;

  /* Timer response */
  VI6_TimerResponse();

  /* Input devices Response */
  VI6_AnimInputResponse();

  for (i = 0; i < VI6_Anim.NumOfUnits; i++)
    VI6_Anim.Units[i]->Response(VI6_Anim.Units[i], &VI6_Anim);

  VI6_RndStart();

  for (i = 0; i < VI6_Anim.NumOfUnits; i++)
  {
    SelectObject(VI6_Anim.hDC, GetStockObject(NULL_BRUSH));
    SelectObject(VI6_Anim.hDC, GetStockObject(DC_PEN));
    SetDCPenColor(VI6_Anim.hDC, RGB(55, 155, 255));

    VI6_Anim.Units[i]->Render(VI6_Anim.Units[i], &VI6_Anim);
  }
  VI6_RndEnd();
} /* End of 'VI6_AnimRender' function */

VOID VI6_AnimFlipFullScreen( VOID )
{
  static BOOL IsFullScreen = FALSE;
  static RECT SaveRect;
  HWND hWnd = VI6_Anim.hWnd;
  if (!IsFullScreen)
  {
    HMONITOR hmon;
    MONITORINFO moninfo;
    RECT rc;

    /* Goto fullscreen mode */
    GetWindowRect(hWnd, &SaveRect);

    /* Get nearest monitor */
    hmon = MonitorFromWindow(hWnd, MONITOR_DEFAULTTONEAREST);

    /* Get monitor information */
    moninfo.cbSize = sizeof(moninfo);
    GetMonitorInfo(hmon, &moninfo);

    rc = moninfo.rcMonitor;
    AdjustWindowRect(&rc, GetWindowLong(hWnd, GWL_STYLE), FALSE);
    SetWindowPos(hWnd, HWND_TOP,
      rc.left, rc.top,
      rc.right - rc.left, rc.bottom - rc.top,
      SWP_NOOWNERZORDER);
    IsFullScreen = TRUE;
  }
  else
  {
    /* Restore window size and position */
    SetWindowPos(hWnd, HWND_TOP,
      SaveRect.left, SaveRect.top,
      SaveRect.right - SaveRect.left, SaveRect.bottom - SaveRect.top,
      SWP_NOOWNERZORDER);
    IsFullScreen = FALSE;
  }
} /* End of 'VI6_AnimFlipFullScreen' function */

VOID VI6_AnimExit( VOID )
{
} /* End of 'VI6_AnimExit' function */

VOID VI6_AnimUnitAdd( vi6UNIT *Uni )
{
  if (VI6_Anim.NumOfUnits < VI6_MAX_UNITS)
    VI6_Anim.Units[VI6_Anim.NumOfUnits++] = Uni, Uni->Init(Uni, &VI6_Anim);
} /* End of 'VI6_AnimUnitAdd' function */
/* END OF 'ANIM.C' FILE */