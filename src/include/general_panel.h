#ifndef OCTANE_GUI_CLIENT_GENERAL_PANEL_H_
#define OCTANE_GUI_CLIENT_GENERAL_PANEL_H_

#include <QtWidgets/QLineEdit>
#include <QtWidgets/QWidget>

namespace octane::gui {
  class GeneralPanel : public QWidget {
    QLineEdit* idInput;

  public:
    GeneralPanel(QWidget* parent);

  private:
    void initLayout();
  };
} // namespace octane::gui

#endif // OCTANE_GUI_CLIENT_GENERAL_PANEL_H_