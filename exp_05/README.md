# Experiment 05 — ESP8266: HSL bus times on SSD1306 (GraphQL)

PlatformIO firmware for **NodeMCU v2** (`nodemcuv2`), **Arduino** framework, **GNU C++20**.

The sketch connects to **Wi-Fi**, syncs time with **NTP**, calls the **Digitransit / HSL GraphQL** (**v2** routing endpoint) over **HTTPS**, parses the JSON with **ArduinoJson 7**, and shows the next departures for the stop configured in **`include/queries.h`** (default: **Von Daehnin katu 14**, `gtfsId` **`HSL:1362151`**) on a **128×64 SSD1306** OLED (I²C **0x3C**).

**Note:** The old **v1** URL `…/routing/v1/routers/hsl/index/graphql` returns **HTTP 404** (“endpoint has been removed”). If you see **API Error** with code **404**, update the URL to **v2** as in `src/main.cpp`.

---

## Credentials (required before build)

**`include/secrets.h` is gitignored** so keys and your Wi-Fi password are not committed.

1. Copy the template:

   ```bash
   cd ~/Embedded/exp_05
   cp include/secrets.example.h include/secrets.h
   ```

2. Edit **`include/secrets.h`** and set **`SECRET_SSID`**, **`SECRET_PASS`**, and **`HSL_API_KEY`** (Digitransit subscription key from [Digitransit](https://digitransit.fi/)).

Without **`include/secrets.h`**, the build will fail on missing headers.

---

## Dependencies (`platformio.ini`)

| Library | Role |
|---------|------|
| [Adafruit SSD1306](https://registry.platformio.org/libraries/adafruit/Adafruit%20SSD1306) + GFX | OLED drawing |
| [ArduinoJson](https://registry.platformio.org/libraries/bblanchon/ArduinoJson) | Parse GraphQL JSON |
| [NTPClient](https://registry.platformio.org/libraries/arduino-libraries/NTPClient) | Local time for “minutes until arrival” |
| ESP8266 core | **Wi-Fi**, **BearSSL** (`WiFiClientSecure`), **HTTPClient** |

---

## Layout

| Path | Purpose |
|------|---------|
| `platformio.ini` | Board, `lib_deps`, C++20 |
| `src/main.cpp` | Wi-Fi, NTP, HTTPS POST, JSON, OLED UI |
| `include/queries.h` | GraphQL query (`namespace hsl`) |
| `include/secrets.example.h` | Template for secrets |
| `include/secrets.h` | **Local only** (gitignored) — your SSID / password / API key |
| `.clangd` | Host **clang++** parse + **`PSTR`** shim + narrow suppressions for ESP8266 + Json noise |

---

## Build, upload, monitor

```bash
cd ~/Embedded/exp_05
pio run
pio run --target upload
pio device monitor
```

After changing **`platformio.ini`** or **`lib_deps`**:

```bash
pio run
pio run -t compiledb
```

Then restart clangd (**`:LspRestart`**).

---

## Neovim / clangd (LSP)

Same workflow as **`exp_01`** → *Step-by-step: Neovim + clangd*.

**Why extra `Diagnostics / Suppress` entries?** This project uses **HTTPS**, **BearSSL `WiFiClientSecure`**, **ArduinoJson v7**, and **`JsonDocument`**. When clangd retargets the compile command to **host `clang++`**, it sometimes mis-resolves overloads and base-class references that **xtensa-g++** handles correctly. The firmware build is authoritative; the suppressions only quiet those **editor-only** false positives. **`PSTR(s)=(s)`** keeps **`F("...")`** usable under clangd, as in **`exp_04`**.

---

## Behaviour notes

- **TLS:** `WiFiClientSecure::setInsecure()` is used for simplicity (no certificate pin). Tighten this for production if you care about MITM risk.
- **NTP offset:** `kHelsinkiOffsetSeconds` is **UTC+3** (summer-oriented). Adjust or use proper timezone rules if you need winter (EET) correctness.
- **GraphQL:** Headers match the original sketch (`Content-Type: application/graphql`, subscription key). If the API changes, update **`queries.h`** and parsing in **`fetch_and_parse_json`**.

---

## References

- [PlatformIO ESP8266](https://docs.platformio.org/en/latest/platforms/espressif8266.html)
- [Digitransit routing API](https://digitransit.fi/en/developers/apis/1-routing-api/)
- [ArduinoJson v7](https://arduinojson.org/v7/how-to/deserialize-a-document/)
