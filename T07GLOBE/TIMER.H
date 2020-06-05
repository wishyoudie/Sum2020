/* FILE NAME: GLOBE.H
 * PROGRAMMER VI6
 * DATE: 04.06.2020
 * PURPOSE: WinAPI Globe Drawing application timer header
 */

/* Global timer data */
extern DBL GLB_Time, GLB_DeltaTime;
extern DBL GLB_FPS;
extern BOOL GLB_IsPause;

VOID TimerInit( VOID );
VOID TimerResponse( VOID );