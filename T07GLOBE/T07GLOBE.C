/* FILE NAME: T07GLOBE.C
 * PROGRAMMER VI6
 * DATE: 04.06.2020
 * PURPOSE: WinAPI Globe Drawing application main handlers
 */

#include <stdio.h>
#include <windows.h>

#include "GLOBE.H"
#include "TIMER.H"

/* Window class name */
#define WND_CLASS_NAME "My window class"

/* Forward declaration */
LRESULT CALLBACK WinFunc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam );
VOID FlipFullScreen( HWND hWnd );

/* Main program function
 * ARGUMENTS:
 *   - HINSTANCE:
 *       HINSTANCE hInstance, HINSTANCE hPrevInstance;
 *   - Command line:
 *       CHAR *CmdLine, INT CmdShow;
 * RETURNS: None.    
 */
int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, CHAR *CmdLine, INT CmdShow )
{
  HWND hWnd;
  MSG msg;
  WNDCLASS wc;

  /* Fill window class structure */
  wc.style = CS_HREDRAW | CS_VREDRAW;
  wc.cbClsExtra = 0;
  wc.cbWndExtra = 0;
  wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
  wc.hCursor = LoadCursor(NULL, IDC_HAND);
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
  hWnd = CreateWindow(WND_CLASS_NAME, "T07GLOBE", WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN, 
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
  static BITMAP bm;
  static HDC hMemDCFrame;
  static HBITMAP hBmFrame;
  static INT w, h;
  CHAR Buf[102];

  switch (Msg)
  {
  case WM_GETMINMAXINFO:
    ((MINMAXINFO *)lParam)->ptMaxTrackSize.y =
      GetSystemMetrics(SM_CYMAXTRACK) + GetSystemMetrics(SM_CYCAPTION) + 2 * GetSystemMetrics(SM_CYBORDER);
    return 0;

  case WM_CREATE:
    TimerInit();

    hDC = GetDC(hWnd);
    hMemDCFrame = CreateCompatibleDC(hDC);
    ReleaseDC(hWnd, hDC);
    hBmFrame = NULL;
    SetTimer(hWnd, 30, 12, NULL);
    return 0;

  case WM_SIZE:
    w = LOWORD(lParam);
    h = HIWORD(lParam);

    if (hBmFrame != NULL)
      DeleteObject(hBmFrame);
    hDC = GetDC(hWnd);
    hBmFrame = CreateCompatibleBitmap(hDC, w, h);
    GlobeSet(w / 2, h / 2, (w < h ? w : h) * 0.8 / 2);
    ReleaseDC(hWnd, hDC);
    SelectObject(hMemDCFrame, hBmFrame);
    SendMessage(hWnd, WM_TIMER, 0, 0);
    return 0;

  case WM_SYSKEYDOWN:
    if (wParam == VK_RETURN)
      FlipFullScreen(hWnd);
    return 0;

  case WM_KEYDOWN:
    if (wParam == VK_ESCAPE)
      SendMessage(hWnd, WM_CLOSE, 0, 0);
    return 0;

  case WM_TIMER:
    TimerResponse();

    /* Clean background */ 
    SelectObject(hMemDCFrame, GetStockObject(DC_BRUSH));
    SetDCBrushColor(hMemDCFrame, RGB(0, 0, 0));
    SelectObject(hMemDCFrame, GetStockObject(NULL_PEN));
    Rectangle(hMemDCFrame, 0, 0, w + 1, h + 1);  

    /* Draw globe */
    GlobeDraw(hMemDCFrame);
    /* Send repaint message */
    InvalidateRect(hWnd, NULL, FALSE);
    SetTextColor(hMemDCFrame, RGB(0, 255, 0));
    SetBkMode(hMemDCFrame, TRANSPARENT);
    TextOut(hMemDCFrame, 0, 0, Buf, sprintf(Buf, "FPS: %.3f", GLB_FPS)); 

    return 0;

  case WM_PAINT:
    hDC = BeginPaint(hWnd, &ps);

    /* Copy frame to window */
    BitBlt(hDC, 0, 0, w, h, hMemDCFrame, 0, 0, SRCCOPY);

    EndPaint(hWnd, &ps);

    return 0;

  case WM_DESTROY:
    if (hBmFrame != 0)
      DeleteObject(hBmFrame);
    DeleteDC(hMemDCFrame);

    PostMessage(hWnd, WM_QUIT, 0, 0);
    KillTimer(hWnd, 30);
    return 0;

  }
  return DefWindowProc(hWnd, Msg, wParam, lParam);
} /* End of 'WinFunc' function */

/* Toggle window fullscreen function
 * ARGUMENTS:
 *   - Window:
 *       HWND hWnd;
 * RETURNS: None.    
 */
VOID FlipFullScreen( HWND hWnd )
{
  static BOOL IsFullScreen = FALSE;
  static RECT SaveRect;

  if (!IsFullScreen)
  {
    HMONITOR hmon;
    MONITORINFO moninfo;
    RECT rc;

    /* Goto fullscreen mode */
    GetWindowRect(hWnd, &SaveRect);

    /* Get nearest monitor */
    hmon = MonitorFromWindow(hWnd, MONITOR_DEFAULTTONEAREST);

    /* Get monitor information */
    moninfo.cbSize = sizeof(moninfo);
    GetMonitorInfo(hmon, &moninfo);

    rc = moninfo.rcMonitor;
    AdjustWindowRect(&rc, GetWindowLong(hWnd, GWL_STYLE), FALSE);
    SetWindowPos(hWnd, HWND_TOP,
      rc.left, rc.top,
      rc.right - rc.left, rc.bottom - rc.top,
      SWP_NOOWNERZORDER);
    IsFullScreen = TRUE;
  }
  else
  {
    /* Restore window size and position */
    SetWindowPos(hWnd, HWND_TOP,
      SaveRect.left, SaveRect.top,
      SaveRect.right - SaveRect.left, SaveRect.bottom - SaveRect.top,
      SWP_NOOWNERZORDER);
    IsFullScreen = FALSE;
  }
}
/* End of 'FlipFullScreen' function */

/* END OF 'TO7GLOBE.C' FILE */