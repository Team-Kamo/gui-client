#include "include/key_config_panel.h"

#include <QApplication>
#include <QMessageBox>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>

#include "include/config.h"
#include "include/settings.h"

namespace octane::gui {
  KeyConfigPanel::KeyConfigPanel(QWidget* parent) : QWidget(parent) {
    initLayout();
  }

  void KeyConfigPanel::initLayout() {
    auto rootLayout = new QVBoxLayout(this);

    auto form = new QFormLayout();

    auto copyFromSelectionLabel = new QLabel("選択からコピー");
    auto copyFromSelectionInput = new QLineEdit();
    copyFromSelectionInput->setText(
      Settings::getAsStr(SETTING_KEY_KEYMAP_COPY_FROM_SELECTION));
    Settings::watchAsStr(
      SETTING_KEY_KEYMAP_COPY_FROM_SELECTION,
      [=](const auto& hotkey) { copyFromSelectionInput->setText(hotkey); });

    form->addRow(copyFromSelectionLabel, copyFromSelectionInput);

    auto copyFromClipboardLabel = new QLabel("クリップボードからコピー");
    auto copyFromClipboardInput = new QLineEdit();
    copyFromClipboardInput->setText(
      Settings::getAsStr(SETTING_KEY_KEYMAP_COPY_FROM_CLIPBOARD));
    Settings::watchAsStr(
      SETTING_KEY_KEYMAP_COPY_FROM_CLIPBOARD,
      [=](const auto& hotkey) { copyFromClipboardInput->setText(hotkey); });

    form->addRow(copyFromClipboardLabel, copyFromClipboardInput);

    auto pasteToSelectionLabel = new QLabel("選択へペースト");
    auto pasteToSelectionInput = new QLineEdit();
    pasteToSelectionInput->setText(
      Settings::getAsStr(SETTING_KEY_KEYMAP_PASTE_TO_SELECTION));
    Settings::watchAsStr(
      SETTING_KEY_KEYMAP_PASTE_TO_SELECTION,
      [=](const auto& hotkey) { pasteToSelectionInput->setText(hotkey); });

    form->addRow(pasteToSelectionLabel, pasteToSelectionInput);

    auto pasteToClipboardLabel = new QLabel("クリップボードへペースト");
    auto pasteToClipboardInput = new QLineEdit();
    pasteToClipboardInput->setText(
      Settings::getAsStr(SETTING_KEY_KEYMAP_PASTE_TO_CLIPBOARD));
    Settings::watchAsStr(
      SETTING_KEY_KEYMAP_PASTE_TO_CLIPBOARD,
      [=](const auto& hotkey) { pasteToClipboardInput->setText(hotkey); });

    form->addRow(pasteToClipboardLabel, pasteToClipboardInput);

    auto buttonRow = new QHBoxLayout();

    auto resetBtn  = new QPushButton("初期化");
    auto cancelBtn = new QPushButton("取消");
    auto saveBtn   = new QPushButton("保存");

    QObject::connect(resetBtn, &QPushButton::clicked, qApp, [=]() {
      if (QMessageBox::question(
            this, tr("確認"), tr("キー設定を初期化します。よろしいですか？"))
          == QMessageBox::Yes) {
        Settings::setAsStr(SETTING_KEY_KEYMAP_COPY_FROM_SELECTION,
                           SETTING_DEFAULT_VALUE_KEYMAP_COPY_FROM_SELECTION);
        Settings::setAsStr(SETTING_KEY_KEYMAP_COPY_FROM_CLIPBOARD,
                           SETTING_DEFAULT_VALUE_KEYMAP_COPY_FROM_CLIPBOARD);
        Settings::setAsStr(SETTING_KEY_KEYMAP_PASTE_TO_SELECTION,
                           SETTING_DEFAULT_VALUE_KEYMAP_PASTE_TO_SELECTION);
        Settings::setAsStr(SETTING_KEY_KEYMAP_PASTE_TO_CLIPBOARD,
                           SETTING_DEFAULT_VALUE_KEYMAP_PASTE_TO_CLIPBOARD);
      }
    });
    QObject::connect(cancelBtn, &QPushButton::clicked, qApp, [=]() {
      Settings::setAsStr(
        SETTING_KEY_KEYMAP_COPY_FROM_SELECTION,
        Settings::getAsStr(SETTING_KEY_KEYMAP_COPY_FROM_SELECTION));
      Settings::setAsStr(
        SETTING_KEY_KEYMAP_COPY_FROM_CLIPBOARD,
        Settings::getAsStr(SETTING_DEFAULT_VALUE_KEYMAP_COPY_FROM_CLIPBOARD));
      Settings::setAsStr(
        SETTING_KEY_KEYMAP_PASTE_TO_SELECTION,
        Settings::getAsStr(SETTING_DEFAULT_VALUE_KEYMAP_PASTE_TO_SELECTION));
      Settings::setAsStr(
        SETTING_KEY_KEYMAP_PASTE_TO_CLIPBOARD,
        Settings::getAsStr(SETTING_DEFAULT_VALUE_KEYMAP_PASTE_TO_CLIPBOARD));
    });
    QObject::connect(saveBtn, &QPushButton::clicked, qApp, [=]() {
      Settings::setAsStr(SETTING_KEY_KEYMAP_COPY_FROM_SELECTION,
                         copyFromSelectionInput->text());
      Settings::setAsStr(SETTING_KEY_KEYMAP_COPY_FROM_CLIPBOARD,
                         copyFromClipboardInput->text());
      Settings::setAsStr(SETTING_KEY_KEYMAP_PASTE_TO_SELECTION,
                         pasteToSelectionInput->text());
      Settings::setAsStr(SETTING_KEY_KEYMAP_PASTE_TO_CLIPBOARD,
                         pasteToClipboardInput->text());
    });

    buttonRow->addWidget(resetBtn);
    buttonRow->addWidget(cancelBtn);
    buttonRow->addWidget(saveBtn);

    form->addRow(buttonRow);

    rootLayout->addLayout(form);
  }
} // namespace octane::gui