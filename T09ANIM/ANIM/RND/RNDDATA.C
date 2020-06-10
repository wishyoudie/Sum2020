/* FILE NAME: RNDDATA.C
 * PROGRAMMER: VI6
 * DATE: 06.06.2020
 * PURPOSE: 3D animation project.
 *          Render system global module.
 */

#include "RND.H"

/* Render global data */
HWND VI6_hRndWnd;        /* Work window handle */
HDC VI6_hRndDC;     /* Work window memory device context  */
INT VI6_RndFrameW, VI6_RndFrameH; /* Work window size */
HGLRC VI6_hRndGLRC; /* OpenGL rendering context */

/* Camera/projection parameters */
DBL
  VI6_RndProjSize = 0.1,  /* Project plane fit square */
  VI6_RndProjDist = 0.1,  /* Distance to project plane from viewer (near) */
  VI6_RndProjFarClip = 300;  /* Distance to project far clip plane (far) */


MATR
  VI6_RndMatrView, /* View coordinate system matrix */
  VI6_RndMatrProj, /* Projection coordinate system matrix */
  VI6_RndMatrVP;   /* Stored (View * Proj) matrix */

/* Primitives */
vi6VERTEX *V;
INT NumOfV, NumOfI;
INT *I;
/* END OF 'RNDDATA.C' FILE */