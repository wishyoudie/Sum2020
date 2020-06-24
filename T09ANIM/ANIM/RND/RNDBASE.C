/* FILE NAME: RNDBASE.C
 * PROGRAMMER: VI6
 * DATE: 06.06.2020
 * PURPOSE: 3D animation project.
 *          Render system base functions module.
 */

#include <time.h>
#include "RND.H"
#include "res/rndres.h"
/* Link libraries */
#pragma comment(lib, "opengl32")

/* Render initiate function
 * ARGUMENTS:
 *   - Window:
 *       HWND hWnd;
 * RETURNS: None.
 */
VOID VI6_RndInit( HWND hWnd )
{
  INT i;
  PIXELFORMATDESCRIPTOR pfd = {0};

  /* Store window handle */
  VI6_hRndWnd = hWnd;

  /* Obtain work window device context */
  VI6_hRndDC = GetDC(hWnd);

  /* OpenGL initialization */
  pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
  pfd.nVersion = 1;
  pfd.dwFlags = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL;
  pfd.cColorBits = 32;
  pfd.cDepthBits = 32;

  i = ChoosePixelFormat(VI6_hRndDC, &pfd);
  DescribePixelFormat(VI6_hRndDC, i, sizeof(pfd), &pfd);
  SetPixelFormat(VI6_hRndDC, i, &pfd);

  /* OpenGL init: setup rendering context */
  VI6_hRndGLRC = wglCreateContext(VI6_hRndDC);
  wglMakeCurrent(VI6_hRndDC, VI6_hRndGLRC);
  if (glewInit() != GLEW_OK ||
        !(GLEW_ARB_vertex_shader && GLEW_ARB_fragment_shader))
  {
    MessageBox(hWnd, "Error OpenGL initialization", "ERROR",
      MB_ICONERROR | MB_OK);
    exit(0);
  }

  /* Set default render parametres */
  glClearColor(0.30, 0.50, 0.8, 1); //day
  ///glClearColor(0.965, 0.278, 0.278, 1); //sunset
  ///glClearColor(0, 0, 0, 1); //night
  glEnable(GL_DEPTH_TEST);
  
  glEnable(GL_PRIMITIVE_RESTART);
  glPrimitiveRestartIndex(-1);

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  VI6_RndFrameW = 47;
  VI6_RndFrameH = 47;

  VI6_RndProjSize = VI6_RndProjDist = 0.1;
  VI6_RndProjFarClip = 300000;

  VI6_RndProjSet();
  //VI6_RndCamSet(VecSet(3, 3, 3), VecSet(0, 0, 0), VecSet(0, 1, 0));

  /* Set first stage */
  VI6_GameStage = 0;
  /* Initialize resources */
  VI6_RndResInit();
  /* Initialize shadows */
  VI6_RndLightInit();
} /* End of 'VI6_RndInit' function */

/* Render close function
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID VI6_RndClose( VOID )
{
  VI6_RndLightClose();
  VI6_RndResClose();
  wglMakeCurrent(NULL, NULL);
  wglDeleteContext(VI6_hRndGLRC);
  ReleaseDC(VI6_hRndWnd, VI6_hRndDC);
} /* End of 'VI6_RndClose' function */

/* Window resize function
 * ARGUMENTS:
 *   - Window:
 *       HWND hWnd;
 * RETURNS: None.
 */
VOID VI6_RndResize( INT W, INT H )
{
  /* Set OpenGL viewport */
  glViewport(0, 0, W, H);

  /* Save size */
  VI6_RndFrameW = W;
  VI6_RndFrameH = H;

  /* Recalculate projection */
  VI6_RndProjSet();
} /* End of 'VI6_RndResize' function */

/* Frame copy function
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID VI6_RndCopyFrame( VOID )
{
  wglSwapLayerBuffers(VI6_hRndDC, WGL_SWAP_MAIN_PLANE);
} /* End of 'VI6_RndCopyFrame' function */

/* Start render function
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID VI6_RndStart( VOID )
{
#ifndef NDEBUG
  INT t;
  static INT reload_time;

  if ((t = clock()) - reload_time > 2 * CLOCKS_PER_SEC)
  {
    VI6_RndShdUpdate();
    reload_time = t;
  }
#endif /* NDEBUG */

  /* Redraw shadows */
  VI6_RndLightShadow();

  /* Clear background */
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

} /* End of 'VI6_RndStart' function */

/* End render function
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID VI6_RndEnd( VOID )
{
  glFinish();
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

  VI6_RndCamLoc = Loc;
  VI6_RndCamAt = At;
  VI6_RndCamRight = VecSet(VI6_RndMatrView.A[0][0], VI6_RndMatrView.A[1][0], VI6_RndMatrView.A[2][0]);
  VI6_RndCamUp = VecSet(VI6_RndMatrView.A[0][1], VI6_RndMatrView.A[1][1], VI6_RndMatrView.A[2][1]);
  VI6_RndCamDir = VecSet(-VI6_RndMatrView.A[0][2], -VI6_RndMatrView.A[1][2], -VI6_RndMatrView.A[2][2]);
} /* End of 'VI6_RndCamSet' function */

/* END OF 'RNDBASE.C' FILE */