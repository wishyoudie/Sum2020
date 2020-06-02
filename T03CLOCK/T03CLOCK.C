/* FILE NAME: T03CLOCK.C
 * PROGRAMMER VI6
 * DATE: 02.06.2020
 * PURPOSE: WinAPI Clock Drawing application sample
 */
#include <math.h>
#include <windows.h>

/* Window class name */
#define WND_CLASS_NAME "My window class"

/* Forward declaration */
LRESULT CALLBACK WinFunc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam );
VOID DrawHand( HDC hDC, INT X0, INT Y0, DOUBLE angle, INT L, INT W );
VOID FlipFullScreen( HWND hWnd );

/* The main program function */
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
  hWnd = CreateWindow(WND_CLASS_NAME, "T03CLOCK", WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN, 
    CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
    NULL, NULL, hInstance, NULL);

  ShowWindow(hWnd, CmdShow);

  /* Message loop */
  while (GetMessage(&msg, NULL, 0, 0))
    DispatchMessage(&msg);

  return 0;
} /* End of 'WinMain' function */

/* Main window handle function */
LRESULT CALLBACK WinFunc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
  INT r, len;
  HDC hDC;
  PAINTSTRUCT ps;
  HPEN hPen, hPenOld;
  HFONT hFnt, hFntOld;
  SYSTEMTIME st;
  RECT rc;
  CHAR Buf[102];
  static BITMAP bm;
  static HDC hMemDCFrame, hMemDCClock;
  static HBITMAP hBmFrame, hBmClock;
  static INT w, h;

  switch (Msg)
  {
  case WM_GETMINMAXINFO:
    ((MINMAXINFO *)lParam)->ptMaxTrackSize.y =
      GetSystemMetrics(SM_CYMAXTRACK) + GetSystemMetrics(SM_CYCAPTION) + 2 * GetSystemMetrics(SM_CYBORDER);
    return 0;

  case WM_CREATE:
    hDC = GetDC(hWnd);
    hMemDCFrame = CreateCompatibleDC(hDC);
    hMemDCClock = CreateCompatibleDC(hDC);
    ReleaseDC(hWnd, hDC);
    hBmFrame = NULL;
    hBmClock = LoadImage(NULL, "preview.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    GetObject(hBmClock, sizeof(bm), &bm);
    SelectObject(hMemDCClock, hBmClock);
    SetTimer(hWnd, 30, 10, NULL);
    return 0;

  case WM_SIZE:
    w = LOWORD(lParam);
    h = HIWORD(lParam);

    if (hBmFrame != NULL)
      DeleteObject(hBmFrame);
    hDC = GetDC(hWnd);
    hBmFrame = CreateCompatibleBitmap(hDC, w, h);
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
    /* Clean background */ 
    SelectObject(hMemDCFrame, GetStockObject(WHITE_BRUSH));
    SelectObject(hMemDCFrame, GetStockObject(NULL_PEN));
    Rectangle(hMemDCFrame, 0, 0, w + 1, h + 1);  
    
    /* Draw clockface */
    r = (w < h ? w : h);
    SetStretchBltMode(hMemDCFrame, COLORONCOLOR);
    StretchBlt(hMemDCFrame, w / 2 - r / 2, h / 2 - r / 2, r, r, hMemDCClock, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY);
    
    /* Draw hands */
    GetLocalTime(&st);
    
    hPen = CreatePen(PS_SOLID, 8, RGB(0, 0, 0));
    hPenOld = SelectObject(hMemDCFrame, hPen);

    DrawHand(hMemDCFrame, w / 2, h / 2, (st.wHour % 12 + st.wMinute / 60.0) * 2 * 3.14159265359 / 12.0, r * 0.18, 30);
    DrawHand(hMemDCFrame, w / 2, h / 2, (st.wMinute + st.wSecond / 60.0) * 2 * 3.14159265359 / 60.0, r * 0.30, 18);
    DrawHand(hMemDCFrame, w / 2, h / 2, (st.wSecond + st.wMilliseconds / 1000.0) * 2 * 3.14159265359 / 60.0, r * 0.47, 13);

    SelectObject(hMemDCFrame, hPenOld);
    DeleteObject(hPen);

    /* Output text */
    hFnt = CreateFont(r / 8 , 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, RUSSIAN_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, FF_SWISS | VARIABLE_PITCH, "Consolas"); 
    hFntOld = SelectObject(hMemDCFrame, hFnt);
    len = wsprintf(Buf, "%02i.%02i.%i", st.wDay, st.wMonth, st.wYear);
    
    SetBkMode(hMemDCFrame, TRANSPARENT);
    SetTextColor(hMemDCFrame, RGB(0, 0, 0));
    /* TextOut(hMemDCFrame, w / 2, h / 2, Buf, len); */
    rc.left = w / 3;
    rc.right = 0;
    rc.top = 0;
    rc.bottom = h;
    DrawText(hMemDCFrame, Buf, len, &rc, DT_CENTER | DT_VCENTER);

    SelectObject(hMemDCFrame, hFntOld);
    DeleteObject(hFnt);

    /* Send repaint message */
    InvalidateRect(hWnd, NULL, FALSE);

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
    DeleteObject(hBmClock);
    DeleteDC(hMemDCClock);

    PostMessage(hWnd, WM_QUIT, 0, 0);
    KillTimer(hWnd, 30);
    return 0;

  }
  return DefWindowProc(hWnd, Msg, wParam, lParam);
} /* End of 'WinFunc' function */

/* DrawHand function */
VOID DrawHand( HDC hDC, INT X0, INT Y0, DOUBLE angle, INT L, INT W )
{
  INT i;
  HPEN hPenOld;
  HBRUSH hBrOld;
  POINT pnts[] =
  {
    {0, L}, {W, 0}, {0, -W}, {-W, 0}
  }, pnts1[sizeof(pnts) / sizeof(pnts[0])];

  /* Transform hand shape */
  for (i = 0; i < sizeof(pnts) / sizeof(pnts[0]); i++)
  {
    pnts1[i].x = X0 + pnts[i].x * cos(angle) + pnts[i].y * sin(angle);
    pnts1[i].y = Y0 - (pnts[i].y * cos(angle) - pnts[i].x * sin(angle));
  }
  hPenOld = SelectObject(hDC, GetStockObject(DC_PEN));
  hBrOld = SelectObject(hDC, GetStockObject(DC_BRUSH));

  SetDCPenColor(hDC, RGB(0, 0 ,0));
  SetDCBrushColor(hDC, RGB(15, 15, 15));

  Polygon(hDC, pnts1, sizeof(pnts) / sizeof(pnts[0]));

  SelectObject(hDC, hPenOld);
  SelectObject(hDC, hBrOld);
} /* End of 'DrawHand' function */

/* Toggle window fullscreen mode function */
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
} /* End of 'FlipFullScreen' function */
/* END OF 'TO3CLOCK.C' FILE */