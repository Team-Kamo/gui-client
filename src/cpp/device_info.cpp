#include "include/device_info.h"

#if defined(_WIN32) || defined(_WIN64)
#include "windows/include/win_device_info.h"
#elif defined(__APPLE__)
#include "darwin/include/osx_device_info.h"
#endif // defined(_WIN32) || defined(_WIN64)

namespace octane::gui {
  std::string getDeviceName() {
#if defined(_WIN32) || defined(_WIN64)
    return windows::getDeviceName();
#elif defined(__APPLE__)
    return darwin::getDeviceName();
#else
    return "Not supported on this device.";
#endif // defined(_WIN32) || defined(_WIN64)
  }
} // namespace octane::gui
