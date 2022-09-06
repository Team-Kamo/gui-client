#include "include/tmp_dir.h"

#include <QDir>
#include <QFileInfo>

namespace octane::gui {
  TmpDir TmpDir::instance;
  TmpDir::TmpDir() : tmpDirs(nullptr), numTmpDirs(0), lastIndex(-1) {}
  TmpDir::~TmpDir() {
    delete[] tmpDirs;
    tmpDirs = nullptr;
  }
  QTemporaryDir& TmpDir::makeTmpDir() {
    if (instance.numTmpDirs <= instance.lastIndex) {
      instance.extend();
    }
    return instance.tmpDirs[instance.lastIndex++];
  }
  void TmpDir::extend() {
    size_t oldSize = numTmpDirs;
    if (oldSize == 0) {
      numTmpDirs = 16;
    } else {
      numTmpDirs *= 2;
    }

    auto newTmpDirs = new QTemporaryDir[numTmpDirs];
    for (size_t i = 0; i < oldSize; ++i) {
      moveFiles(newTmpDirs[i], tmpDirs[i]);
    }

    delete[] tmpDirs;
    tmpDirs = newTmpDirs;
  }

  void TmpDir::moveFiles(QTemporaryDir& dist, QTemporaryDir& src) {
    QDir srcDir(src.path());
    auto distPath = dist.path();

    for (const auto& file : srcDir.entryInfoList(QDir::NoDotAndDotDot)) {
      srcDir.rename(file.path(), distPath + "/" + file.fileName());
    }
  }
} // namespace octane::gui