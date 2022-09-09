#include "include/api.h"

#include <QBuffer>
#include <ctime>
#include <vector>

#include "include/config.h"
#include "include/device_info.h"
#include "include/message_box.h"
#include "include/settings.h"

namespace octane::gui {
  Api Api::instance;
  Api::Api() : client(nullptr) {}
  Api::~Api() {
    if (client) {
      delete client;
      client = nullptr;
    }
  }
  Result<_, ErrorResponse> Api::init(const QString& token,
                                     const QString& origin,
                                     const QString& baseUrl) {
    if (instance.client) {
      delete instance.client;
      instance.client = nullptr;
    }

    instance.client = new ApiClient(token.toUtf8().toStdString(),
                                    origin.toUtf8().toStdString(),
                                    baseUrl.toUtf8().toStdString());
    auto result     = instance.client->init();
    if (!result) {
      return error(result.err());
    }

    return ok();
  }
  Result<_, ErrorResponse> Api::connect(std::uint64_t id) {
    if (instance.client == nullptr) {
      return makeError(
        "ERR_API_CLIENT_IS_NOT_INITIALIZED",
        "ApiClient is not initialized. Call Api::init before connecting.");
    }
    auto result = instance.client->connectRoom(id, getDeviceName());
    if (!result) {
      return error(result.err());
    }
    return ok();
  }
  Result<std::uint64_t, ErrorResponse> Api::createRoom(const QString& name) {
    if (instance.client == nullptr) {
      return makeError(
        "ERR_API_CLIENT_IS_NOT_INITIALIZED",
        "ApiClient is not initialized. Call Api::init before connecting.");
    }
    auto createResult
      = instance.client->createRoom(name.toUtf8().toStdString());
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
  Result<_, ErrorResponse> Api::upload(ClipboardData&& data) {
    if (instance.client == nullptr) {
      return makeError(
        "ERR_API_CLIENT_IS_NOT_INITIALIZED",
        "ApiClient is not initialized. Call Api::init before connecting.");
    }
    if (std::holds_alternative<UniData>(data.data)) {
      auto& uniData = std::get<UniData>(data.data);
      ContentType contentType;
      if (uniData.mime.find("text/") == 0) {
        contentType = ContentType::Clipboard;
      } else {
        contentType = ContentType::File;
      }
      auto result = instance.client->uploadContent(
      Content{
        .contentStatus = ContentStatus{
          .device    = getDeviceName(),
          .timestamp = (std::uint64_t)std::time(nullptr),
          .type      = contentType,
          .mime      = std::move(uniData.mime),
        },
        .data = std::move(uniData.data),
      });
      if (!result) {
        return error(result.err());
      }
    } else {
      auto& multiData = std::get<MultiData>(data.data);
      std::vector<FileInfo> uploadData;
      uploadData.reserve(multiData.files.size());
      for (const auto& file : multiData.files) {
        uploadData.push_back(FileInfo{
          .filename = file.first,
          .data     = std::move(file.second),
        });
      }

      auto result = instance.client->uploadContent(
        Content{
          .contentStatus = ContentStatus{
            .device = getDeviceName(),
            .timestamp = (std::uint64_t)std::time(nullptr),
            .type = ContentType::MultiFile,
            .mime = "",
          },
          .data = std::move(uploadData),
        }
      );
      if (!result) {
        return error(result.err());
      }
    }

    return ok();
  }
  Result<ClipboardData, ErrorResponse> Api::download() {
    if (instance.client == nullptr) {
      return makeError(
        "ERR_API_CLIENT_IS_NOT_INITIALIZED",
        "ApiClient is not initialized. Call Api::init before connecting.");
    }
    auto result = instance.client->getContent();
    if (!result) {
      return error(result.err());
    }

    auto& data   = result.get().data;
    auto& status = result.get().contentStatus;

    if (std::holds_alternative<std::string>(data)) {
      auto& str = std::get<std::string>(data);
      std::vector<std::uint8_t> vec;
      vec.resize(str.size());
      std::copy(str.begin(), str.end(), vec.begin());
      return ok(ClipboardData{
        .data = UniData{ .mime = std::move(status.mime),
                         .data = std::move(vec), },
      });
    } else if (std::holds_alternative<std::vector<std::uint8_t>>(data)) {
      return ok(ClipboardData{
        .data = UniData{
          .mime = status.mime,
          .data = std::move(std::get<std::vector<std::uint8_t>>(data)),
        },
      });
    } else {
      const auto& files = std::get<std::vector<FileInfo>>(data);
      MultiData multiData;
      for (const auto& file : files) {
        multiData.files[file.filename] = std::move(file.data);
      }
      return ok(ClipboardData{
        .data = std::move(multiData),
      });
    }
  }
} // namespace octane::gui