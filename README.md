📡 Wi-Fi AIO Tool v1.9.3
Developed by Remilia Lembrik

A lightweight and effective tool for ESP8266 designed to demonstrate the vulnerabilities of the 802.11 protocol through the distribution of Beacon frames (SSID Spam).

Main features
Multi-List Spamming: Three built-in SSID lists for different scenarios.

Dynamic UI: Informative interface based on OLED SSD1306 (U8g2).

OLED Optimization: Using a full-line buffer for smooth text display.

Stable MAC Generation: Unique generation of MAC addresses for each SSID based on the base SID.

Channel Hopping: Work on a given channel for maximum efficiency.


Controller: NodeMCU (ESP8266).

Display: 128x64 OLED (I2C).

Control: One clock button (pin D7 / GPIO13) for switching modes.

📦 Dependencies
To build the project, you will need the following libraries:

U8g2 — for working with the display.


How to use

Startup: When you turn on, you will be greeted with a stylish three-line greeting: Wi-Fi AIO Tool.

Switching: A short press on the button changes the "set" of broadcast networks.

Modes:

Mode 0: The main list (advertising/info).

Mode 1: Custom List 1.

Mode 2: Custom List 2.

⚠️ Disclaimer
This project was created solely for educational purposes and for testing network security. The author is not responsible for any illegal use of this software. Remember: it may be illegal to interfere with other people's networks!
