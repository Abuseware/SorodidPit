#include <windows.h>

#include <array>
#include <tuple>

HWND Window, Button;

std::array<HBRUSH, 8> brushes = {
  CreateSolidBrush(RGB(255,   0,   0)),
  CreateSolidBrush(RGB(255, 255,   0)),
  CreateSolidBrush(RGB(  0, 255,   0)),
  CreateSolidBrush(RGB(  0, 255, 255)),
  CreateSolidBrush(RGB(  0,   0, 255)),
  CreateSolidBrush(RGB(255, 255, 255)),
  CreateSolidBrush(RGB(127, 127, 127)),
  CreateSolidBrush(RGB(  0,   0,   0))
};

unsigned int current_color = 0;

const int BUTTONSIZE = 50;
const int MARGIN = 50;
const int ScreenW = GetSystemMetrics(SM_CXSCREEN);
const int ScreenH = GetSystemMetrics(SM_CYSCREEN);

int buttonpos[8][2] = {
  { //0
    MARGIN,
    MARGIN
  },
  { //1
    ScreenW - MARGIN - BUTTONSIZE,
    MARGIN
  },
  { //2
    MARGIN,
    ScreenH - MARGIN - BUTTONSIZE
  },
  { //3
    ScreenW - MARGIN - BUTTONSIZE,
    ScreenH - MARGIN - BUTTONSIZE
  },
  { //4
    ScreenW / 4 * 1 - BUTTONSIZE / 2,
    ScreenH / 2 - BUTTONSIZE / 2
  },
  { //5
    ScreenW / 4 * 3 - BUTTONSIZE / 2,
    ScreenH / 2 - BUTTONSIZE / 2
  },
  { //6
    ScreenW / 2 - BUTTONSIZE / 2,
    ScreenH / 4 * 1 - BUTTONSIZE / 2
  },
  { //7
    ScreenW / 2 - BUTTONSIZE / 2,
    ScreenH / 4 * 3 - BUTTONSIZE / 2
  }
};

void redraw() {
  PAINTSTRUCT ps;
  HDC hdc;

  hdc = BeginPaint(Window, &ps);
  FillRect(hdc, &ps.rcPaint, brushes[current_color]);
  EndPaint(Window, &ps);

  hdc = BeginPaint(Button, &ps);
  FillRect(hdc, &ps.rcPaint, brushes[7]);
  ps.rcPaint.top += 1;
  ps.rcPaint.left += 1;
  ps.rcPaint.bottom -= 1;
  ps.rcPaint.right -= 1;
  FillRect(hdc, &ps.rcPaint, brushes[(current_color + 1) % brushes.size()]);
  EndPaint(Button, &ps);
}

void buttonClicked() {
  if(++current_color < brushes.size()) {
    SetWindowPos(
      Button,
      NULL,
      buttonpos[current_color][0], buttonpos[current_color][1],
      0, 0,
      SWP_NOSIZE | SWP_NOZORDER
    );
    InvalidateRect(Window, NULL, true);
  } else {
    PostQuitMessage(0);
  }
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
  switch(msg) {
    case WM_DESTROY:
      PostQuitMessage(0);
      break;
    case WM_PAINT:
      redraw();
      break;
    case WM_COMMAND:
      if((HWND)lParam == Button) buttonClicked();
      break;
    default:
      return DefWindowProc(hwnd, msg, wParam, lParam);
  }
  return 0;
}

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
  WNDCLASSEX WindowClass;
  MSG Msg;

  const char WindowClassName[] = "myWindowClass";

  WindowClass.cbSize        = sizeof(WNDCLASSEX);
  WindowClass.style         = 0;
  WindowClass.lpfnWndProc   = WndProc;
  WindowClass.cbClsExtra    = 0;
  WindowClass.cbWndExtra    = 0;
  WindowClass.hInstance     = hInstance;
  WindowClass.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
  WindowClass.hCursor       = LoadCursor(NULL, IDC_ARROW);
  WindowClass.hbrBackground = brushes[7];
  WindowClass.lpszMenuName  = NULL;
  WindowClass.lpszClassName = WindowClassName;
  WindowClass.hIconSm       = LoadIcon(NULL, IDI_APPLICATION);

  RegisterClassEx(&WindowClass);

  Window = CreateWindowEx(
    WS_EX_TOPMOST,
    WindowClassName, "Color Test",
    WS_POPUP | WS_MAXIMIZE | WS_VISIBLE,
    0, 0, ScreenW, ScreenH,
    NULL, NULL, hInstance, NULL
  );

  Button = CreateWindow(
    "BUTTON",
    "",
    WS_VISIBLE | WS_CHILD | BS_FLAT | BS_PUSHBUTTON,
    buttonpos[0][0], buttonpos[0][1], BUTTONSIZE, BUTTONSIZE,
    Window, NULL, hInstance, NULL
  );

  ShowWindow(Window, nCmdShow);

  while(GetMessage(&Msg, NULL, 0, 0) > 0) {
    TranslateMessage(&Msg);
    DispatchMessage(&Msg);
  }
  return Msg.wParam;
}
