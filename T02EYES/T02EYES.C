/* Ilyin Vladimir, 01.06.2020 */
#include <stdlib.h>
#include <math.h>
#include <windows.h>

/* Window class name */
#define WND_CLASS_NAME "My window class"

/* Forward declaration */
LRESULT CALLBACK WinFunc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam );
VOID DrawEye( HDC hDC, INT X, INT Y, INT R, INT R1, INT Mx, INT My ); 
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
  hWnd = CreateWindow(WND_CLASS_NAME, "T02EYES", WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
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
  INT i;
  HDC hDC;
  PAINTSTRUCT ps;
  POINT pt;
  static INT w, h;
  static HDC hMemDC;
  static HBITMAP hBm;

  switch (Msg)
  {
  case WM_CREATE:
    hDC = GetDC(hWnd);
    hMemDC = CreateCompatibleDC(hDC);
    ReleaseDC(hWnd, hDC);
    hBm = NULL;
    SetTimer(hWnd, 30, 10, NULL);
    return 0;
  case WM_SIZE:
    w = LOWORD(lParam);
    h = HIWORD(lParam);

    if (hBm != NULL)
      DeleteObject(hBm);
    hDC = GetDC(hWnd);
    hBm = CreateCompatibleBitmap(hDC, w, h);
    ReleaseDC(hWnd, hDC);
    SelectObject(hMemDC, hBm);
    SendMessage(hWnd, WM_TIMER, 0, 0);
    return 0;
  case WM_KEYDOWN:
    if (wParam == VK_ESCAPE)
      SendMessage(hWnd, WM_CLOSE, 0, 0);
    return 0;
  case WM_TIMER:
    SelectObject(hMemDC, GetStockObject(WHITE_BRUSH));
    SelectObject(hMemDC, GetStockObject(NULL_PEN));
    Rectangle(hMemDC, 0, 0, w + 1, h + 1);

    GetCursorPos(&pt);
    ScreenToClient(hWnd, &pt);
    srand(30);
    for (i = 0; i < 102; i++)
      DrawEye(hMemDC, rand() % w, rand() % h, 50 + rand() % 47, 18 + rand() % 8, pt.x, pt.y);
    InvalidateRect(hWnd, NULL, FALSE);
    return 0;
  case WM_PAINT:
    hDC = BeginPaint(hWnd, &ps);
    BitBlt(hDC, 0, 0, w, h, hMemDC, 0, 0, SRCCOPY);
    EndPaint(hWnd, &ps);
    return 0; 
  case WM_DESTROY:
    if (hBm != 0)
      DeleteObject(hBm);
    DeleteDC(hMemDC);
    PostMessage(hWnd, WM_QUIT, 0, 0);
    KillTimer(hWnd, 30);
    return 0;
  }
  return DefWindowProc(hWnd, Msg, wParam, lParam);
} /* End of 'WinFunc' function */

/* 'DrawEye' function */
 VOID DrawEye( HDC hDC, INT X, INT Y, INT R, INT R1, INT Mx, INT My )
{
  INT dx = Mx - X, dy = My - Y, delta = R - R1;
  DOUBLE len = sqrt(dx * dx + dy * dy), t;
  

  SelectObject(hDC, GetStockObject(DC_BRUSH));
  SelectObject(hDC, GetStockObject(DC_PEN));
  SetDCPenColor(hDC, RGB(0, 0, 0));
  SetDCBrushColor(hDC, RGB(255, 255, 255));
  Ellipse(hDC, X - R, Y - R, X + R + 1, Y + R + 1);


  SelectObject(hDC, GetStockObject(DC_BRUSH));
  SelectObject(hDC, GetStockObject(DC_PEN));
  SetDCPenColor(hDC, RGB(0, 0, 0));
  SetDCBrushColor(hDC, RGB(0, 0, 0));

  if (!len)
    t = 0;
  else if (delta > len)
    t = 1;
  else
    t = delta / len;
  dx *= t;
  dy *= t;
  Ellipse(hDC, X + dx - R1, Y + dy - R1, X + dx + R1 + 1, Y + dy + R1 + 1);
} /* End of DrawEye function */
/* END OF 'TO2EYES.C' FILE */