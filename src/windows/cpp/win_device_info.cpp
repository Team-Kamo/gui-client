#include "include/win_device_info.h"

#include <Windows.h>

namespace octane::gui::windows {
  std::string getDeviceName() {
    TCHAR lpBuffer[MAX_COMPUTERNAME_LENGTH + 1];
    DWORD size = MAX_COMPUTERNAME_LENGTH + 1;
    GetComputerName(lpBuffer, &size);
#ifdef _UNICODE
    int bufSize
      = WideCharToMultiByte(CP_UTF8, 0, lpBuffer, size, nullptr, 0, 0, nullptr);
    std::string str;
    str.resize(bufSize);
    WideCharToMultiByte(
      CP_UTF8, 0, lpBuffer, size, str.data(), str.size(), 0, nullptr);
    return str;
#else
    return std::string(lpBuffer, size);
#endif // _UNICODE
  }
} // namespace octane::gui::windows