#include "include/clipboard_manager.h"

#include <QApplication>
#include <QBuffer>
#include <QClipboard>
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QMessageBox>
#include <QMimeData>
#include <QUrl>
#include <cassert>

#if defined(Q_OS_WIN)
#include "windows/include/win_clipboard_manager.h"
#elif defined(Q_OS_MAC)
#include "darwin/include/osx_clipboard_manager.h"
#endif

namespace octane::gui {
  ClipboardManager* ClipboardManager::getSystemClipboardManager() {
#if defined(Q_OS_WIN)
    return new windows::WinClipboardManager();
#elif defined(Q_OS_MAC)
    return new darwin::OsxClipboardManager();
#endif
    return nullptr;
  }
  ClipboardManager::~ClipboardManager() {}
  std::optional<ClipboardData> ClipboardManager::getClipboardData() {
    auto clipboard = QApplication::clipboard();
    auto mimeData  = clipboard->mimeData();

    if (mimeData->hasImage()) {
      auto image = qvariant_cast<QImage>(mimeData->imageData());
      QByteArray data;
      QBuffer buffer(&data);
      buffer.open(QIODevice::ReadWrite);
      image.save(&buffer, "PNG");
      return ClipboardData{
        .data = UniData {
          .mime = "image/png",
          .data = std::move(data),
        },
      };
    }
    if (mimeData->hasUrls()) {
      MultiData multiData;
      for (const auto& url : mimeData->urls()) {
        if (!url.isLocalFile()) continue;
        QFileInfo fileInfo(url.toLocalFile());
        if (fileInfo.isDir()) {
          if (!searchFiles(fileInfo.fileName().toStdString() + "/",
                           fileInfo,
                           multiData.files)) {
            return std::nullopt;
          }
        } else {
          auto filename             = fileInfo.fileName().toStdString();
          multiData.files[filename] = readFile(fileInfo);
        }
      }
      return ClipboardData{
        .data = std::move(multiData),
      };
    }
    if (mimeData->hasText()) {
      return ClipboardData{
        .data = UniData {
          .mime = "text/plain",
        .data = mimeData->text().toUtf8(),
        },
      };
    }

    return std::nullopt;
  }

  void ClipboardManager::setClipboardData(const ClipboardData& data,
                                          QTemporaryDir& tmpDir) {
    auto clipboard = QApplication::clipboard();

    if (std::holds_alternative<UniData>(data.data)) {
      const auto& uniData = std::get<UniData>(data.data);
      if (uniData.mime.starts_with("image/")) {
        QImage image;
        image.loadFromData(uniData.data);
        clipboard->setImage(image);
      } else {
        clipboard->setText(uniData.data);
      }
    } else {
      const auto& multiData = std::get<MultiData>(data.data);

      for (const auto& file : multiData.files) {
        auto tmpFile = tmpDir.filePath(QString::fromStdString(file.first));
        writeFile(tmpFile, file.second);
      }

      QString filenames;
      QList<QUrl> urls;
      auto root = QDir(tmpDir.path());
      for (const auto& file : root.entryInfoList()) {
        if (file.fileName() == "." || file.fileName() == "..") {
          continue;
        }
        filenames.append(file.fileName());
        filenames.append("\n");
        qDebug() << file.absoluteFilePath();
        urls.append(QUrl::fromLocalFile(file.absoluteFilePath()));
      }

      auto mimeData = new QMimeData();
      mimeData->setText(filenames);
      mimeData->setUrls(urls);
      clipboard->setMimeData(mimeData);
    }
  };
  bool ClipboardManager::searchFiles(
    const std::string& basePath,
    const QFileInfo& dirInfo,
    std::unordered_map<std::string, QByteArray>& output) {
    if (output.size() == 100) {
      if (QMessageBox::question(
            nullptr,
            qApp->tr("確認"),
            qApp->tr("ファイルの数が100個以上あります。続けますか？"))
          == QMessageBox::No) {
        return false;
      }
    }
    assert(dirInfo.isDir());
    QDir dir(dirInfo.absoluteFilePath());
    // NOTE: QDir::NoDotAndDotDotを指定するとなぜかループが回らない。
    for (const auto& fileInfo : dir.entryInfoList()) {
      qDebug() << fileInfo.fileName();
      if (fileInfo.fileName() == "." || fileInfo.fileName() == "..") {
        continue;
      }
      if (fileInfo.isDir()) {
        if (!searchFiles(basePath + fileInfo.fileName().toStdString() + "/",
                         fileInfo,
                         output)) {
          return false;
        }
      } else {
        auto filename    = basePath + fileInfo.fileName().toStdString();
        output[filename] = readFile(fileInfo);
      }
    }
    return true;
  }
  QByteArray ClipboardManager::readFile(const QFileInfo& fileInfo) {
    QFile file(fileInfo.absoluteFilePath());
    file.open(QIODevice::ReadOnly);
    return file.readAll();
  }
  void ClipboardManager::writeFile(const QString& path,
                                   const QByteArray& bytes) {
    QDir dir = QFileInfo(path).dir();
    if (!dir.exists()) {
      dir.mkpath(dir.path());
    }
    QFile file(path);
    file.open(QIODevice::WriteOnly);
    file.write(bytes);
  }
} // namespace octane::gui