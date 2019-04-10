/*
 * Copyright (c) 2018, Artur Kaleta
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <windows.h>

#include <array>

HWND Window, Button;

std::array<HBRUSH, 8> brushes = {
        CreateSolidBrush(RGB(255, 0, 0)),
        CreateSolidBrush(RGB(255, 255, 0)),
        CreateSolidBrush(RGB(0, 255, 0)),
        CreateSolidBrush(RGB(0, 255, 255)),
        CreateSolidBrush(RGB(0, 0, 255)),
        CreateSolidBrush(RGB(255, 255, 255)),
        CreateSolidBrush(RGB(127, 127, 127)),
        CreateSolidBrush(RGB(0, 0, 0))
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
    if (++current_color < brushes.size()) {
        SetWindowPos(
                Button,
                nullptr,
                buttonpos[current_color][0], buttonpos[current_color][1],
                0, 0,
                SWP_NOSIZE | SWP_NOZORDER
        );
        InvalidateRect(Window, nullptr, true);
    } else {
        PostQuitMessage(0);
    }
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        case WM_PAINT:
            redraw();
            break;
        case WM_COMMAND:
            if ((HWND) lParam == Button) buttonClicked();
            break;
        default:
            return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    WNDCLASSEX WindowClass;
    MSG Msg;

    const char *WindowClassName = "myWindowClass";

    WindowClass.cbSize = sizeof(WNDCLASSEX);
    WindowClass.style = 0;
    WindowClass.lpfnWndProc = WndProc;
    WindowClass.cbClsExtra = 0;
    WindowClass.cbWndExtra = 0;
    WindowClass.hInstance = hInstance;
    WindowClass.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(0));
    WindowClass.hCursor = LoadCursor(nullptr, IDC_CROSS);
    WindowClass.hbrBackground = brushes[7];
    WindowClass.lpszMenuName = nullptr;
    WindowClass.lpszClassName = WindowClassName;
    WindowClass.hIconSm = LoadIcon(nullptr, IDI_APPLICATION);

    RegisterClassEx(&WindowClass);

    Window = CreateWindowEx(
            WS_EX_TOPMOST,
            WindowClassName, "Color Test",
            WS_POPUP | WS_MAXIMIZE | WS_VISIBLE,
            0, 0, ScreenW, ScreenH,
            nullptr, nullptr, hInstance, nullptr
    );

    Button = CreateWindow(
            "BUTTON",
            "",
            WS_VISIBLE | WS_CHILD | BS_FLAT | BS_PUSHBUTTON,
            buttonpos[0][0], buttonpos[0][1], BUTTONSIZE, BUTTONSIZE,
            Window, nullptr, hInstance, nullptr
    );

    ShowWindow(Window, nCmdShow);

    while (GetMessage(&Msg, nullptr, 0, 0) > 0) {
        TranslateMessage(&Msg);
        DispatchMessage(&Msg);
    }
    return Msg.wParam;
}
