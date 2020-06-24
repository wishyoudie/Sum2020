/* FILE NAME: RND.H
 * PROGRAMMER: VI6
 * DATE: 06.06.2020
 * PURPOSE: 3D animation project.
 *          Render declaration module.
 */

#ifndef __RND_H_
#define __RND_H_

#define GLEW_STATIC
#include "glew.h"

#include "../../DEF.H"


/* Render global data */
extern HWND VI6_hRndWnd;                     /* Work window handle */
extern HDC VI6_hRndDC;                       /* Work window memory device context  */
extern INT VI6_RndFrameW, VI6_RndFrameH;     /* Work window size */
extern HGLRC VI6_hRndGLRC;                   /* OpenGL rendering context */
extern INT VI6_RndProgId;                    /* Current shader ID */

/* Game stage controls */
extern INT VI6_GameStage;                 /* Current stage of the game */
extern BOOL VI6_IsChickReady;             /* Stage 2 ready flag */
extern INT VI6_DayTime;                   /* Current time of day */

/* Camera/projection parameters */
extern DBL
  VI6_RndProjSize,  /* Project plane fit square */
  VI6_RndProjDist,  /* Distance to project plane from viewer (near) */
  VI6_RndProjFarClip;  /* Distance to project far clip plane (far) */

/* Matrixes */
extern MATR
  VI6_RndMatrView, /* View coordinate system matrix */
  VI6_RndMatrProj, /* Projection coordinate system matrix */
  VI6_RndMatrVP;   /* Stored (View * Proj) matrix */

/* Camera parameters */
extern VEC
  VI6_RndCamLoc,   /* Camera location */
  VI6_RndCamRight, /* Camera right direction */
  VI6_RndCamUp,    /* Camera up direction */
  VI6_RndCamDir,   /* Camera forward direction */
  VI6_RndCamAt;    /* Camera at direction */

/* Shadows parameters */
extern INT VI6_RndShadowTexId;
extern MATR VI6_RndShadowMatr;
extern BOOL VI6_RndShadowPassFlag;
extern VEC 
  VI6_RndLightDir,
  VI6_RndLightColor;

/* Additional parameters for shaders */
extern FLT VI6_RndShdAddon0, VI6_RndShdAddon1, VI6_RndShdAddon2;

typedef struct tagvi6PRIMS
{
  INT NumOfPrims; /* Number of primitives in array */  
  vi6PRIM *Prims; /* Array of primitives */
  MATR Trans;     /* Common transformation matrix */
} vi6PRIMS;

/***
* Rendering functions
***/

VOID VI6_RndInit( HWND hWnd );
VOID VI6_RndClose( VOID );
VOID VI6_RndResize( INT W, INT H );
VOID VI6_RndCopyFrame( VOID );
VOID VI6_RndStart( VOID );
VOID VI6_RndEnd( VOID );
VOID VI6_RndProjSet( VOID );
VOID VI6_RndCamSet( VEC Loc, VEC At, VEC Up );


/***
 * Primitives functions
 ***/

/* Basic functions */
BOOL VI6_RndPrimCreate( vi6PRIM *Pr, vi6PRIM_TYPE Type, vi6VERTEX *V, INT NumOfV, INT *I, INT NumOfI );
VOID VI6_RndPrimFree( vi6PRIM *Pr );
VOID VI6_RndPrimDraw( vi6PRIM *Pr, MATR World );
BOOL VI6_RndPrimLoad( vi6PRIM *Pr, CHAR *FileName );
BOOL VI6_RndPrimCreateFromGrid( vi6PRIM *Pr, vi6VERTEX *V, INT W, INT H, BOOL IsNormalsNeed );

/* Specific shapes */
BOOL VI6_RndPrimSphereCreate( vi6PRIM *Pr, VEC C, DBL R, INT SplitW, INT SplitH );
BOOL VI6_RndPrimTorusCreate( vi6PRIM *Pr, VEC C, DBL R1, DBL R2, INT SplitW, INT SplitH );
BOOL VI6_RndPrimIcosahedronCreate( vi6PRIM *Pr );

/***
 * G3DM files functions
***/
BOOL VI6_RndPrimsCreate( vi6PRIMS *Prs, INT NumOfPrims );
VOID VI6_RndPrimsFree( vi6PRIMS *Prs );
VOID VI6_RndPrimsDraw( vi6PRIMS *Prs, MATR World );
BOOL VI6_RndPrimsLoad( vi6PRIMS *Prs, CHAR *FileName );

/***
 * Shadows functions
***/

VOID VI6_RndLightInit( VOID );
VOID VI6_RndLightClose( VOID );
VOID VI6_RndLightShadow( VOID );

#endif /* __RND_H_ */

/* END OF 'RND.H' FILE */