#include <windows.h>

#include <array>

std::array<HBRUSH, 7> brushes = {
  CreateSolidBrush(RGB(255,   0,   0)),
  CreateSolidBrush(RGB(255, 255,   0)),
  CreateSolidBrush(RGB(  0, 255,   0)),
  CreateSolidBrush(RGB(  0, 255, 255)),
  CreateSolidBrush(RGB(  0,   0, 255)),
  CreateSolidBrush(RGB(255, 255, 255)),
  CreateSolidBrush(RGB(  0,   0,   0))
};

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
  static unsigned int i = 0;

  switch(msg) {
    case WM_PAINT:
      PAINTSTRUCT ps;
      HDC hdc;
      hdc = BeginPaint(hwnd, &ps);
      FillRect(hdc, &ps.rcPaint, brushes[i]);
      EndPaint(hwnd, &ps);
      break;
    case WM_LBUTTONUP:
      if(++i < brushes.size()) {
        InvalidateRect(hwnd, NULL, true);
      } else {
        PostQuitMessage(0);
      }
      break;
    default:
      return DefWindowProc(hwnd, msg, wParam, lParam);
  }
  return 0;
}

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
  WNDCLASSEX wc;
  HWND hwnd;
  MSG Msg;

  const char WindowClassName[] = "myWindowClass";

  wc.cbSize        = sizeof(WNDCLASSEX);
  wc.style         = 0;
  wc.lpfnWndProc   = WndProc;
  wc.cbClsExtra    = 0;
  wc.cbWndExtra    = 0;
  wc.hInstance     = hInstance;
  wc.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
  wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
  wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
  wc.lpszMenuName  = NULL;
  wc.lpszClassName = WindowClassName;
  wc.hIconSm       = LoadIcon(NULL, IDI_APPLICATION);

  RegisterClassEx(&wc);

  hwnd = CreateWindowEx(WS_EX_TOPMOST,
    WindowClassName, "Color Test",
    WS_POPUP | WS_MAXIMIZE | WS_VISIBLE,
    0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN),
    NULL, NULL, hInstance, NULL);

  ShowWindow(hwnd, nCmdShow);
  //UpdateWindow(hwnd);

  while(GetMessage(&Msg, NULL, 0, 0) > 0) {
    TranslateMessage(&Msg);
    DispatchMessage(&Msg);
  }
  return Msg.wParam;
}
