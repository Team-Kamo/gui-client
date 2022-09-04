#include "include/message_box.h"

#include <QApplication>
#include <QMessageBox>

namespace octane::gui {
  void openCritical(QWidget* parent, const QString& err) {
    QMessageBox::critical(parent, qApp->tr("Error"), err);
  }
  void openCritical(QWidget* parent, const ErrorResponse& err) {
    QMessageBox::critical(parent,
                          qApp->tr("Error"),
                          qApp->tr((err.code + "\n" + err.reason).c_str()));
  }
} // namespace octane::gui