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

## PlatformIO CLI cheat sheet

Run these from a **project directory** that contains `platformio.ini` (for example `exp_01/`, `exp_05/`), unless noted as global.

### Account / environment (optional)

| Command | Purpose |
|--------|---------|
| `pio account login` | Log in to PlatformIO (needed for some cloud or registry features). |
| `pio system info` | Show Python, platform, and PIO paths (useful when debugging installs). |
| `pio doctor` | Check the installation and suggest fixes. |

### New project (`pio init`)

| Command | Purpose |
|--------|---------|
| `pio project init --board nodemcuv2` | Create `platformio.ini` + default folders (`src/`, `lib/`, `include/`, `test/`) for **NodeMCU 1.0 (ESP-12E)**. |
| `pio project init --board nodemcuv2 --project-option "framework=arduino"` | Same, with framework pinned explicitly (if you need a non-default combo). |

After `init`, put firmware in **`src/`** (for example `src/main.cpp`).

### Build, clean, and firmware output

| Command | Purpose |
|--------|---------|
| `pio run` | **Compile** the default environment in `platformio.ini` (e.g. `[env:nodemcuv2]`). |
| `pio run -e nodemcuv2` | Build a **named** environment (`-e` = environment id). |
| `pio run -t clean` | Remove the build directory for that environment (forces a full rebuild next time). |
| `pio run -t size` | Show **flash/RAM** usage after a successful build. |

### Upload and serial monitor

| Command | Purpose |
|--------|---------|
| `pio run -t upload` | **Build (if needed) and flash** firmware over USB serial. |
| `pio run -t upload --upload-port /dev/ttyUSB0` | Upload to a **specific** port (use when auto-detection fails). |
| `pio device list` | List detected serial ports and USB devices. |
| `pio device monitor` | Open a **serial terminal** (default baud is often 115200; set in code with `Serial.begin`). |
| `pio device monitor -p /dev/ttyUSB0 -b 115200` | Monitor a chosen port and baud rate. |

Shortcut that builds, uploads, then opens the monitor:

```bash
pio run -t upload && pio device monitor
```

### Libraries and dependencies

| Command | Purpose |
|--------|---------|
| *(preferred)* Edit **`lib_deps`** in `platformio.ini` | Declares libraries for the project; `pio run` downloads them into `.pio/libdeps/`. |
| `pio pkg install` | Install packages according to `platformio.ini` (usually automatic on `pio run`). |
| `pio pkg list` | List installed packages for the project. |
| `pio pkg update` | Update dependencies (use when you want newer compatible versions). |

### Tests (when you add `test/`)

| Command | Purpose |
|--------|---------|
| `pio test` | Run **PlatformIO unit tests** for the active environments. |
| `pio test -e nodemcuv2` | Run tests for one environment only. |

### LSP / IDE integration (clangd, Neovim)

| Command | Purpose |
|--------|---------|
| `pio run -t compiledb` | Generate **`compile_commands.json`** in the project (and a symlink at the project root when configured). **Regenerate** after changing `platformio.ini`, `lib_deps`, or include paths. |

Individual experiments (`exp_01`, `exp_04`, `exp_05`, …) document **Neovim + clangd** details in their own `README.md` files.

### Useful global maintenance

| Command | Purpose |
|--------|---------|
| `pio upgrade` | Upgrade **PlatformIO Core** to the latest release. |
| `pio platform update espressif8266` | Update the **ESP8266** platform packages (toolchain, frameworks). |

---

## Learning projects in this repo

The **`exp_*`** directories are self-contained PlatformIO sketches (see each folder’s `README.md`):

- **`exp_01`** — toolchain + LSP walkthrough, basic firmware layout.  
- **`exp_02`–`exp_04`** — GPIO / RGB / OLED experiments.  
- **`exp_05`** — Wi-Fi, HTTPS, JSON, and OLED (Digitransit / HSL).

---

## Next steps (after Phase 1)

- Open an experiment folder, run **`pio run`**, then **`pio run -t upload`** with the board connected.
- For a new blank project from scratch: **`pio project init --board nodemcuv2`**, then add code under **`src/`**.
- For **SSD1306** and similar hardware, declare libraries in **`lib_deps`** in `platformio.ini` (see `exp_04` / `exp_05`).

---

## References

- [PlatformIO documentation](https://docs.platformio.org/)
- [ESP8266 Arduino core / boards](https://github.com/esp8266/Arduino)

---

## Document info

- **OS:** Debian 13  
- **Scope:** ESP8266 + **PlatformIO Core (CLI)** on Debian: install, USB permissions, and common **`pio`** commands used across the **`exp_*`** learning projects. No Arduino IDE required.
