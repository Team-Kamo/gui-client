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
  Result<_, ErrorResponse> Api::upload(const ClipboardData& data) {
    if (instance.client == nullptr) {
      return makeError(
        "ERR_API_CLIENT_IS_NOT_INITIALIZED",
        "ApiClient is not initialized. Call Api::init before connecting.");
    }
    if (std::holds_alternative<UniData>(data.data)) {
      const auto& uniData = std::get<UniData>(data.data);
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
          .mime      = uniData.mime,
        },
        .data = std::vector<std::uint8_t>(uniData.data.begin(),
                                           uniData.data.end()),
      });
      if (!result) {
        return error(result.err());
      }
    } else {
      const auto& multiData = std::get<MultiData>(data.data);
      std::vector<FileInfo> uploadData;
      uploadData.reserve(multiData.files.size());
      for (const auto& file : multiData.files) {
        uploadData.push_back(FileInfo{
          .filename = file.first,
          .data
          = std::vector<std::uint8_t>(file.second.begin(), file.second.end()),
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
      return ok(ClipboardData{
        .data = UniData{ .mime = status.mime,
                         .data = QByteArray::fromStdString(
                           std::get<std::string>(data)), },
      });
    } else if (std::holds_alternative<std::vector<std::uint8_t>>(data)) {
      const auto& vec = std::get<std::vector<std::uint8_t>>(data);
      QByteArray qData;
      qData.reserve(vec.size());
      std::copy(vec.begin(), vec.end(), std::back_inserter(qData));
      return ok(ClipboardData{
        .data = UniData{
          .mime = status.mime,
          .data = std::move(qData),
        },
      });
    } else {
      const auto& files = std::get<std::vector<FileInfo>>(data);
      MultiData multiData;
      for (const auto& file : files) {
        multiData.files[file.filename].reserve(file.data.size());
        std::copy(file.data.begin(),
                  file.data.end(),
                  std::back_inserter(multiData.files[file.filename]));
      }
      return ok(ClipboardData{
        .data = std::move(multiData),
      });
    }
  }
} // namespace octane::gui