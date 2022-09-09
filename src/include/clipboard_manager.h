#ifndef OCTANE_GUI_CLIENT_CLIPBOARD_MANAGER_H_
#define OCTANE_GUI_CLIENT_CLIPBOARD_MANAGER_H_

#include <QAbstractNativeEventFilter>
#include <QFileInfo>
#include <QTemporaryDir>
#include <functional>
#include <optional>
#include <string>
#include <unordered_map>

#include "./clipboard_data.h"

namespace octane::gui {
  /**
   * @brief APIと通信し、クリップボードの管理をする抽象クラス。
   *
   */
  class ClipboardManager : public QAbstractNativeEventFilter {
  public:
    static ClipboardManager* getSystemClipboardManager();

    virtual ~ClipboardManager() noexcept                     = 0;
    virtual std::optional<ClipboardData> copyFromClipboard() = 0;
    virtual void copyFromSelection(
      const std::function<void(ClipboardData&&)>& callback)
      = 0;
    virtual void pasteToClipboard(const ClipboardData& data) = 0;
    virtual void pasteToSelection(const ClipboardData& data) = 0;

  protected:
    std::optional<ClipboardData> getClipboardData();
    void setClipboardData(const ClipboardData& data, QTemporaryDir& tmpDir);
    /**
     * @brief 再帰的にファイルを探索する。
     * @details
     * dirはディレクトリでなければならない。
     *
     * @param[in] basePath
     * @param[in] dirInfo
     * @param[in,out] output
     * @return キャンセルされたらfalse
     */
    bool searchFiles(const QString& basePath,
                     const QFileInfo& dirInfo,
                     std::unordered_map<std::string, QByteArray>& output);
    QByteArray readFile(const QFileInfo& fileInfo);
    void writeFile(const QString& path, const QByteArray& bytes);
  };
} // namespace octane::gui

#endif // OCTANE_GUI_CLIENT_CLIPBOARD_MANAGER_H_