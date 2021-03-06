/* FILE NAME: MAIN.C
 * PROGRAMMER VI6
 * DATE: 06.06.2020
 * PURPOSE: 3D animation project main handler.
 *          WinAPI startup module.
 */

#include "../UNITS/UNITS.H"

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

  wc.style = CS_HREDRAW | CS_VREDRAW;
  wc.cbClsExtra = 0;
  wc.cbWndExtra = 0;
  wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
  wc.hCursor = LoadCursor(NULL, IDC_CROSS);
  wc.hIcon = LoadIcon(NULL, IDI_SHIELD);
  wc.hInstance = hInstance;
  wc.lpszClassName = WND_CLASS_NAME;
  wc.lpszMenuName = NULL;
  wc.lpfnWndProc = WinFunc;

  if (!RegisterClass(&wc))
  {
    MessageBox(NULL, "Error register window class", "Error", MB_OK | MB_ICONERROR);
    return 0;
  }

  hWnd = CreateWindow(WND_CLASS_NAME, "--=HAPPY CHICK=--", WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN, 
    CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
    NULL, NULL, hInstance, NULL);

  ShowWindow(hWnd, CmdShow);

  VI6_AnimUnitAdd(VI6_UnitCreateEnvi());
  VI6_AnimUnitAdd(VI6_UnitCreateCtrl());
  VI6_AnimUnitAdd(VI6_UnitCreateChick());
  VI6_AnimUnitAdd(VI6_UnitCreateEgg());

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
  extern VI6_MouseWheel;

  switch (Msg)
  {
  case WM_GETMINMAXINFO:
    ((MINMAXINFO *)lParam)->ptMaxTrackSize.y =
      GetSystemMetrics(SM_CYMAXTRACK) + GetSystemMetrics(SM_CYCAPTION) + 2 * GetSystemMetrics(SM_CYBORDER);
    return 0;
  case WM_CREATE:
    VI6_AnimInit(hWnd);
    VI6_AnimFlipFullScreen();
    SetTimer(hWnd, 47, 2, NULL);
    return 0;
  case WM_SIZE:
    VI6_AnimResize(LOWORD(lParam), HIWORD(lParam));
    return 0;
  case WM_KEYDOWN:
    if (wParam == VK_ESCAPE)
      SendMessage(hWnd, WM_CLOSE, 0, 0);
  case WM_SYSKEYDOWN:
    if (wParam == VK_RETURN)
      VI6_AnimFlipFullScreen();
    return 0;
  case WM_SETCURSOR:
    ShowCursor(FALSE);
    return 0;
  case WM_LBUTTONDOWN:
    SetCapture(hWnd);
    return 0;
  case WM_LBUTTONUP:
    ReleaseCapture();
    return 0;
  case WM_MOUSEWHEEL:
    VI6_MouseWheel += (SHORT)HIWORD(wParam);
    return 0;
  case WM_TIMER:
    VI6_AnimRender();
    VI6_RndCopyFrame();
    return 0;
  case WM_ERASEBKGND:
    return 1;
  case WM_PAINT:
    hDC = BeginPaint(hWnd, &ps);
    VI6_AnimCopyFrame();
    EndPaint(hWnd, &ps);
    return 0; 
  case WM_CLOSE:
    if (VI6_GameStage == 2)
    {
      DestroyWindow(hWnd);
      return 0;
    }
    if (MessageBox(hWnd, "Do you really wish to quit without finishing the game?", "--=HAPPY CHICK=--", MB_YESNO | MB_DEFBUTTON1 | MB_ICONERROR) == IDYES)
      DestroyWindow(hWnd);
    return 0;
  case WM_DESTROY:
    VI6_AnimClose();
    KillTimer(hWnd, 47);
    PostQuitMessage(30);
    return 0;
  }
  return DefWindowProc(hWnd, Msg, wParam, lParam);
} /* End of 'WinFunc' function */

/* END OF 'MAIN.C' function */