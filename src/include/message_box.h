#ifndef OCTANE_GUI_CLIENT_MESSAGE_BOX_H_
#define OCTANE_GUI_CLIENT_MESSAGE_BOX_H_

#include <error_response.h>

#include <QtWidgets/QWidget>

namespace octane::gui {
  void openCritical(QWidget* parent, const QString& err);
  void openCritical(QWidget* parent, const ErrorResponse& err);
} // namespace octane::gui

#endif // OCTANE_GUI_CLIENT_MESSAGE_BOX_H_