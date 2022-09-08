#ifndef OCTANE_GUI_CLIENT_API_H_
#define OCTANE_GUI_CLIENT_API_H_

#include <api_client.h>

#include "./clipboard_data.h"

namespace octane::gui {
  class Api {
    ApiClient* client;

    Api();
    ~Api() noexcept;
    Api(const Api&) = delete;
    Api(Api&&)      = delete;
    Api& operator=(const Api&) = delete;
    Api& operator=(Api&&) = delete;

    static Api instance;

  public:
    static Result<_, ErrorResponse> init(const QString& token,
                                         const QString& origin,
                                         const QString& baseUrl);
    static Result<_, ErrorResponse> connect(std::uint64_t id);
    static Result<std::uint64_t, ErrorResponse> createRoom(const QString& name);
    static Result<_, ErrorResponse> upload(const ClipboardData& data);
    static Result<ClipboardData, ErrorResponse> download();

  private:
    static std::vector<uint8_t> readFile(std::string_view filename);
  };
} // namespace octane::gui

#endif // OCTANE_GUI_CLIENT_API_H_