/* FILE NAME: TIMER.C
 * PROGRAMMER VI6
 * DATE: 04.06.2020
 * PURPOSE: WinAPI Globe Drawing application timer
 */

#include "ANIM/ANIM.H"

/* Local timer data */
static UINT64
    StartTime,    /* Start program time */
    OldTime,      /* Previous frame time */
    OldTimeFPS,   /* Old time FPS measurement */
    PauseTime,    /* Time during pause period */
    TimePerSec,   /* Timer resolution */
    FrameCounter; /* Frames counter */

/* Timer initialization function
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID VI6_TimerInit( VOID )
{
  LARGE_INTEGER t;

  QueryPerformanceFrequency(&t);
  TimePerSec = t.QuadPart;
  QueryPerformanceCounter(&t);
  StartTime = OldTime = OldTimeFPS = t.QuadPart;
  FrameCounter = 0;
  VI6_Anim.IsPause = FALSE;
  VI6_Anim.FPS = 30.0;
  PauseTime = 0;
} /* End of 'VI6_TimerInit' function */

/* Timer response function
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID VI6_TimerResponse( VOID )
{
  LARGE_INTEGER t;

  QueryPerformanceCounter(&t);
  /* Global time */
  VI6_Anim.GlobalTime = (DBL)(t.QuadPart - StartTime) / TimePerSec;
  VI6_Anim.GlobalDeltaTime = (DBL)(t.QuadPart - OldTime) / TimePerSec;

  /* Time with pause */
  if (VI6_Anim.IsPause)
  {
    VI6_Anim.DeltaTime = 0;
    PauseTime += t.QuadPart - OldTime;
  }
  else
  {
    VI6_Anim.DeltaTime = VI6_Anim.GlobalDeltaTime;
    VI6_Anim.Time = (DBL)(t.QuadPart - PauseTime - StartTime) / TimePerSec;
  }
  /* FPS */
  FrameCounter++;
  if (t.QuadPart - OldTimeFPS > TimePerSec)
  {
    VI6_Anim.FPS = FrameCounter * TimePerSec / (DBL)(t.QuadPart - OldTimeFPS);
    OldTimeFPS = t.QuadPart;
    FrameCounter = 0;
  }
  OldTime = t.QuadPart;
} /* End of 'VI6_TimerResponse' function */
/* END OF TIMER.C FILE */