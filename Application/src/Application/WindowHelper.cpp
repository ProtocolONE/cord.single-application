#include <Application/WindowHelper.h>

namespace GGS {
  namespace Application {

    WindowHelper::WindowHelper()
    {
    }

    WindowHelper::~WindowHelper()
    {
    }

    void WindowHelper::activate(HWND window)
    {
      HWND hForegroundWnd = GetForegroundWindow();
      if (hForegroundWnd == INVALID_HANDLE_VALUE)
      {
        hForegroundWnd = FindWindowW(L"Shell_TrayWnd", NULL);
      }

      if (window == hForegroundWnd)
        return;

      if (IsIconic(window))
        ShowWindow(window, SW_RESTORE);

      if (!SetForegroundWindow(window))
      {
        DWORD lpdwProcessId;
        DWORD curThread = GetWindowThreadProcessId(hForegroundWnd, &lpdwProcessId);
        DWORD myThread = GetCurrentThreadId();
        DWORD newThread = GetWindowThreadProcessId(window, &lpdwProcessId);
        AttachThreadInput(myThread, newThread, true);
        AttachThreadInput(myThread, curThread, true);
        AttachThreadInput(curThread, newThread, true);
        if (!SetForegroundWindow(window))
        {
          // OK, this is not funny - bring out the extreme measures (usually for 2000/XP)
          // Simulate two single ALT keystrokes, See LockSetForegroundWindow

          keybd_event((byte)VK_MENU, (byte)MapVirtualKey(VK_MENU, 0), 0, 0);
          keybd_event((byte)VK_MENU, (byte)MapVirtualKey(VK_MENU, 0), KEYEVENTF_KEYUP, 0);
          keybd_event((byte)VK_MENU, (byte)MapVirtualKey(VK_MENU, 0), 0, 0);
          keybd_event((byte)VK_MENU, (byte)MapVirtualKey(VK_MENU, 0), KEYEVENTF_KEYUP, 0);

          SetForegroundWindow(window);
        }

        AttachThreadInput(myThread, newThread, false);
        AttachThreadInput(myThread, curThread, false);
        AttachThreadInput(curThread, newThread, false);
      }

      SetWindowPos(window, HWND_TOPMOST, 0, 0, 0, 0, SWP_SHOWWINDOW | SWP_NOSIZE | SWP_NOMOVE);
      SetWindowPos(window, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_SHOWWINDOW | SWP_NOSIZE | SWP_NOMOVE);
    }

  }
}