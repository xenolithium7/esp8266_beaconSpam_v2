-----

# 📡 ESP8266 BeaconSpam v2

A lightweight and effective tool for **ESP8266** designed to demonstrate the vulnerabilities of the **802.11 protocol** through the distribution of Beacon frames (SSID Spam).

> [\!CAUTION]
> **Disclaimer:** This project was created solely for educational purposes and for testing network security. The author is not responsible for any illegal use of this software. Remember: it may be illegal to interfere with other people's networks\!


## ✨ Main Features

  * **Multi-List Spamming:** Three built-in SSID lists for different scenarios.
  * **Dynamic UI:** Informative interface based on OLED SSD1306 (**U8g2**).
  * **Stable MAC Generation:** Unique generation of MAC addresses for each SSID based on the base SID.

## 🛠 Hardware Requirements

| Component | Specification |
| :--- | :--- |
| **Controller** | ESP8266 (NodeMCU, Wemos D1 Mini, etc.) |
| **Display** | 128x64 OLED SSD1306 (I2C) |
| **Control** | Button connected to pin **D7 (GPIO13)** |



## 🔌 Pinout

### 📺 OLED Display (I2C)
| OLED Pin | ESP8266 Pin | GPIO | Description |
| :--- | :--- | :--- | :--- |
| **VCC** | 3.3V | — | Power (3.3V) |
| **GND** | GND | — | Ground |
| **SCL** | D1 | GPIO5 | I2C Clock |
| **SDA** | D2 | GPIO4 | I2C Data |

### 🔘 Control Button
| Button Pin | ESP8266 Pin | GPIO | Description |
| :--- | :--- | :--- | :--- |
| **Pin 1** | D7 | GPIO13 | Control Signal (Input) |
| **Pin 2** | GND | — | Ground |



## 📦 Dependencies

To build the project, make sure you have the following library installed in your Arduino IDE / PlatformIO:

  * [U8g2](https://github.com/olikraus/u8g2) — for working with the display.



## 🚀 Switching Modes

### 1\. Startup

A **short press** on the button (D7) changes the active list of broadcast networks.

### 3\. Operating Modes

| Mode | Description |
| :--- | :--- |
| **Mode 0** | Idle / Off |
| **Mode 1** | Custom List 1 |
| **Mode 2** | Custom List 2 |
| **Mode 3** | Custom List 3 |
| **Mode 4** | **All Lists** (Maximum Spam) |

-----
