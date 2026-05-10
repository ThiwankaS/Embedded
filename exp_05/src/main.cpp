// libraries for hardare, WiFi, Time and JSON parsing
#include <Adafruit_GFX.h>       // enable graphics for OLED
#include <Adafruit_SSD1306.h>   // OLED display driver
#include <Arduino.h>            // core arduino fucntionalities
#include <ArduinoJson.h>        // parse the http JSON response
#include <ESP8266HTTPClient.h>  // core http client libary for ESP8266
#include <ESP8266WiFi.h>        // core WiFi library for ESP8266
#include <NTPClient.h>          // to get the current time from the web
#include <WiFiClientSecure.h>   // for http connections
#include <WiFiUdp.h>            // required for NTP(time)
#include <Wire.h>               // I2C for SSD1306 (explicit; do not rely on transitive includes)

#include "queries.h"            // consist the GraphQL query string
#include "secrets.h"            // consist API key / WiFi SSD ID and Password

namespace {

    /** OLED display settings */
    constexpr int kScreenWidth              = 128;  // OLED widht in pixels
    constexpr int kScreenHeight             = 64;   // OLED height in pixels
    constexpr int kOledReset                = -1;   // reset pin shared with ESP8266 reset
    constexpr int kI2cAddress               = 0x3C; // standard I2C address

    /** time and logic constants */
    // 3L * 3600L = 3 hours (UTC+3 for Helsinki Summer Time)
    constexpr long kHelsinkiOffsetSeconds   = 3L * 3600L;
    // 30,000ms = 30 seconds. How often to refresh the bus times
    constexpr unsigned long kPollIntervalMs = 30'000;
    // API Endpoint for Digitransit HSL
    constexpr char kGraphQlUrl[] = "https://api.digitransit.fi/routing/v2/hsl/gtfs/v1";

    /** object initialization */
    Adafruit_SSD1306 display(kScreenWidth, kScreenHeight, &Wire, kOledReset);
    WiFiUDP ntpUDP;
    NTPClient timeClient(ntpUDP, "pool.ntp.org", kHelsinkiOffsetSeconds);

    /** setting up OLED display */
    void setup_display() {
        // Initialize the OLED. SSD1306_SWITCHCAPVCC generates display voltage internally.
        if (!display.begin(SSD1306_SWITCHCAPVCC, kI2cAddress)) {
            for (;;) { // Loop forever if display fails
                delay(100);
            }
        }
        display.clearDisplay();
        display.setTextColor(SSD1306_WHITE);
        display.display();
    }

    /** conecting to WiFi */
    void connect_wifi() {
        display.clearDisplay();
        display.setCursor(0, 0);
        display.println(F("Connecting WiFi ..."));
        display.display();

        WiFi.begin(SECRET_SSID, SECRET_PASS);
        while (WiFi.status() != WL_CONNECTED) {
            delay(500);
            display.println(F(".")); // F() macro keeps the string in the flash memory
            display.display();
        }
    }

    /** converts current time to a long integer */
    [[nodiscard]] long now_seconds_since_midnight() {
        timeClient.update();
        return (timeClient.getHours() * 3600L) + (timeClient.getMinutes() * 60L) +
               timeClient.getSeconds();
    }

    /** converts seconds (from midnight) into a "HH:MM" string */
    [[nodiscard]] String format_hhmm(long seconds_since_midnight) {
        const int h = static_cast<int>((seconds_since_midnight / 3600) % 24);
        const int m = static_cast<int>((seconds_since_midnight / 60) % 60);
        char buf[6];
        snprintf(buf, sizeof buf, "%02d:%02d", h, m);
        return String(buf);
    }

    /** visual debugging helper function */
    void show_api_error(int http_code) {
        display.clearDisplay();
        display.setCursor(0, 0);
        display.println(F("API Error"));
        display.print(F("Code: "));
        display.println(http_code);
        display.display();
    }

    /** fetch data and parse */
    [[nodiscard]] bool fetch_and_parse_json(WiFiClientSecure& client, HTTPClient& http, JsonDocument& doc) {
        
        // intializing the connection to the server
        if (!http.begin(client, String(kGraphQlUrl))) {
            return false;
        }
        
        // seting up the header for the POST request
        http.addHeader(F("Content-Type"), F("application/graphql"));
        http.addHeader(F("digitransit-subscription-key"), HSL_API_KEY);
       
        // Build the full query string: Prefix + ID + Suffix
        String fullQuery = String(hsl::kQueryPrefix) + hsl::kDefaultStopId + hsl::kQuerySuffix;

        // sending the POST request
        const int http_code = http.POST(fullQuery);
        if (http_code != HTTP_CODE_OK) {
            show_api_error(http_code);  // handling the http error
            http.end();                 // close the http connection and free up memory
            return false;
        }

        // Parse the incoming data stream
        // deserializeJson takes the raw text stream and builds the 'doc' object processes data as arrives
        const DeserializationError err = deserializeJson(doc, http.getStream());
        http.end(); // close and free up memory

        if (err) { // handling data recieving and parsing error
            display.clearDisplay();
            display.setCursor(0, 0);
            display.println(F("JSON parse error"));
            display.println(err.c_str());
            display.display();
            return false;
        }
        return true;
    }

    /** display fetched data */
    void display_arrivals(JsonArrayConst arrivals, long now_seconds) {
        display.clearDisplay();
        display.setTextSize(1);
        display.setCursor(0, 0);

        // static header
        display.println(F("Bus Stop:"));
        display.println(F("Von Daehnin katu 14"));
        display.println(F("---------------------"));

        for (JsonObjectConst arrival : arrivals) { // loop over all the JSON objects
            
            // get the route data from the JSON document
            const char* route = arrival["trip"]["routeShortName"];
            if (!route) {
                route = "?"; // fall-back value
            }
            
            const double arrival_raw    = arrival["realtimeArrival"].as<double>();
            const long arrival_sec      = static_cast<long>(arrival_raw);
            const long diff_seconds     = arrival_sec - now_seconds;
            const int mins_left         = static_cast<int>(diff_seconds / 60);

            display.print(route);
            display.print(F(": "));

            if (mins_left > 15) {
                display.println(format_hhmm(arrival_sec));
            } else if (mins_left <= 0) {
                display.println(F("NOW"));
            } else {
                display.print(mins_left);
                display.println(F(" min"));
            }
        }
        display.display();
    }

    /** fetching data from the API and display */
    void fetch_and_display() {
        WiFiClientSecure client;
        client.setInsecure();

        HTTPClient http;
        JsonDocument doc;

        if (!fetch_and_parse_json(client, http, doc)) {
            return;
        }

        const JsonArrayConst arrivals = doc["data"]["stop"]["stoptimesWithoutPatterns"].as<JsonArrayConst>();
        if (arrivals.isNull()) {
            display.clearDisplay();
            display.setCursor(0, 0);
            display.println(F("No arrivals data"));
            display.display();
            return;
        }

        const long now_sec = now_seconds_since_midnight();
        display_arrivals(arrivals, now_sec);
    }

} // namespace

void setup() {
    Serial.begin(115200); // Baud Rate
    setup_display();
    connect_wifi();
    timeClient.begin();
}

void loop() {
    fetch_and_display();
    delay(kPollIntervalMs); // Wait 30 seconds before next update
}
