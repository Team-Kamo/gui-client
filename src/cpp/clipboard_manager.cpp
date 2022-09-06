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

namespace octane::gui {
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
        QFileInfo fileInfo(url.path());
        if (fileInfo.isDir()) {
          if (!searchFiles("", fileInfo, multiData.files)) {
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
      QString filenames;
      for (const auto& file : multiData.files) {
        filenames.append(QString::fromStdString(file.first));
        filenames.append("\n");
      }
      clipboard->setText(filenames);

      QList<QUrl> urls;
      for (const auto& file : multiData.files) {
        auto tmpFile = tmpDir.filePath(QString::fromStdString(file.first));
        writeFile(tmpFile, file.second);
        urls.append(tmpFile);
      }

      auto mimeData = new QMimeData();
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
    QDir dir(dirInfo.fileName());
    for (const auto& fileInfo : dir.entryInfoList(QDir::NoDotAndDotDot)) {
      if (fileInfo.isDir()) {
        if (!searchFiles(basePath + "/" + fileInfo.fileName().toStdString(),
                         fileInfo,
                         output)) {
          return false;
        }
      } else {
        auto filename    = fileInfo.fileName().toStdString();
        output[filename] = readFile(fileInfo);
      }
    }
    return true;
  }
  QByteArray ClipboardManager::readFile(const QFileInfo& fileInfo) {
    QFile file(fileInfo.filePath());
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