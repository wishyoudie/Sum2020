/* FILE NAME: MAIN.C
 * PROGRAMMER VI6
 * DATE: 06.06.2020
 * PURPOSE: 3D animation project main handler.
 *          WinAPI startup module.
 */

#include "../ANIM/RND/RND.H"

 /* Window class name */
#define WND_CLASS_NAME "My window class"

/* Forward declaration */
LRESULT CALLBACK WinFunc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam );


/* The main program function.
 * ARGUMENTS:
 *   - handle of application instance:
 *       HINSTANCE hInstance;
 *   - dummy handle of previous application instance (not used):
 *       HINSTANCE hPrevInstance;
 *   - command line string:
 *       CHAR *CmdLine;
 *   - show window command parameter (see SW_***):
 *       INT CmdShow;
 * RETURNS:
 *   (INT) Error level for operation system (0 for success).
 */
int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, CHAR *CmdLine, INT CmdShow )
{
  HWND hWnd;
  MSG msg;
  WNDCLASS wc;

  SetDbgMemHooks();

  /* Fill window class structure */
  wc.style = CS_HREDRAW | CS_VREDRAW;
  wc.cbClsExtra = 0;
  wc.cbWndExtra = 0;
  wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
  wc.hCursor = LoadCursor(NULL, IDC_ARROW);
  wc.hIcon = LoadIcon(NULL, IDI_SHIELD);
  wc.hInstance = hInstance;
  wc.lpszClassName = WND_CLASS_NAME;
  wc.lpszMenuName = NULL;
  wc.lpfnWndProc = WinFunc;

  /* Register window class */
  if (!RegisterClass(&wc))
  {
    MessageBox(NULL, "Error register window class", "Error", MB_OK | MB_ICONERROR);
    return 0;
  }

  /* Window creation */
  hWnd = CreateWindow(WND_CLASS_NAME, "T08ANIM", WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN, 
    CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
    NULL, NULL, hInstance, NULL);

  ShowWindow(hWnd, CmdShow);

  /* Message loop */
  while (TRUE)
    if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
    {
      if (msg.message == WM_QUIT)
        break;
      DispatchMessage(&msg);
    }
    else
      SendMessage(hWnd, WM_TIMER, 47, 0);

  return 0;
} /* End of 'WinMain' function */

/* Main window handle function
 * ARGUMENTS:
 *   - Window:
 *       HWND hWnd;
 *   - Message:
 *       UINT Msg;
  *   - Parameters:
 *       WPARAM wParam, LPARAM lParam;
 * RETURNS:
 *    - Window:
 *       DefWindowProc(hWnd, Msg, wParam, lParam).
 */
LRESULT CALLBACK WinFunc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
  HDC hDC;
  PAINTSTRUCT ps;
  CHAR Buf[102];
  //static vi6PRIM Sphere;
 //static vi6PRIM Cone;
  static vi6PRIM Cow;
  static vi6PRIM Torus;
  static vi6PRIM Tree;
  static vi6PRIM Bench;

  switch (Msg)
  {
  case WM_GETMINMAXINFO:
    ((MINMAXINFO *)lParam)->ptMaxTrackSize.y =
      GetSystemMetrics(SM_CYMAXTRACK) + GetSystemMetrics(SM_CYCAPTION) + 2 * GetSystemMetrics(SM_CYBORDER);
    return 0;
  case WM_CREATE:
    TimerInit();
    VI6_RndInit(hWnd);

    //VI6_RndPrimSphereCreate(&Sphere, VecSet(0, 0, 0), 2, 36, 18); // Sphere
   //VI6_RndPrimConeCreate(&Cone, VecSet(6, 0, 0), 8 * 0.30, 0.47, 18); // Cone
    VI6_RndPrimTorusCreate(&Torus, VecSet(0, 0, 0), 4, 2, 25, 12); // Torus
    VI6_RndPrimLoad(&Cow, "cow.obj"); // Cow
    VI6_RndPrimLoad(&Tree, "tree.obj"); // Tree
    VI6_RndPrimLoad(&Bench, "bench.obj"); // Bench

    SetTimer(hWnd, 47, 2, NULL);
    return 0;
  case WM_SIZE:
    VI6_RndResize(LOWORD(lParam), HIWORD(lParam));
    return 0;
  case WM_KEYDOWN:
    if (wParam == VK_ESCAPE)
      SendMessage(hWnd, WM_CLOSE, 0, 0);
    return 0;
  case WM_TIMER:
    TimerResponse();
    VI6_RndStart();

    //VI6_RndPrimDraw(&Sphere, MatrRotateY(15 * clock() / 1000.0)); //Sphere
    //VI6_RndPrimDraw(&Cone, MatrScale(VecSet(0.5, fabs(sin(0.08 * clock() / 1000.0)), 0.5))); //Cone
    VI6_RndPrimDraw(&Torus, MatrMulMatr3(MatrScale(VecSet1(0.05)), MatrRotateY(10 * clock() / 1000.0), MatrTranslate(VecSet(-2, 0, 2)))); //Torus
    VI6_RndPrimDraw(&Cow, MatrMulMatr3(MatrScale(VecSet1(0.3)), MatrTranslate(VecSet(-0.5, 0, 1.5)), MatrRotateY(-45))); //Cow

    VI6_RndPrimDraw(&Tree, MatrTranslate(VecSet(1, 0, -1))); //Tree
    VI6_RndPrimDraw(&Bench, MatrScale(VecSet1(0.01))); //Bench

    VI6_RndEnd();
    InvalidateRect(hWnd, NULL, FALSE);
    SetTextColor(VI6_hRndDCFrame, RGB(0, 0, 0));
    SetBkMode(VI6_hRndDCFrame, TRANSPARENT);
    TextOut(VI6_hRndDCFrame, 0, 0, Buf, sprintf(Buf, "FPS: %.3f", GLB_FPS));
    return 0;
  case WM_ERASEBKGND:
    return 1;
  case WM_PAINT:
    hDC = BeginPaint(hWnd, &ps);
    VI6_RndCopyFrame(hDC);
    EndPaint(hWnd, &ps);
    return 0; 
  case WM_CLOSE:
    if (MessageBox(hWnd, "Are you sure?", "Close", MB_YESNO | MB_DEFBUTTON1 | MB_ICONQUESTION) == IDYES)
      DestroyWindow(hWnd);
    return 0;
  case WM_DESTROY:
    VI6_RndPrimFree(&Cow);
    VI6_RndClose();
    KillTimer(hWnd, 47);
    PostQuitMessage(30);
    return 0;
  }
  return DefWindowProc(hWnd, Msg, wParam, lParam);
} /* End of 'WinFunc' function */

/* END OF 'MAIN.C' function */