# Nintendo Switch port (libnx + SDL2 + OpenGL ES2)

Homebrew target for DKC2Recomp. Boots straight into the game — no
pre-boot launcher, no overlay, no diagnostics bundles.

## Build

Prerequisites: devkitPro with devkitA64, libnx, and switch portlibs SDL2.

```sh
# 1. Generate the private recomp units once (needs the USA v1.0 ROM;
#    never committed — generated/ is gitignored):
python scripts/generate_snesrecomp.py --rom <path>/dkc2.smc

# 2. Build the homebrew:
make -f Makefile.switch -j$(nproc)
# -> dkc2.nro + dkc2.nacp
```

Without step 1 the link stops at four undefined symbols
(`g_dispatch_table[_count]`, `g_ram_routine_guard[_count]`); everything
else compiles and links. That is the expected no-ROM state.

## Install

- Copy `dkc2.nro` to `sdmc:/switch/dkc2/`.
- Copy the legally obtained ROM to `sdmc:/switch/dkc2/rom.smc`
  (`rom.sfc` also accepted; a 512-byte copier header is stripped and the
  payload is verified as USA v1.0 before boot).
- First boot writes `config.ini`/`launcher.cfg` defaults; `saves/` holds
  SRAM and savestates. A missing ROM writes `MISSING_ROM.txt` next to
  the NRO instead of showing a picker.
- Debugging without nxlink: create an empty `sdmc:/switch/dkc2/log.flag`
  and stderr (breadcrumbs, warnings, fatals) is redirected, unbuffered,
  to `sdmc:/switch/dkc2/debug.log`, truncated each boot. Delete the flag
  for normal silent runs.

## Controls (fixed, seamless)

Player 1 autodetects the first attached controller; player 2 is off.

| Switch | SNES |
|---|---|
| A / B / X / Y (physical positions) | A / B / X / Y |
| ZL / ZR | L / R |
| Plus (+) | Start |
| Minus (−) | Select |
| DPad, left stick | DPad |

ZL/ZR are digital on Switch hardware and reported as full-scale trigger
axes, which is what the shared L/R trigger bindings match. Assist
shortcuts (rewind/fast-forward on triggers) are cleared on Switch so ZL
never double-fires gameplay + rewind.

## Presentation

- Fixed 1280�--720 SDL window on the GLES2-accelerated renderer; the SNES
  frame (4:3 native, 16:9 widescreen default) is letterboxed by the
  logical-size scaler.
- 16:9 widescreen is forced on regardless of config: applied once at
  boot (`Dkc2VideoSetAspect` in the Switch path) and answered always-true
  by `Dkc2VideoIsWidescreen`. Bounded title/menu screens still center via
  the per-frame scene policy, which keeps following live camera/HDMA
  state every frame (freezing that would smear margins while scrolling).
- The Reconstruct GLSL upscaler is unavailable (no desktop GL context);
  nearest/bilinear samplers apply, with the fallback reported the same
  way the desktop presenter reports a failed shader build.
- VSync is on; frame deadlines stay on the host 60.0988 Hz clock.

## Deliberately excluded on Switch

recomp-ui launcher, Dear ImGui overlay (`runner/desktop_overlay_switch.c`
is a no-op stub), diagnostics reports/bundles
(`runner/diagnostics_switch.c` stub; breadcrumbs still reach stderr, i.e.
the nxlink USB log), TCP debug server, co-sim, oracle, mods, desktop GL
presenter (`runner/desktop_present_sdl.c`), macOS/Win32 hosts.

## Files

| Path | Role |
|---|---|
| `Makefile.switch` | devkitA64 makefile (includes the framework fragment, drops five newer-than-pin framework units nothing references, forces SDL2/no-launcher defines, `--gc-sections` so the uncalled launcher entry and its recomp-ui references leave the link) |
| `runner/sdl_main.c` (`__SWITCH__`) | SD bring-up, `sdmc:/switch/dkc2/rom.smc` resolution, forced handheld settings (P1 gamepad, 16:9, Switch binds), applet tick, raw-joystick input path |
| `runner/switch_gamepad.c` | HID-order raw joystick reader (A/B/X/Y, ZL/ZR→triggers, ±→Start/Select, stick-as-DPad) |
| `runner/desktop_present_switch.c` | `Dkc2SdlPresenter` over `SDL_Renderer` |
| `runner/desktop_overlay_switch.c` | no-op overlay |
| `runner/diagnostics_switch.c` | no-op diagnostics |
| `snesrecomp/runner/src/switch/` | framework: SD bring-up, applet tick, ROM resolver (new files in the pin) |
| `snesrecomp/runner/switch.mk` | framework source fragment for make builds |
| `snesrecomp/runner/src/host_report.c` | two `__SWITCH__` guards (no `sys/utsname.h` on newlib; static horizon identity) |
