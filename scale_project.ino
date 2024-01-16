#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <uMQTTBroker.h>

#include "fonts/NotoSansBold24pt7b.h"
#include "fonts/NotoSans14pt7b.h"
#include "fonts/NotoSans12pt7b.h"
#include "fonts/NotoSans10pt7b.h"
#include "icon.h"

// pin connection
#define TFT_DC    D1
#define TFT_RST   D2
#define TFT_CS    D7
#define touchPin D0

Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);

const char* ssid = "NodeMCU"; // Access Point SSID
const char* password = "123456789";  // Access Point Password

ESP8266WebServer server(80);

String BG_COLOR = "ffd500";

int hexStringToInt(String hex) {
  return (int)strtol(hex.c_str(), NULL, 16);
}

class myMQTTBroker: public uMQTTBroker {
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

void setupWiFiAP () {
  WiFi.softAP(ssid, password);
}

void setupMessaging () {
  myBroker.init();
  myBroker.subscribe("#");
}

void setupWebServer () {
  server.on("/", rootHandler);
  server.begin();
}

void startup () {
  tft.init(240, 240, SPI_MODE2);
  tft.setRotation(1);

  tft.fillScreen(ST77XX_BLACK);
  setBackground();
  printWeight("230.23");
  printUnit("gr");
  printModeLabel();
  printMode("creamer");
  tft.drawBitmap(20, 30, mug_icon, 48, 48, ST77XX_BLACK);
}

void setup() {
  Serial.begin(9600);
  pinMode(touchPin, INPUT);

  setupWiFiAP();
  setupMDNS();
  setupMessaging();
  setupWebServer();

  startup();
}

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

void printWeight(String number) {
  tft.setTextSize(1);
  tft.setFont(&NotoSansBold24pt7b);
  tft.setTextColor(hexStringToInt("000000"));

  tft.fillRoundRect(17, 145, 215, 36, 10, hexStringToInt(BG_COLOR));

  tft.setCursor(30, 180);
  tft.print(number);
}

void printUnit(String number) {
  tft.setTextSize(1);
  tft.setFont(&NotoSans14pt7b);
  tft.setTextColor(hexStringToInt("000000"));

  tft.fillRoundRect(17, 188, 100, 24, 10, hexStringToInt(BG_COLOR));

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
  tft.setTextSize(1);
  tft.setFont(&NotoSans14pt7b);
  tft.setTextColor(hexStringToInt("000000"));

  tft.fillRect(75, 50, 120, 28, hexStringToInt(BG_COLOR));

  tft.setCursor(80, 72);
  tft.print(number);
}

void setBackground () {
  tft.fillRoundRect(5, 5, 230, 230, 35, hexStringToInt(BG_COLOR));
}
