#include <ThunderAuto/platform/platform_linux.h>

PlatformLinux::PlatformLinux(GLFWwindow* window)
  : PlatformImpl(window) {}

std::string
PlatformLinux::open_file_dialog(FileType type,
                                const FileExtensionList& extensions) {
  // TODO: Implement.
  return "";
}

std::string
PlatformLinux::save_file_dialog(const FileExtensionList& extensions) {
  // TODO: Implement.
  return "";
}
