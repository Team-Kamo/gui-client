#include "include/osx_clipboard_manager.h"

#include <ApplicationServices/ApplicationServices.h>
#include <Carbon/Carbon.h>

#include <QTimerEvent>
#include <thread>

#include "../../include/tmp_dir.h"

namespace octane::gui::darwin {
  OsxClipboardManager::OsxClipboardManager() : observer(nullptr) {}
  OsxClipboardManager::~OsxClipboardManager() noexcept {
    if (observer) {
      observer->destroy();
    }
  }
  std::optional<ClipboardData> OsxClipboardManager::copyFromClipboard() {
    return getClipboardData();
  }
  void OsxClipboardManager::copyFromSelection(
    const std::function<void(ClipboardData &&)> &callback) {
    if (observer) {
      observer->destroy();
    }
    observer = new Observer(this, callback);

    CGEventSourceRef src = CGEventSourceCreate(
      CGEventSourceStateID(kCGEventSourceStateCombinedSessionState));
    CGEventRef cDown = CGEventCreateKeyboardEvent(src, kVK_ANSI_C, true);
    CGEventRef cUp   = CGEventCreateKeyboardEvent(src, kVK_ANSI_C, false);
    CGEventSetFlags(cDown, CGEventFlags(kCGEventFlagMaskCommand));
    CGEventSetFlags(cUp, CGEventFlags(kCGEventFlagMaskCommand));
    CGEventPost(kCGAnnotatedSessionEventTap, cDown);
    CGEventPost(kCGAnnotatedSessionEventTap, cUp);
    CFRelease(cDown);
    CFRelease(cUp);
    CFRelease(src);
  }
  void OsxClipboardManager::pasteToClipboard(const ClipboardData &data) {
    setClipboardData(data, TmpDir::makeTmpDir());
  }
  void OsxClipboardManager::pasteToSelection(const ClipboardData &data) {
    setClipboardData(data, TmpDir::makeTmpDir());

    CGEventSourceRef src = CGEventSourceCreate(
      CGEventSourceStateID(kCGEventSourceStateCombinedSessionState));
    CGEventRef vDown = CGEventCreateKeyboardEvent(src, kVK_ANSI_V, true);
    CGEventRef vUp   = CGEventCreateKeyboardEvent(src, kVK_ANSI_V, false);
    CGEventSetFlags(vDown, CGEventFlags(kCGEventFlagMaskCommand));
    CGEventSetFlags(vUp, CGEventFlags(kCGEventFlagMaskCommand));
    CGEventPost(kCGAnnotatedSessionEventTap, vDown);
    CGEventPost(kCGAnnotatedSessionEventTap, vUp);
    CFRelease(vDown);
    CFRelease(vUp);
    CFRelease(src);
  }
  bool OsxClipboardManager::nativeEventFilter(const QByteArray &eventType,
                                              void *message,
                                              qintptr *result) {
    return false;
  }

  OsxClipboardManager::Observer::Observer(
    OsxClipboardManager *parent,
    const std::function<void(ClipboardData &&)> &copyFromSelectionCallback)
    : QObject(nullptr),
      parent(parent),
      copyFromSelectionCallback(copyFromSelectionCallback),
      timerId(0),
      oldData(parent->getClipboardData()),
      count(0),
      limit(5) {
    timerId = startTimer(100);
  }
  void OsxClipboardManager::Observer::timerEvent(QTimerEvent *e) {
    if (e->timerId() != timerId) return;
    auto data = parent->getClipboardData();
    if (data && oldData != data) {
      copyFromSelectionCallback(std::move(data.value()));
      destroy();
    }
    if (++count == limit) {
      if (data) {
        copyFromSelectionCallback(std::move(data.value()));
      }
      destroy();
    }
  }
  void OsxClipboardManager::Observer::destroy() {
    killTimer(timerId);
    parent->observer = nullptr;
    delete this;
  }
} // namespace octane::gui::darwin