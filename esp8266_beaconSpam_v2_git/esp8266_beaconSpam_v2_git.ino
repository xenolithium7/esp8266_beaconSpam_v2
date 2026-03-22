#include <ESP8266WiFi.h>
#include <U8g2lib.h>

U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);
#define BUTTON_PIN 13 

// settings
const uint8_t targetChannel = 1;
const uint32_t baseMacSeed = 0xDEADC0DE; 

// Wi-Fi SSIDs
const char list1[][33] PROGMEM = {
  "one",
  "two",
  "three",
  "four"
 };
const char list2[][33] PROGMEM = { 
  "one", 
  "two",
  "three",
  "four"
 };
const char list3[][33] PROGMEM = { 
  "one", 
  "two", 
  "three"
 };

uint8_t currentMode = 0; 
uint16_t totalSsidCount = 0;
uint32_t lastButtonPress = 0;
String scrollText = "";
int scrollX = 128, textWidth = 0;
uint32_t lastScrollUpdate = 0;

uint8_t beaconPacket[109] = {
  0x80, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
  0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 
  0x00, 0x00, 0x83, 0x51, 0xf7, 0x8f, 0x0f, 0x00, 0x00, 0x00, 0xe8, 0x03, 
  0x21, 0x00, 0x00, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 
  0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 
  0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 
  0x01, 0x08, 0x82, 0x84, 0x8b, 0x96, 0x24, 0x30, 0x48, 0x6c, 0x03, 0x01, 
  0x01, 0x00, 0x00
};

void getStableMac(uint8_t m, uint8_t idx, uint8_t* outMac) {
  randomSeed(baseMacSeed + (m * 100) + idx);
  outMac[0] = random(0x0, 0x100) & 0xfe | 0x02;
  for(int j=1; j<6; j++) outMac[j] = random(0x0, 0x100);
}

void loadMode(uint8_t mode) {
  scrollText = "";
  if (mode == 0) { totalSsidCount = 0; return; }
  
  if (mode == 1) totalSsidCount = sizeof(list1)/33;
  else if (mode == 2) totalSsidCount = sizeof(list2)/33;
  else if (mode == 3) totalSsidCount = sizeof(list3)/33;
  else if (mode == 4) totalSsidCount = (sizeof(list1)+sizeof(list2)+sizeof(list3))/33;
  
  auto add = [&](const char l[][33], int c){ 
    for(int i=0; i<c; i++){ char b[33]; memcpy_P(b, l[i], 32); b[32]='\0'; scrollText += String(b)+"  * "; } 
  };
  if(mode==1) add(list1, sizeof(list1)/33);
  else if(mode==2) add(list2, sizeof(list2)/33);
  else if(mode==3) add(list3, sizeof(list3)/33);
  else if(mode==4) { add(list1, sizeof(list1)/33); add(list2, sizeof(list2)/33); add(list3, sizeof(list3)/33); }

  textWidth = u8g2.getUTF8Width(scrollText.c_str());
  scrollX = 128;
}

void floodBeacons() {
  if (currentMode == 0) return;
  for (uint16_t i = 0; i < totalSsidCount; i++) {
    uint8_t mac[6]; char ssid[33];
    if (currentMode < 4) {
      getStableMac(currentMode, i, mac);
      if (currentMode == 1) memcpy_P(ssid, list1[i], 32);
      else if (currentMode == 2) memcpy_P(ssid, list2[i], 32);
      else if (currentMode == 3) memcpy_P(ssid, list3[i], 32);
    } else {
      int c1 = sizeof(list1)/33, c2 = sizeof(list2)/33;
      if (i < c1) { getStableMac(1, i, mac); memcpy_P(ssid, list1[i], 32); }
      else if (i < c1 + c2) { getStableMac(2, i-c1, mac); memcpy_P(ssid, list2[i-c1], 32); }
      else { getStableMac(3, i-c1-c2, mac); memcpy_P(ssid, list3[i-c1-c2], 32); }
    }
    memcpy(&beaconPacket[10], mac, 6);
    memcpy(&beaconPacket[16], mac, 6);
    memcpy(&beaconPacket[38], ssid, 32);
    beaconPacket[82] = targetChannel;
    wifi_send_pkt_freedom(beaconPacket, 83, 0);
    yield();
  }
}

void setup() {
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  u8g2.begin(); u8g2.enableUTF8Print(); u8g2.setFont(u8g2_font_6x12_t_cyrillic);
  
    // OLED text (boot)
  u8g2.clearBuffer();
  u8g2.setCursor(0, 25);
  u8g2.print("Wait! I'm thinking!");
  u8g2.setCursor(0, 45);
  u8g2.print("meow meow!");
  u8g2.sendBuffer();
  delay(1000);

u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_logisoso18_tf);

  u8g2.setCursor(32, 20);
  u8g2.print("Wi-Fi");

  u8g2.drawBox(0, 24, 128, 20);
  u8g2.setDrawColor(0);
  u8g2.setCursor(45, 41);
  u8g2.print("Beacon");
  u8g2.setDrawColor(1);

  u8g2.setCursor(38, 62);
  u8g2.print("Spam");

  u8g2.sendBuffer();
  delay(2000);


  // loading effect
  for(int i = 10; i < 118; i += 10) {
    u8g2.drawBox(i, 44, 5, 2);
    u8g2.sendBuffer();
    delay(100);
  }
  u8g2.setFont(u8g2_font_6x12_t_cyrillic);
  loadMode(0); // off mode
  wifi_set_opmode(STATION_MODE);
  wifi_set_channel(targetChannel);
}

void loop() {
  if (digitalRead(BUTTON_PIN) == LOW && millis() - lastButtonPress > 450) {
    currentMode = (currentMode + 1) % 5;
    loadMode(currentMode);
    lastButtonPress = millis();
  }

  if (currentMode > 0) floodBeacons();

  // OLED Text

  if (millis() - lastScrollUpdate >= 30) {
    u8g2.clearBuffer();
    if (currentMode == 0) {
      u8g2.setCursor(0, 9);  u8g2.print("Mode: OFF");
      u8g2.setCursor(0, 19); u8g2.print("Wi-Fi AIO Tool 1.9.2");
      u8g2.setCursor(0, 29); u8g2.print("by remilialembrik");
      u8g2.drawHLine(0, 32, 128);
      u8g2.setCursor(0, 42); u8g2.print("");
      u8g2.setCursor(0, 52); u8g2.print("");
      u8g2.setCursor(0, 62); u8g2.print("");
    } else {
      u8g2.setCursor(0, 10); u8g2.print("Wi-Fi Spam v2");
      u8g2.setCursor(0, 22); u8g2.print("by remilialembrik");
      u8g2.drawHLine(0, 26, 128);
      u8g2.setCursor(0, 38);
      if(currentMode == 4) u8g2.print("Mode: ALL");
      else { u8g2.print("Mode: "); u8g2.print(currentMode); }
      u8g2.print(" | SSID: "); u8g2.print(totalSsidCount);
      u8g2.setCursor(0, 48); u8g2.print("Uptime: "); u8g2.print(millis() / 1000); u8g2.print(" сек");
      u8g2.drawHLine(0, 52, 128);
      u8g2.setCursor(scrollX, 63); u8g2.print(scrollText);
      scrollX -= 2;
      if (scrollX < -textWidth) scrollX = 128;
    }
    u8g2.sendBuffer();
    lastScrollUpdate = millis();
  }
}