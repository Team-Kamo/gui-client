#ifndef OCTANE_GUI_CLIENT_GLOBAL_TMP_DIR_H_
#define OCTANE_GUI_CLIENT_GLOBAL_TMP_DIR_H_

#include <QTemporaryDir>
#include <vector>

namespace octane::gui {
  class TmpDir {
    // NOTE: QTemporaryDirのムーブコンストラクタはQt6.4以降で使える。
    // そのためstd::vectorによる実装ができない。
    QTemporaryDir* tmpDirs;
    int64_t lastIndex;
    int64_t numTmpDirs;

    static TmpDir instance;

    void extend();
    void moveFiles(QTemporaryDir& dist, QTemporaryDir& src);

  public:
    TmpDir();
    ~TmpDir();
    static QTemporaryDir& makeTmpDir();
  };
} // namespace octane::gui

#endif // OCTANE_GUI_CLIENT_GLOBAL_TMP_DIR_H_