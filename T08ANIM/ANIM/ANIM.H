/* FILE NAME: ANIM.H
 * PROGRAMMER VI6
 * DATE: 09.06.2020
 * PURPOSE: 3D Animation functions header file.
 */

#ifndef __ANIM_H_
#define __ANIM_H_

#include "RND/RND.H"
#define VI6_MAX_UNITS 3000
typedef struct tagvi6UNIT vi6UNIT;
typedef struct tagvi6ANIM
{
  HWND hWnd;
  HDC hDC;
  INT W, H;
  vi6UNIT *Units[VI6_MAX_UNITS];
  INT NumOfUnits;
  DBL
    GlobalTime, GlobalDeltaTime, /* Global time and interframe interval */
    Time, DeltaTime,             /* Time with pause and interframe interval */
    FPS;                         /* Frames per second value */
  BOOL
    IsPause;                     /* Pause flag */
  BYTE
    Keys[256],
    KeysOld[256],
    KeysClick[256];
  INT
    Mx, My, Mz,
    Mdx, Mdy, Mdz;
  BYTE
    JBut[32], JButOld[32], JButClick[32]; /* Joystick button states */
  INT JPov;                               /* Joystick point-of-view control [-1,0..7] */
  DBL
    JX, JY, JZ, JR;                       /* Joystick axes */
} vi6ANIM;

#define UNIT_BASE_FIELDS \
  VOID (*Init)( vi6UNIT *Uni, vi6ANIM *Ani );      \
  VOID (*Close)( vi6UNIT *Uni, vi6ANIM *Ani );     \
  VOID (*Response)( vi6UNIT *Uni, vi6ANIM *Ani );  \
  VOID (*Render)( vi6UNIT *Uni, vi6ANIM *Ani )

struct tagvi6UNIT
{
  UNIT_BASE_FIELDS;
};

extern vi6ANIM VI6_Anim;

VOID VI6_AnimInit( HWND hWnd );
VOID VI6_AnimClose( VOID );
VOID VI6_AnimResize( INT W, INT H );
VOID VI6_AnimCopyFrame( HDC hDC );
VOID VI6_AnimRender( VOID );
VOID VI6_AnimUnitAdd( vi6UNIT *Uni );
VOID VI6_AnimFlipFullScreen( VOID );
VOID VI6_AnimExit( VOID );
vi6UNIT * VI6_AnimUnitCreate( INT Size );

VOID VI6_TimerInit( VOID );
VOID VI6_TimerResponse( VOID );

VOID VI6_AnimInputInit( VOID );
VOID VI6_AnimInputResponse( VOID );

#endif /*__ANIM_H_ */

/* END OF 'ANIM.H' FILE */