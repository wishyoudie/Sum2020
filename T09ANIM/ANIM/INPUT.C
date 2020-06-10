/* FILE NAME: INPUT.C
 * PROGRAMMER VI6
 * DATE: 09.06.2020
 * PURPOSE: 3D Animation input devices reactions functions.
 */

#include "ANIM.H"
#include <mmsystem.h>

#pragma comment(lib, "winmm")

#define VI6_GET_JOYSTIC_AXIS(A) \
   (2.0 * (ji.dw ## A ## pos – jc.w ## A ## min) / (jc.w ## A ## max – jc.w ## A ## min) - 1)

INT VI6_MouseWheel;

static VOID VI6_AnimKeyboardInit( VOID )
{
  GetKeyboardState(VI6_Anim.Keys);
}
static VOID VI6_AnimKeyboardResponse( VOID )
{
  INT i;

  GetKeyboardState(VI6_Anim.Keys);
  for (i = 0; i < 256; i++)
  {
    VI6_Anim.Keys[i] >>= 7;
    VI6_Anim.KeysClick[i] = VI6_Anim.Keys[i] && VI6_Anim.KeysOld[i];
  }

  memcpy(VI6_Anim.KeysOld, VI6_Anim.Keys, 256);
}

static VOID VI6_AnimMouseInit( VOID )
{
  POINT pt;

  GetCursorPos(&pt);
  ScreenToClient(VI6_Anim.hWnd, &pt);

  /* Delta screen coordinates */
  VI6_Anim.Mdx = 0;
  VI6_Anim.Mdy = 0;
  VI6_Anim.Mdz = 0;
  /* Absolute values */
  VI6_Anim.Mx = pt.x;
  VI6_Anim.My = pt.y;
  VI6_Anim.Mz = 0;
  VI6_MouseWheel = 0;
}
static VOID VI6_AnimMouseResponse( VOID )
{
  POINT pt;

  GetCursorPos(&pt);
  ScreenToClient(VI6_Anim.hWnd, &pt);

  /* Delta screen coordinates */
  VI6_Anim.Mdx = pt.x - VI6_Anim.Mx;
  VI6_Anim.Mdy = pt.y - VI6_Anim.My;
  VI6_Anim.Mdz = VI6_MouseWheel;
  /* Absolute values */
  VI6_Anim.Mx = pt.x;
  VI6_Anim.My = pt.y;
  VI6_Anim.Mz += VI6_MouseWheel;
  VI6_MouseWheel = 0;
}

/* static VOID VI6_AnimJoystickInit( VOID );
static VOID VI6_AnimJoystickResponse( VOID )
{
  INT i;
  /* Joystick /
  if (joyGetNumDevs() > 0)
  {
    JOYCAPS jc;

    /* Get joystick info /
    if (joyGetDevCaps(JOYSTICKID1, &jc, sizeof(jc)) == JOYERR_NOERROR)
    {
      JOYINFOEX ji;

      ji.dwSize = sizeof(JOYINFOEX);
      ji.dwFlags = JOY_RETURNALL;
      if (joyGetPosEx(JOYSTICKID1, &ji) == JOYERR_NOERROR)
      {
        /* Buttons /
        for (i = 0; i < 32; i++)
        {
          VI6_Anim.JBut[i] = (ji.dwButtons >> i) & 1;
          VI6_Anim.JButClick[i] = VI6_Anim.JBut[i] && !VI6_Anim.JButOld[i];
          VI6_Anim.JButOld[i] = VI6_Anim.JBut[i];
        }
        /* Axes /
        VI6_Anim.JX = VI6_GET_JOYSTIC_AXIS(X);
        VI6_Anim.JY = VI6_GET_JOYSTIC_AXIS(Y);
        VI6_Anim.JZ = VI6_GET_JOYSTIC_AXIS(Z);
        VI6_Anim.JR = VI6_GET_JOYSTIC_AXIS(R);
        /* Point of view /
        VI6_Anim.JPov = ji.dwPOV == 0xFFFF ? -1 : ji.dwPOV / 4500;
      }
    }
  }
}
*/
VOID VI6_AnimInputInit( VOID )
{
  VI6_AnimMouseInit();
  VI6_AnimKeyboardInit();
}
VOID VI6_AnimInputResponse( VOID )
{
  VI6_AnimMouseResponse();
  VI6_AnimKeyboardResponse();
}
/* END OF 'INPUT.C' FILE */