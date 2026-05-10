# ESP8266 RTOS SDK on Debian 13 with Neovim 0.12

Pure **C/C++** development for **ESP8266** using the official **ESP8266_RTOS_SDK** (FreeRTOS, IDF-style build with `idf.py` and CMake). This is **not** the lighter **Arduino + PlatformIO** path used elsewhere in this repository; it is the **native RTOS SDK** from Espressif.

**Target system:** Debian 13 (Trixie)  
**Editor:** Neovim **0.12** with **clangd** for navigation and diagnostics  

---

## 1. What you are installing

| Piece | Role |
|-------|------|
| **ESP8266_RTOS_SDK** | Headers, components, FreeRTOS, CMake/`idf.py` workflow ([repo](https://github.com/espressif/ESP8266_RTOS_SDK)). |
| **Xtensa toolchain** | `xtensa-lx106-elf-gcc` etc., pulled in by the SDK’s **`install.sh`**. |
| **clangd** | Language server; needs **`compile_commands.json`** from the build directory. |

---

## 2. System prerequisites

Install build tools, Python, CMake/Ninja, SSL/libffi (used by tooling), and utilities for USB flashing:

```bash
sudo apt update
sudo apt install --yes \
  git wget curl flex bison gperf \
  python3 python3-pip python3-venv python3-setuptools \
  cmake ninja-build ccache \
  libffi-dev libssl-dev dfu-util libusb-1.0-0
```

**Notes:**

- **`python3-venv`** is recommended so SDK installers can use isolated environments cleanly.
- **`dialout`** (below) is separate — install packages first, then fix serial permissions.

---

## 3. Install ESP8266_RTOS_SDK

Install the SDK under a dedicated tree (here **`~/esp`**):

```bash
mkdir -p ~/esp
cd ~/esp
git clone --recursive https://github.com/espressif/ESP8266_RTOS_SDK.git
cd ESP8266_RTOS_SDK
./install.sh
```

- **`--recursive`** pulls submodules; required for a complete SDK.
- **`./install.sh`** downloads the **Xtensa** cross-compiler and sets up the Python environment the SDK expects. Follow any on-screen instructions (e.g. add `export` to your shell if the script suggests it).

**Official documentation:** [ESP8266_RTOS_SDK programming guide (IDF style)](https://docs.espressif.com/projects/esp8266-rtos-sdk/en/latest/)

---

## 4. Environment configuration (per shell)

The SDK provides **`export.sh`** to put the toolchain and **`IDF_PATH`** on your `PATH`. To avoid polluting every login session, use an alias:

```bash
# ~/.bashrc or ~/.zshrc — ESP8266 RTOS SDK
alias get_esp8266='. "$HOME/esp/ESP8266_RTOS_SDK/export.sh"'
```

Reload your shell:

```bash
source ~/.bashrc   # or: source ~/.zshrc
```

Before building or flashing, run:

```bash
get_esp8266
```

---

## 5. USB serial permissions

Your user must be in the **`dialout`** group to use **`/dev/ttyUSB0`** (typical USB‑UART) without `sudo`:

```bash
sudo usermod -a -G dialout "$USER"
```

**You must log out and log back in** (or reboot) for the new group to apply. Confirm:

```bash
groups | grep -o dialout
```

---

## 6. Project layout (CMake / `idf.py`)

A minimal IDF-style project looks like:

```text
project_root/
├── CMakeLists.txt          # Top-level project
├── sdkconfig               # Kconfig output (from menuconfig / defaults)
└── main/
    ├── CMakeLists.txt      # “main” component
    └── main.cpp            # Application source (C++)
```

Create a new project by copying one of the SDK **examples** or following the **Get Started** guide — do not hand-roll CMake unless you know the required ESP-IDF CMake glue:

- [Get Started](https://docs.espressif.com/projects/esp8266-rtos-sdk/en/latest/get-started/index.html)

---

## 7. C++ entry point (`main/main.cpp`)

`app_main` is the RTOS entry symbol and must use **C linkage** when compiled as C++:

```cpp
#include <cstdio>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

extern "C" void app_main(void)
{
    printf("Initializing system via C++...\n");

    for (;;) {
        printf("Hello from Neovim on Debian 13\n");
        vTaskDelay(pdMS_TO_TICKS(2000));
    }
}
```

**Notes:**

- Prefer **`printf`** / **`cstdio`** on embedded targets; **`<iostream>`** pulls in more of the C++ runtime and increases flash/RAM use.
- **`void`** in **`app_main(void)`** matches typical SDK examples.

---

## 8. Build, flash, and monitor

In **project_root**, after **`get_esp8266`**:

```bash
cd /path/to/your/project
get_esp8266

idf.py menuconfig                  # optional: serial port default, flash size, Wi-Fi, …
idf.py build
idf.py -p /dev/ttyUSB0 flash
idf.py -p /dev/ttyUSB0 monitor     # serial console; Ctrl+] to exit (same convention as ESP-IDF monitor)
```

**Legacy `make` workflow:** Older docs use **`make menuconfig`**, **`make flash`**, **`make monitor`** after sourcing **`export.sh`**. That still works in many trees; **IDF-style** projects prefer **`idf.py`** ([`tools/idf.py`](https://github.com/espressif/ESP8266_RTOS_SDK/blob/master/tools/idf.py) in the SDK).

**Common variants:**

| Command | Purpose |
|--------|---------|
| `idf.py fullclean` | Remove the build directory for a clean rebuild. |
| `idf.py flash monitor` | Build, flash, and attach the serial monitor (exact subcommands depend on SDK version — check **`idf.py --help`**). |

**Port:** Adjust **`/dev/ttyUSB0`** to match your board (`dmesg`, or **`ls /dev/ttyUSB*`**).

**Python deps:** The programming guide also installs packages from **`$IDF_PATH/requirements.txt`** — run **`install.sh`** first; if **`idf.py`** complains about missing modules, use:

```bash
python3 -m pip install --user -r "$HOME/esp/ESP8266_RTOS_SDK/requirements.txt"
```

---

## 9. Neovim 0.12 + clangd (`compile_commands.json`)

Install **clangd** (Debian package name `clangd`):

```bash
sudo apt install clangd
```

CMake can emit a [compilation database](https://clang.llvm.org/docs/JSONCompilationDatabase.html) for the LSP. After a successful configure/build, **`build/compile_commands.json`** is usually generated. Expose it at the project root so **clangd** finds it without extra config:

```bash
get_esp8266
cd /path/to/your/project
idf.py reconfigure    # or: idf.py build (generates / refreshes the build dir)
ln -sf build/compile_commands.json .
```

If **`compile_commands.json`** is missing, run **`idf.py build`** once (CMake export behaviour depends on SDK version). Alternatively point **clangd** at **`build/`** with a **`.clangd`** file:

```yaml
CompileFlags:
  CompilationDatabase: build
```

Restart the LSP in Neovim (e.g. **`:LspRestart`**) after regenerating the database.

---

## 10. Reference links

| Resource | URL |
|----------|-----|
| **ESP8266_RTOS_SDK (GitHub)** | <https://github.com/espressif/ESP8266_RTOS_SDK> |
| **SDK examples (GitHub)** | <https://github.com/espressif/ESP8266_RTOS_SDK/tree/master/examples> |
| **hello_world example** | <https://github.com/espressif/ESP8266_RTOS_SDK/tree/master/examples/get-started/hello_world> |
| **Programming guide (IDF style)** | <https://docs.espressif.com/projects/esp8266-rtos-sdk/en/latest/> |
| **Linux setup (official)** | <https://docs.espressif.com/projects/esp8266-rtos-sdk/en/latest/get-started/linux-setup.html> |
| **ESP8266EX overview** | <https://www.espressif.com/en/products/socs/esp8266ex/overview> |
| **FreeRTOS (ESP port context)** | See API Reference in the programming guide |
| **clangd** | <https://clangd.llvm.org/> |
| **JSON compilation database** | <https://clang.llvm.org/docs/JSONCompilationDatabase.html> |

---

## 11. Relationship to this repository

The **`exp_*` PlatformIO** projects in this tree use the **Arduino** framework on ESP8266, not **ESP8266_RTOS_SDK**. This document is a **standalone** reference for a **native RTOS SDK** workflow on the same class of hardware.

---

*Last updated for Debian 13, Neovim 0.12, and ESP8266_RTOS_SDK (IDF-style). Adjust paths (`~/esp`) and serial ports to match your machine.*
