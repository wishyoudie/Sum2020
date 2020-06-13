/* FILE NAME: RNDBASE.C
 * PROGRAMMER: VI6
 * DATE: 06.06.2020
 * PURPOSE: 3D animation project.
 *          Render system base functions module.
 */

#include <time.h>
#include "RND.H"

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
  glClearColor(0.30, 0.50, 0.8, 1);
  glEnable(GL_DEPTH_TEST);
  
  glEnable(GL_PRIMITIVE_RESTART);
  glPrimitiveRestartIndex(-1);

  VI6_RndFrameW = 47;
  VI6_RndFrameH = 47;

  VI6_RndProjSize = VI6_RndProjDist = 0.1;
  VI6_RndProjFarClip = 300;

  VI6_RndProjSet();
  VI6_RndCamSet(VecSet(3, 3, 3), VecSet(0, 0, 0), VecSet(0, 1, 0));

  
} /* End of 'VI6_RndInit' function */

/* Render close function
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID VI6_RndClose( VOID )
{
  VI6_RndShdDelete(VI6_RndProgId);
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
  ///SwapBuffers(VI6_hRndDC);
  wglSwapLayerBuffers(VI6_hRndDC, WGL_SWAP_MAIN_PLANE);
} /* End of 'VI6_RndCopyFrame' function */

/* Start render function
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID VI6_RndStart( VOID )
{
  INT t = clock();
  static INT old_time;
  /* Clear background */
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  if (t - old_time > CLOCKS_PER_SEC)
  {
    VI6_RndShdDelete(VI6_RndProgId);
    VI6_RndProgId = VI6_RndShdLoad("DEFAULT");
    old_time = t;
  }
  
  /* Set draw parameteres */
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
} /* End of 'VI6_RndCamSet' function */

/* END OF 'RNDBASE.C' FILE */