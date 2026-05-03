# Experiment 03 — ESP8266 (NodeMCU v2): RGB LED

PlatformIO firmware for **Espressif ESP8266** on **NodeMCU v2** (`nodemcuv2`), **Arduino** framework, **GNU C++20**.

This sketch drives a **through-hole or module RGB LED** (three channels) with a small **`RGBLed`** type and an **`enum class Light`**. **Common-anode** wiring is assumed: the enum maps **On → `LOW`** and **Off → `HIGH`** (cathodes pulled to the GPIOs through current-limiting resistors). `loop()` steps **red → green → blue**, one second on each color.

**GPIO mapping (comments in `main.cpp`):**

| Channel | NodeMCU label | GPIO |
|---------|----------------|------|
| Red | D1 | 5 |
| Green | D2 | 4 |
| Blue | D3 | 0 |

Adjust **`RGBLed statusLed(5, 4, 0)`** if your wiring differs.

---

## Layout

| Path | Purpose |
|------|---------|
| `platformio.ini` | Board, platform, framework, C++ standard |
| `src/main.cpp` | `RGBLed`, `RGBState`, `Light`, `setup` / `loop` |
| `include/` | Shared headers (optional) |
| `lib/` | Private libraries |
| `test/` | PlatformIO tests |
| `.clangd` | clangd / Neovim settings (same strategy as `exp_01` / `exp_02`) |

---

## Prerequisites

- **PlatformIO Core** on your `PATH` and USB **`dialout`** access — see the repository root **`README.md`**.
- Hardware: NodeMCU, **RGB LED** (or RGB module), **three resistors** (e.g. 220 Ω–1 kΩ per channel), breadboard and jumpers. Treat I/O as **3.3 V**.

**Wiring (common anode, typical):** Connect the LED **anode** to **3V3** (or use your module’s **VCC** as documented). Connect **R / G / B** cathodes through resistors to **GPIO 5, 4, and 0** respectively, matching `RGBLed(5, 4, 0)`. For **common-cathode** LEDs, invert the `Light` enum values (`On = HIGH`, `Off = LOW`) or adjust `digitalWrite` logic.

---

## Build, upload, monitor

Run from **`exp_03/`**:

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

This project uses **`compile_commands.json`** (from PlatformIO) plus **`.clangd`** so clangd parses as host C++ while the firmware still builds with **Xtensa GCC**.

1. After changing **`platformio.ini`**, libraries, or includes:

   ```bash
   cd ~/Embedded/exp_03
   pio run -t compiledb
   ```

2. Restart clangd in Neovim (**`:LspRestart`**).

For a **full step-by-step** (install `clangd`, checks, troubleshooting), use **`exp_01/README.md`** → *Step-by-step: Neovim + clangd*. The configuration pattern matches **`exp_01`** and **`exp_02`**.

---

## Configuration notes

- **Board:** `nodemcuv2` (`platformio.ini`).
- **C++ standard:** C++20 via `build_flags` / `build_unflags` in `platformio.ini`.

---

## References

- [PlatformIO ESP8266](https://docs.platformio.org/en/latest/platforms/espressif8266.html)
- [Project configuration `platformio.ini`](https://docs.platformio.org/en/latest/projectconf/index.html)
