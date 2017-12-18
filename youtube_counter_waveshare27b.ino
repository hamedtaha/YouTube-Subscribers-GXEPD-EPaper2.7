// include library, include base class, make path known
#include <GxEPD.h>

#include <GxGDEW027C44/GxGDEW027C44.cpp>    // 2.7" b/w/r

#include <GxIO/GxIO_SPI/GxIO_SPI.cpp>
#include <GxIO/GxIO.cpp>

// FreeFonts from Adafruit_GFX
#include <Fonts/FreeSansBold24pt7b.h>
#include <Fonts/FreeSansBold18pt7b.h>
#include <Fonts/FreeSans9pt7b.h>

#include <YoutubeApi.h>
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include "youtubelogo.h"
#include <ArduinoJson.h> // This Sketch doesn't technically need this, but the library does so it must be installed.

#if defined(ESP8266)

// generic/common.h
//static const uint8_t SS    = 15;
//static const uint8_t MOSI  = 13;
//static const uint8_t MISO  = 12;
//static const uint8_t SCK   = 14;
// pins_arduino.h
//static const uint8_t D8   = 15;
//static const uint8_t D7   = 13;
//static const uint8_t D6   = 12;
//static const uint8_t D5   = 14;

// GxIO_SPI(SPIClass& spi, int8_t cs, int8_t dc, int8_t rst = -1, int8_t bl = -1);
GxIO_Class io(SPI, SS, 0, 2); // arbitrary selection of D3(=0), D4(=2), selected for default of GxEPD_Class
// GxGDEP015OC1(GxIO& io, uint8_t rst = 2, uint8_t busy = 4);
GxEPD_Class display(io); // default selection of D4(=2), D2(=4)
#endif

#define DEMO_DELAY 10

//------- Replace the following! ------
char ssid[] = "";       // your network SSID (name)
char password[] = "";  // your network key
#define API_KEY ""  // your google apps API Token
#define CHANNEL_ID "" // makes up the url of channel


WiFiClientSecure client;
YoutubeApi api(API_KEY, client);

unsigned long api_mtbs = 600000; //mean time between api requests
unsigned long api_lasttime;   //last time api request has been done

long subs = 0;
String subscribers,viewCount,videoCount;

void setup() {
  // put your setup code here, to run once:

  Serial.begin(115200);
  Serial.println();
  Serial.println("setup");
  display.init();
  display.setRotation(3);
  Serial.println("setup done");

  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);

  // Attempt to connect to Wifi network:
  Serial.print("Connecting Wifi: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  IPAddress ip = WiFi.localIP();
  Serial.println(ip);


}

void loop() {
  // put your main code here, to run repeatedly:
  //if (millis() - api_lasttime > api_mtbs)  {
    if(api.getChannelStatistics(CHANNEL_ID))
    {
     subscribers=api.channelStats.subscriberCount;
     viewCount=api.channelStats.viewCount;
     videoCount=api.channelStats.videoCount;
     display.drawPaged(showFontCallback);
     Serial.print("Subscriber Count: ");
     Serial.println(subscribers);
     Serial.print("View Count: ");
     Serial.println(api.channelStats.viewCount);
     Serial.print("Video Count: ");
     Serial.println(api.channelStats.videoCount);
      }
      // api_lasttime = millis();
  //}
   delay(api_mtbs);
 

}

void showFontCallback()
{
  const char* name = "FreeSansBold24pt7b";
  const GFXfont* f = &FreeSansBold24pt7b;

  const char* name18 = "FreeSansBold18pt7b";
  const GFXfont* f18pt = &FreeSansBold18pt7b;
  
  const char* name9 = "FreeSans9pt7b";
  const GFXfont* f9pt = &FreeSans9pt7b;
  

  
  display.fillScreen(GxEPD_WHITE);
  display.setTextColor(GxEPD_BLACK);
  display.setCursor(0, 130);
  display.println();
  //display.drawLine(0,60,264,60,GxEPD_BLACK);
  display.drawLine(0,60,264,60,GxEPD_RED);
  display.setFont(f);
  int x=(subscribers.length()*30)/2;  // make the text in the center of the display 
  //display.setTextWrap(true);
  display.setCursor(132-x, 100);
  display.println(subscribers);
  display.setFont(f9pt);
  display.setCursor(80, 120);
  display.setTextColor(GxEPD_BLACK);
  display.println("Subscribers");
  
  display.drawBitmap(60, 10, gImage_ytlogo1, 46, 32, GxEPD_RED);
  display.drawBitmap(110, 10, gImage_ytlogo2, 94, 29, GxEPD_BLACK);

  display.drawLine(0,140,264,140,GxEPD_BLACK);
  display.setTextColor(GxEPD_BLACK);
  display.setCursor(0, 165);
  display.println("Views:" +viewCount);
  display.setCursor(142, 165);
  display.println("Videos:" +videoCount);


}
