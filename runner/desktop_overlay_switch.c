/* Nintendo Switch overlay stub for DKC2Recomp.
 *
 * The desktop overlay is Dear ImGui on an OpenGL 3 context
 * (desktop_overlay.cpp), which has no Switch backend here, and the Switch
 * build runs launcher-free by design ("no intermediary"). Every
 * Dkc2DesktopOverlay* entry point is therefore a NULL-tolerant no-op that
 * keeps the game loop, settings flow, and presenter calls in sdl_main.c
 * compiling unchanged: the overlay is never open, takes no actions, and
 * draws nothing.
 *
 * Only compiled on __SWITCH__; desktop builds keep desktop_overlay.cpp.
 */
#ifdef __SWITCH__

#include "desktop_overlay.h"

#include <stddef.h>
#include <string.h>

/* The host applies overlay settings every frame (ApplyOverlaySettings in
 * sdl_main.c), so GetSettings must return coherent values, not stack
 * garbage: a trashed player_src is exactly how "controls not present"
 * happens. The stub round-trips the live settings through a snapshot. */
static RecompLauncherCSettings s_settings;
static bool s_have_settings;

Dkc2DesktopOverlay *Dkc2DesktopOverlayCreate(
    const RecompLauncherCSettings *settings) {
  if (settings) {
    s_settings = *settings;
    s_have_settings = true;
  }
  /* Non-NULL dummy: sdl_main.c treats NULL as a fatal init failure.
   * The overlay is still never open (IsOpen is always false) and draws
   * nothing. */
  static int s_dummy;
  return (Dkc2DesktopOverlay *)&s_dummy;
}

bool Dkc2DesktopOverlayInitSdl(Dkc2DesktopOverlay *overlay, void *window,
                               void *gl_context) {
  (void)overlay;
  (void)window;
  (void)gl_context;
  /* Must succeed: sdl_main.c treats false as fatal. */
  return true;
}

bool Dkc2DesktopOverlayInitWin32(Dkc2DesktopOverlay *overlay, void *window) {
  (void)overlay;
  (void)window;
  return true;
}

void Dkc2DesktopOverlayDestroy(Dkc2DesktopOverlay *overlay) {
  (void)overlay;
}

bool Dkc2DesktopOverlayProcessSdlEvent(Dkc2DesktopOverlay *overlay,
                                       const void *event) {
  (void)overlay;
  (void)event;
  return false;
}

bool Dkc2DesktopOverlayProcessWin32Message(Dkc2DesktopOverlay *overlay,
                                           void *window, unsigned message,
                                           uintptr_t wparam,
                                           intptr_t lparam) {
  (void)overlay;
  (void)window;
  (void)message;
  (void)wparam;
  (void)lparam;
  return false;
}

void Dkc2DesktopOverlaySetGamepad(Dkc2DesktopOverlay *overlay,
                                  const Dkc2GamepadState *gamepad) {
  (void)overlay;
  (void)gamepad;
}

void Dkc2DesktopOverlayToggle(Dkc2DesktopOverlay *overlay) {
  (void)overlay;
}

bool Dkc2DesktopOverlayIsOpen(const Dkc2DesktopOverlay *overlay) {
  (void)overlay;
  return false;
}

bool Dkc2DesktopOverlayAssistTools(const Dkc2DesktopOverlay *overlay) {
  (void)overlay;
  return false;
}

int Dkc2DesktopOverlaySelectedSlot(const Dkc2DesktopOverlay *overlay) {
  (void)overlay;
  return 0;
}

void Dkc2DesktopOverlayGetSettings(const Dkc2DesktopOverlay *overlay,
                                   RecompLauncherCSettings *settings) {
  (void)overlay;
  if (settings && s_have_settings) *settings = s_settings;
}

void Dkc2DesktopOverlaySetSettings(Dkc2DesktopOverlay *overlay,
                                   const RecompLauncherCSettings *settings) {
  (void)overlay;
  if (settings) {
    s_settings = *settings;
    s_have_settings = true;
  }
}

uint32_t Dkc2DesktopOverlayTakeActions(Dkc2DesktopOverlay *overlay) {
  (void)overlay;
  return 0;
}

void Dkc2DesktopOverlaySetStatus(Dkc2DesktopOverlay *overlay,
                                 const char *status, bool success) {
  (void)overlay;
  (void)status;
  (void)success;
}

void Dkc2DesktopOverlayRenderOpenGl(void *overlay, int width, int height) {
  (void)overlay;
  (void)width;
  (void)height;
}

#endif /* __SWITCH__ */
