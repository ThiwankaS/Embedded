# Experiment 04 — ESP8266 (NodeMCU v2): SSD1306 OLED (I²C)

PlatformIO firmware for **Espressif ESP8266** on **NodeMCU v2** (`nodemcuv2`), **Arduino** framework, **GNU C++20**.

This sketch brings up a **128×64** monochrome **SSD1306** OLED over **I²C** using **Adafruit SSD1306** and **Adafruit GFX**. It initializes the display at **0x3C**, draws a short greeting with two text sizes, and refreshes the framebuffer once in **`setup()`**. **`loop()`** is left empty for you to extend.

---

## Dependencies (`platformio.ini`)

| Library | Role |
|---------|------|
| [Adafruit SSD1306](https://registry.platformio.org/libraries/adafruit/Adafruit%20SSD1306) | Driver for the OLED |
| [Adafruit GFX](https://registry.platformio.org/libraries/adafruit/Adafruit%20GFX%20Library) | Primitives and text (`println`, `setTextSize`, …) |

PlatformIO installs them under **`.pio/libdeps/`** (gitignored). After changing **`lib_deps`**, run **`pio run`** once so the LSP compilation database sees the new paths, then **`pio run -t compiledb`**.

---

## Layout

| Path | Purpose |
|------|---------|
| `platformio.ini` | Board, framework, C++20, **`lib_deps`** |
| `src/main.cpp` | Wire + SSD1306 init and demo text |
| `include/` | Your headers (optional) |
| `lib/` | Vendored libraries (optional) |
| `test/` | PlatformIO tests |
| `.clangd` | clangd settings (includes a host-only **`PSTR`** shim for `F()` — see below) |

---

## Prerequisites

- **PlatformIO Core**, **`dialout`** for USB serial — repository root **`README.md`**.
- **Hardware:** NodeMCU (or compatible ESP8266), **SSD1306 128×64** module (I²C), four wires: **3V3**, **GND**, **SDA**, **SCL**. On many NodeMCU boards, **D2 → GPIO 4** is **SDA** and **D1 → GPIO 5** is **SCL** for the default **`Wire`** instance (match your silkscreen / board docs).

**I²C address:** The sketch uses **`0x3C`**. If your module uses **`0x3D`**, change the second argument to **`display.begin(..., 0x3D)`**.

---

## Build, upload, monitor

From **`exp_04/`**:

```bash
pio run
pio run --target upload
pio device monitor
```

Upload port override if needed:

```bash
pio run --target upload --upload-port /dev/ttyUSB0
```

---

## Neovim / clangd (LSP)

1. After **`platformio.ini`**, **`lib_deps`**, or include layout changes:

   ```bash
   cd ~/Embedded/exp_04
   pio run
   pio run -t compiledb
   ```

2. Restart clangd (**`:LspRestart`**).

**`F("...")` / `PSTR`:** On the device, **`F()`** stores strings in flash via **`PSTR`** (from newlib **`sys/pgmspace.h`**). When clangd parses as **host `clang++`**, that include chain often does not line up with the PC toolchain, so **`.clangd`** adds **`-DPSTR(s)=(s)`** for the language server only. The firmware build is unchanged (Xtensa GCC still uses the real **`PSTR`**). Do not rely on this define for “correct” flash placement on the MCU — it is for editor diagnostics only.

Full LSP setup (install **`clangd`**, checks, troubleshooting): **`exp_01/README.md`** → *Step-by-step: Neovim + clangd*.

---

## Configuration notes

- **Board:** `nodemcuv2`.
- **C++20:** `build_flags` / `build_unflags` in `platformio.ini`.

---

## References

- [PlatformIO ESP8266](https://docs.platformio.org/en/latest/platforms/espressif8266.html)
- [Adafruit SSD1306 library](https://github.com/adafruit/Adafruit_SSD1306)
- [Project configuration `platformio.ini`](https://docs.platformio.org/en/latest/projectconf/index.html)
