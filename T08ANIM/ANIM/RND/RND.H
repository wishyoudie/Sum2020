/* FILE NAME: RND.H
 * PROGRAMMER: VI6
 * DATE: 06.06.2020
 * PURPOSE: 3D animation project.
 *          Render declaration module.
 */

#ifndef __RND_H_
#define __RND_H_

#include "../../DEF.H"

/* Render global data */
extern HWND VI6_hRndWnd;        /* Work window handle */
extern HDC VI6_hRndDCFrame;     /* Work window memory device context  */
extern HBITMAP VI6_hRndBmFrame; /* Work window background bitmap handle */
extern INT VI6_RndFrameW, VI6_RndFrameH; /* Work window size */

/* Camera/projection parameters */
extern DBL
  VI6_RndProjSize,  /* Project plane fit square */
  VI6_RndProjDist,  /* Distance to project plane from viewer (near) */
  VI6_RndProjFarClip;  /* Distance to project far clip plane (far) */


extern MATR
  VI6_RndMatrView, /* View coordinate system matrix */
  VI6_RndMatrProj, /* Projection coordinate system matrix */
  VI6_RndMatrVP;   /* Stored (View * Proj) matrix */

/* Rendering functions */
VOID VI6_RndInit( HWND hWnd );
VOID VI6_RndClose( VOID );
VOID VI6_RndResize( INT W, INT H );
VOID VI6_RndCopyFrame( HDC hDC );
VOID VI6_RndStart( VOID );
VOID VI6_RndEnd( VOID );
VOID VI6_RndProjSet( VOID );
VOID VI6_RndCamSet( VEC Loc, VEC At, VEC Up );

/* Primitives functions */

/* Basic functions */
BOOL VI6_RndPrimCreate( vi6PRIM *Pr, INT NoofV, INT NoofI );
VOID VI6_RndPrimFree( vi6PRIM *Pr );
VOID VI6_RndPrimDraw( vi6PRIM *Pr, MATR World );
BOOL VI6_RndPrimLoad( vi6PRIM *Pr, CHAR *FileName );
/* Specific shapes */
VOID VI6_RndPrimSphereCreate( vi6PRIM *Pr, VEC C, DBL R, INT SplitW, INT SplitH );
VOID VI6_RndPrimTorusCreate( vi6PRIM *Pr, VEC C, DBL R1, DBL R2, INT SplitW, INT SplitH );
VOID VI6_RndPrimConeCreate( vi6PRIM *Pr, VEC C, DBL H, DBL R, INT N );

#endif /* __RND_H_ */

/* END OF 'RND.H' FILE */