#include "include/api.h"

#include <ctime>
#include <vector>

#include "include/config.h"
#include "include/device_info.h"
#include "include/message_box.h"
#include "include/settings.h"

namespace octane::gui {
  Api Api::instance;
  Api::Api() : apiClient() {}
  Result<_, ErrorResponse> Api::connect(std::uint64_t id) {
    auto result = instance.apiClient.connectRoom(id, getDeviceName());
    if (!result) {
      return error(result.err());
    }
    return ok();
  }
  Result<std::uint64_t, ErrorResponse> Api::createRoom(const QString& name) {
    auto createResult = instance.apiClient.createRoom(name.toStdString());
    if (!createResult) {
      return error(createResult.err());
    }
    auto id            = createResult.get().id;
    auto connectResult = connect(id);
    if (!connectResult) {
      return error(connectResult.err());
    }
    return ok(id);
  }
  void Api::uploadAsClipboard(const ClipboardData& data) {
    std::vector<std::uint8_t> uploadData;
    uploadData.assign(data.data.begin(), data.data.end());
    auto result = instance.apiClient.uploadContent(
      Content{
        .contentStatus
        = ContentStatus{ .device    = getDeviceName(),
                         .timestamp = (std::uint64_t)std::time(nullptr),
                         .type      = ContentType::File,
                         .mime = data.mime, },
        .data = uploadData,
      });
    if (!result) {
      openCritical(nullptr, result.err());
    }
  }
  Result<ClipboardData, ErrorResponse> Api::download() {
    auto result = instance.apiClient.getContent();
    if (!result) {
      return error(result.err());
    }

    auto& data   = result.get().data;
    auto& status = result.get().contentStatus;

    QByteArray qData;
    if (std::holds_alternative<std::string>(data)) {
      qData = QByteArray::fromStdString(std::get<std::string>(data));
    } else {
      auto& vec = std::get<std::vector<std::uint8_t>>(data);
      qData.resize(vec.size());
      std::copy(vec.begin(), vec.end(), qData.begin());
    }

    return ok(ClipboardData{
      .mime = status.mime,
      .data = std::move(qData),
    });
  }
} // namespace octane::gui