# Experiment 02 — ESP8266 (NodeMCU v2): LED class

PlatformIO firmware for **Espressif ESP8266** on **NodeMCU v2** (`nodemcuv2`), **Arduino** framework, **GNU C++20**.

This sketch wraps GPIO output in a small **`Led`** class: **`externalLed`** uses **GPIO 5** (often labelled **D1** on NodeMCU boards). `setup()` configures the pin; `loop()` turns the LED on for 1.5 s, then off for 1.5 s.

---

## Layout

| Path | Purpose |
|------|---------|
| `platformio.ini` | Board, platform, framework, C++ standard |
| `src/main.cpp` | Firmware (`Led` class + `setup` / `loop`) |
| `include/` | Shared headers (optional) |
| `lib/` | Private libraries |
| `test/` | PlatformIO tests |
| `.clangd` | clangd / Neovim settings (same strategy as `exp_01`) |

---

## Prerequisites

- **PlatformIO Core** on your `PATH` and USB **`dialout`** access — see the repository root **`README.md`**.
- Hardware: NodeMCU-class ESP8266, breadboard **LED + current-limiting resistor** (e.g. 220 Ω–1 kΩ), jumper wires. Treat I/O as **3.3 V**.

**Wiring (typical active-high LED):** GPIO **5** → resistor → LED anode → LED cathode → **GND**. If nothing blinks, confirm pin mapping for your board and that you are not driving a pin conflict (some onboard LEDs use other GPIOs).

---

## Build, upload, monitor

Run from **`exp_02/`**:

```bash
pio run
pio run --target upload
pio device monitor
```

If upload fails to find the port:

```bash
pio run --target upload --upload-port /dev/ttyUSB0
```

Machine-local serial settings can go in **`platformio.local.ini`** (gitignored at repo root).

---

## Neovim / clangd (LSP)

This project uses **`compile_commands.json`** (from PlatformIO) plus **`.clangd`** so clangd parses as host C++ while your firmware still builds with **Xtensa GCC**.

1. After changing **`platformio.ini`**, libraries, or includes:

   ```bash
   cd ~/Embedded/exp_02
   pio run -t compiledb
   ```

2. Restart clangd in Neovim (**`:LspRestart`**).

For a **full step-by-step** (install `clangd`, checks, troubleshooting), use **`exp_01/README.md`** → *Step-by-step: Neovim + clangd*. The configuration pattern is the same as in **`exp_01`**.

---

## Configuration notes

- **Board:** `nodemcuv2` (`platformio.ini`).
- **C++ standard:** C++20 via `build_flags` / `build_unflags`.

---

## References

- [PlatformIO ESP8266](https://docs.platformio.org/en/latest/platforms/espressif8266.html)
- [Project configuration `platformio.ini`](https://docs.platformio.org/en/latest/projectconf/index.html)
