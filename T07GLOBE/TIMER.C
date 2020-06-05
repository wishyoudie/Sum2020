/* FILE NAME: TIMER.C
 * PROGRAMMER VI6
 * DATE: 04.06.2020
 * PURPOSE: WinAPI Globe Drawing application timer
 */

#include <time.h>
#include <windows.h>
#include "GLOBE.H"

/* Global timer data */
DBL GLB_Time, GLB_DeltaTime;
DBL GLB_FPS = 30.0;
BOOL GLB_IsPause = FALSE;

/* Local data */
static LONG
  StartTime = -1, PauseTime = 0, OldTime, OldFPSTime, FrameCount;

/* Timer initialization function
 * ARGUMENTS: None.
 * RETURNS: None.    
 */
VOID TimerInit( VOID )
{
  StartTime = PauseTime = OldTime = OldFPSTime = clock();
  FrameCount = 0;
  GLB_IsPause = FALSE;
  GLB_FPS = 30.0;
} /* End of 'TimerInit' function */

/* Timer initialization function
 * ARGUMENTS: None.
 * RETURNS:
 *   - FPS:
 *      DBL FPS.
 */
VOID TimerResponse( VOID )
{
  LONG t = clock();

  if (!GLB_IsPause)
  {
    GLB_Time = (DBL)(t - PauseTime - StartTime) / CLOCKS_PER_SEC;
    GLB_DeltaTime = (DBL)(t - OldTime) / CLOCKS_PER_SEC;
  }
  else
  {
    PauseTime += t - OldTime;
    GLB_DeltaTime = 0;
  }

  FrameCount++;
  
  if (t - OldFPSTime > CLOCKS_PER_SEC)
  {
    GLB_FPS = FrameCount * (CLOCKS_PER_SEC / (DBL)(t - OldFPSTime));
    OldFPSTime = t;
    FrameCount = 0;
  }
  OldTime = t;
} /* End of 'TimerResponse' function */

/* END OF TIMER.C FILE */