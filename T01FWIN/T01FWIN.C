/* Ilyin Vladimir, 01.06.2020 */
#include <stdlib.h>
#include <windows.h>

/* Window class name */
#define WND_CLASS_NAME "My window class"

/* Forward declaration */
LRESULT CALLBACK WinFunc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam );

/* The main program function */
int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, CHAR *CmdLine, INT CmdShow )
{
  HWND hWnd;
  MSG msg;
  WNDCLASS wc;

  /* Fill window class structure */
  wc.style = CS_HREDRAW;
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
  hWnd = CreateWindow(WND_CLASS_NAME, "CGSG", WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
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
  HDC hDC;
  PAINTSTRUCT ps;
  POINT pt;
  static INT w, h;

  switch (Msg)
  {
  case WM_CREATE:
    SetTimer(hWnd, 30, 1002, NULL);
    return 0;
  case WM_SIZE:
    w = LOWORD(lParam);
    h = HIWORD(lParam);
    return 0;
  case WM_KEYDOWN:
    if (wParam == VK_ESCAPE)
      SendMessage(hWnd, WM_CLOSE, 0, 0);
    return 0;
  case WM_TIMER:
    /* InvalidateRect(hWnd, NULL, FALSE); */
    return 0;
  case WM_PAINT:
    hDC = BeginPaint(hWnd, &ps);
    SelectObject(hDC, GetStockObject(DC_BRUSH));
    SelectObject(hDC, GetStockObject(DC_PEN));

    SetDCPenColor(hDC, RGB(0, 0, 0));
    SetDCBrushColor(hDC, RGB(rand() % 256, rand() % 256, rand() % 256));
    Ellipse(hDC, 0, 0, w + 1, h + 1);

    MoveToEx(hDC, w / 2, h / 2, NULL);
    GetCursorPos(&pt);
    ScreenToClient(hWnd, &pt);
    LineTo(hDC, pt.x, pt.y);
    /* Ellipse(hDC, rand() % 1024, rand() % 900, rand() % 1024, rand() % 900); */

    EndPaint(hWnd, &ps);
    return 0; 
  case WM_CLOSE:
    if (MessageBox(hWnd, "Are you sure?", "Close", MB_YESNO | MB_DEFBUTTON2 | MB_ICONQUESTION) == IDYES);
    DestroyWindow(hWnd);
    return 0;
  case WM_DESTROY:
    PostMessage(hWnd, WM_QUIT, 0, 0);
    KillTimer(hWnd, 30);
    return 0;
  }
  return DefWindowProc(hWnd, Msg, wParam, lParam);
} /* End of 'WinFunc' function */

/* END OF 'TO1FWIN.C' FILE */