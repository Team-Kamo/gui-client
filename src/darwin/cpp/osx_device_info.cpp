#include "include/osx_device_info.h"

#include <unistd.h>

namespace octane::gui::darwin {
  std::string getDeviceName() {
    char buf[256];
    gethostname(buf, sizeof(buf));
    return buf;
  }
} // namespace octane::gui::darwin
