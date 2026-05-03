# Embedded: ESP8266 + PlatformIO (Debian)

This repository documents a terminal-first workflow for developing on the **ESP8266** (e.g. NodeMCU / Amica) using **PlatformIO Core (CLI)** on **Debian 13**, without the Arduino IDE. It pairs well with Neovim and CMake-style project layouts.

---

## Hardware

| Component | Role | Notes |
|-----------|------|--------|
| **ESP8266** (NodeMCU / Amica) | Main MCU | ~80 MHz, built-in Wi-Fi; firmware runs as Xtensa machine code. |
| **OLED (SSD1306)** | Display | Typically **128 × 64** pixels over **I²C** (SDA/SCL + power). |
| **LEDs & resistors** | First “Hello World” | Blinking an LED proves toolchain + GPIO. |
| **Breadboard & jumper wires** | Prototyping | Solderless connections; match 3.3 V logic for ESP8266. |

**Voltage:** The ESP8266 I/O is **3.3 V** tolerant. Do not drive GPIO above 3.3 V without level shifting.

---

## Phase 1: Toolchain Setup (PlatformIO Core)

PlatformIO handles cross-compilation for the Xtensa toolchain, dependency management, and uploading firmware over USB serial.

### 1. Install prerequisites and PlatformIO

```bash
# Install python3-venv if you haven't
sudo apt update && sudo apt install python3-venv curl

# Install PlatformIO Core
curl -fsSL https://raw.githubusercontent.com/platformio/platformio-core-installer/master/get-platformio.py | python3
```

### 2. Add PlatformIO to your `PATH`

Append the PlatformIO virtualenv `bin` directory so `pio` and related tools are available in every shell:

```bash
export PATH="$PATH:$HOME/.platformio/penv/bin"
```

Add that line to `~/.bashrc` or `~/.zshrc`, then reload the shell (`source ~/.bashrc`) or open a new terminal.

Verify:

```bash
pio --version
```

### 3. USB serial permissions (required)

So your user can open `/dev/ttyUSB*` or `/dev/ttyACM*` without `sudo`, add yourself to the **`dialout`** group:

```bash
sudo usermod -a -G dialout "$USER"
```

**You must log out and log back in** (or reboot) for the new group membership to apply. Until then, serial uploads may fail with “permission denied.”

Check groups after re-login:

```bash
groups
```

You should see `dialout` in the list.

### 4. Optional checks

- **Device node:** With the board connected via USB, list serial devices:

  ```bash
  ls -l /dev/ttyUSB* /dev/ttyACM* 2>/dev/null
  ```

- **PlatformIO home:** Default install root is `~/.platformio/` (toolchains, packages, project cache).

---

## Next steps (after Phase 1)

- Create a PlatformIO project targeting a board such as `nodemcuv2` (or your exact board ID).
- For the SSD1306, use an I²C library compatible with the Arduino framework on ESP8266 (many projects use Adafruit SSD1306 + Adafruit GFX, or similar).
- First sketch: blink the onboard LED (GPIO varies by board; NodeMCU often maps it clearly in the board docs).

---

## References

- [PlatformIO documentation](https://docs.platformio.org/)
- [ESP8266 Arduino core / boards](https://github.com/esp8266/Arduino)

---

## Document info

- **OS:** Debian 13  
- **Scope:** Phase 1 — CLI toolchain and serial access only; no Arduino IDE required.
