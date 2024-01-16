/**************************************************************************
 * 
 * Interfacing ESP8266 NodeMCU with ST7789 TFT display (240x240 pixel).
 * Graphics test example.
 * This is a free software with NO WARRANTY.
 * https://simple-circuit.com/
 *
 *************************************************************************/
/**************************************************************************
  This is a library for several Adafruit displays based on ST77* drivers.

  Works with the Adafruit 1.8" TFT Breakout w/SD card
    ----> http://www.adafruit.com/products/358
  The 1.8" TFT shield
    ----> https://www.adafruit.com/product/802
  The 1.44" TFT breakout
    ----> https://www.adafruit.com/product/2088
  as well as Adafruit raw 1.8" TFT display
    ----> http://www.adafruit.com/products/618

  Check out the links above for our tutorials and wiring diagrams.
  These displays use SPI to communicate, 4 or 5 pins are required to
  interface (RST is optional).

  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.
  MIT license, all text above must be included in any redistribution
 *************************************************************************/

#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7789.h> // Hardware-specific library for ST7789
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <uMQTTBroker.h>

#include "NotoSansBold24pt7b.h"
#include "NotoSans14pt7b.h"
#include "NotoSans12pt7b.h"
#include "NotoSans10pt7b.h"
#include "ScaleIcon.h"

// ST7789 TFT module connections
#define TFT_DC    D1     // TFT DC  pin is connected to NodeMCU pin D1 (GPIO5)
#define TFT_RST   D2     // TFT RST pin is connected to NodeMCU pin D2 (GPIO4)
#define TFT_CS    D7     // TFT CS  pin is connected to NodeMCU pin D8 (GPIO15)
#define touchPin D0
// initialize ST7789 TFT library with hardware SPI module
// SCK (CLK) ---> NodeMCU pin D5 (GPIO14)
// MOSI(DIN) ---> NodeMCU pin D7 (GPIO13)

Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);

const char* ssid = "NodeMCU";       
const char* password = "123456789"; 

ESP8266WebServer server(80);

String BG_COLOR = "ffd500";

int hexStringToInt(String hex) {
  return (int)strtol(hex.c_str(), NULL, 16);
}

class myMQTTBroker: public uMQTTBroker
{
public:
    virtual bool onConnect(IPAddress addr, uint16_t client_count) {
      Serial.println(addr.toString()+" connected");
      return true;
    }

    virtual void onDisconnect(IPAddress addr, String client_id) {
      Serial.println(addr.toString()+" ("+client_id+") disconnected");
    }

    virtual bool onAuth(String username, String password, String client_id) {
      Serial.println("Username/Password/ClientId: "+username+"/"+password+"/"+client_id);
      return true;
    }
    
    virtual void onData(String topic, const char *data, uint32_t length) {
      char data_str[length+1];
      os_memcpy(data_str, data, length);
      data_str[length] = '\0';
      
      Serial.println("received topic '"+topic+"' with data '"+(String)data_str+"'");
      //printClients();
    }

    // Sample for the usage of the client info methods

    virtual void printClients() {
      for (int i = 0; i < getClientCount(); i++) {
        IPAddress addr;
        String client_id;
         
        getClientAddr(i, addr);
        getClientId(i, client_id);
        Serial.println("Client "+client_id+" on addr: "+addr.toString());
      }
    }
};

myMQTTBroker myBroker;

void setupMDNS() {
  if (!MDNS.begin("scale")) {
    Serial.println("Error setting up mDNS");
    while (1) {
      delay(1000);
    }
  }
  Serial.println("mDNS responder started");
}

void setup() {
  Serial.begin(9600);
  pinMode(touchPin, INPUT);

   WiFi.softAP(ssid, password);

  IPAddress myIP = WiFi.softAPIP();
  Serial.print("Access Point IP:");
  Serial.println(myIP);
  setupMDNS();
  myBroker.init();
  myBroker.subscribe("#");

  tft.init(240, 240, SPI_MODE2);  // Initialize ST7789 with 240x240 resolution
  tft.setRotation(1);  // Adjust the display rotation if needed

  tft.fillScreen(ST77XX_BLACK);  // Fill the screen with black color
  setBackground();
  animateNumberChange("230.23");
  printUnit("gr");
  printModeLabel();
  printMode("creamer");
  tft.drawBitmap(20, 30, mug_icon, 48, 48, ST77XX_BLACK);
  server.on("/", rootHandler);

  server.begin();
}

unsigned long lastFrame = millis();
int frameDisplayTimer = 1000;
int counter = 0;

void loop() {
  MDNS.update();
  int touchValue = digitalRead(touchPin);
  server.handleClient();
  myBroker.publish("broker/counter", (String)counter++);
  delay(1000);
}

void rootHandler () {
  server.send(200, "text/html", renderWebpage());
}

String renderWebpage() {
  String ptr = "<!DOCTYPE html> <html>\n";
  ptr +="<head><script src=\"https://unpkg.com/htmx.org@1.9.10\"></script><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  ptr +="<title>LED Control</title>\n";
  ptr +="<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";
  ptr +="body{margin-top: 50px;} h1 {color: #444444;margin: 50px auto 30px;} h3 {color: #444444;margin-bottom: 50px;}\n";
  ptr +=".button {display: block;width: 80px;background-color: #1abc9c;border: none;color: white;padding: 13px 30px;text-decoration: none;font-size: 25px;margin: 0px auto 35px;cursor: pointer;border-radius: 4px;}\n";
  ptr +=".button-on {background-color: #3498db;}\n";
  ptr +=".button-on:active {background-color: #3498db;}\n";
  ptr +=".button-off {background-color: #34495e;}\n";
  ptr +=".button-off:active {background-color: #2c3e50;}\n";
  ptr +="p {font-size: 14px;color: #888;margin-bottom: 10px;}\n";
  ptr +="</style>\n";
  ptr +="</head>\n";
  ptr +="<body>\n";
  ptr +="<h1>ESP8266 Web Server</h1>\n";
  ptr +="<h3>Hello HTMX</h3>\n";
  ptr +="<button id=\"showButton\" hx-get=\"/uikit-modal.html\" hx-target=\"#modals-here\" class=\"uk-button uk-button-primary\" _=\"on htmx:afterOnLoad wait 10ms then add .uk-open to #modal\">Open Modal</button><div id=\"modals-here\"></div>";
  ptr +="</body>\n";
  ptr +="</html>\n";
  return ptr;
}

void animateNumberChange(String number) {
  // Set text properties with a larger text size (2x)
  tft.setTextSize(1);
  tft.setFont(&NotoSansBold24pt7b);
  tft.setTextColor(hexStringToInt("000000"));

  // Erase the leaving number by filling a rectangle with black
  tft.fillRoundRect(17, 145, 215, 36, 10, hexStringToInt(BG_COLOR));

  // Draw the entering number at its position
  tft.setCursor(30, 180);
  tft.print(number);
}

void printUnit(String number) {
  // Set text properties with a larger text size (2x)
  tft.setTextSize(1);
  tft.setFont(&NotoSans14pt7b);
  tft.setTextColor(hexStringToInt("000000"));

  // Erase the leaving number by filling a rectangle with black
  tft.fillRoundRect(17, 188, 100, 24, 10, hexStringToInt(BG_COLOR));

  // Draw the entering number at its position
  tft.setCursor(30, 209);
  tft.print(number);
}

void printModeLabel() {
  tft.setTextSize(1);
  tft.setFont(&NotoSans10pt7b);
  tft.setCursor(80, 47);
  tft.setTextColor(hexStringToInt("000000"));
  tft.print("mode");
}

void printMode(String number) {
  // Set text properties with a larger text size (2x)
  tft.setTextSize(1);
  tft.setFont(&NotoSans14pt7b);
  tft.setTextColor(hexStringToInt("000000"));

  // Erase the leaving number by filling a rectangle with black
  tft.fillRect(75, 50, 120, 28, hexStringToInt(BG_COLOR));

  // Draw the entering number at its position
  tft.setCursor(80, 72);
  tft.print(number);
}

void setBackground () {
  tft.fillRoundRect(5, 5, 230, 230, 35, hexStringToInt(BG_COLOR));
}