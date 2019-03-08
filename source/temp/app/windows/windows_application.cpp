#include "temp/app/windows/windows_application.h"
#ifdef TEMP_PLATFORM_WINDOWS
#include <Windows.h>

#include "temp/core/logger.h"

namespace temp {
namespace app {
namespace windows {

namespace {
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) {
  switch (msg) {
  case WM_CREATE: {
      auto cp = (CREATESTRUCT*)lp;
      SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)cp->lpCreateParams);
      return 0;
    }
    case WM_SIZE: {
      RECT rect;
      GetClientRect(hwnd, &rect);
      auto app = (WindowsApplication*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
      auto on_resize = app->on_resize_window();
      on_resize(rect.right - rect.left, rect.bottom - rect.top);
      return 0;
    }
    case WM_DESTROY:
      PostQuitMessage(0);
      return 0;
  }

  return DefWindowProc(hwnd, msg, wp, lp);
}
}  // namespace

WindowsApplication::WindowsApplication() {
  WNDCLASSEX wndclass;
  HINSTANCE instance_handle = GetModuleHandle(NULL);

  wndclass.cbSize = sizeof(wndclass);
  wndclass.style = CS_HREDRAW | CS_VREDRAW;
  wndclass.lpfnWndProc = WndProc;
  wndclass.cbClsExtra = 0;
  wndclass.cbWndExtra = 0;
  wndclass.hInstance = instance_handle;
  wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
  wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
  wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
  wndclass.lpszMenuName = NULL;
  wndclass.lpszClassName = "TempuraWindow";
  wndclass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

  RegisterClassEx(&wndclass);

  DWORD style = WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_SIZEBOX;
  RECT window_rect = {
      0,
      0,
      static_cast<LONG>(1080),
      static_cast<LONG>(720),
  };
  AdjustWindowRect(&window_rect, style, FALSE);
  LONG window_width = window_rect.right - window_rect.left;
  LONG window_height = window_rect.bottom - window_rect.top;
  HWND hwnd = CreateWindow("TempuraWindow", "てんぷら", style, CW_USEDEFAULT,
                           CW_USEDEFAULT, window_width, window_height, NULL,
                           NULL, instance_handle, (LPVOID)this);

  ShowWindow(hwnd, SW_SHOW);
  UpdateWindow(hwnd);
  native_window_handle_ = hwnd;

  TEMP_LOG_TRACE("Created WindowsApplication.");
}

WindowsApplication::~WindowsApplication() {
  HWND hwnd = reinterpret_cast<HWND>(native_window_handle_);
  DestroyWindow(hwnd);

  TEMP_LOG_TRACE("Destroied WindowsApplication.");
}

std::int32_t WindowsApplication::Run() {
  on_initialize_();

  MSG msg;
  {
    while (true) {
      if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
        if (msg.message == WM_QUIT) {
          break;
        }
        TranslateMessage(&msg);
        DispatchMessage(&msg);
      } else {
        on_update_();
      }
    }
  }

  on_terminate_();

  return static_cast<std::int32_t>(msg.wParam);
}

void WindowsApplication::Exit() {
  HWND hwnd = reinterpret_cast<HWND>(native_window_handle_);
  SendMessage(hwnd, WM_QUIT, 0, 0);
}

}  // namespace windows
}  // namespace app
}  // namespace temp
#endif