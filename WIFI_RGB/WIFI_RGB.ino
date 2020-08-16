#include "WiFi.h"
#include "ESPAsyncWebServer.h"
#include "SPIFFS.h"
#include "FastLED.h"
#include "AsyncJson.h"
#include "ArduinoJson.h"

#define LED_PIN 32
#define NUM_LEDS 30
#define LED_TYPE WS2812B

CRGB leds[NUM_LEDS];

DynamicJsonDocument doc(4000);
JsonArray ledAry = doc.createNestedArray("leds");

const char* ssid = "ESP32_IOT_MCU";
const char* password = "";

AsyncWebServer server(80);

void setup() {
  Serial.begin(115200);
  LEDS.addLeds<LED_TYPE, LED_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(100);

  if (!SPIFFS.begin(true)) {
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }

  Serial.print("Setting AP (Access Point)…");
  WiFi.softAP(ssid, password);

  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);

  server.on("/", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(SPIFFS, "/index.html", String(), false);
  });

  server.on("/getleds", HTTP_GET, [](AsyncWebServerRequest * request) {
    String ledJson;
    serializeJson(doc, ledJson);
    request->send(200, "application/json", ledJson);
  });

  AsyncCallbackJsonWebHandler *handler = new AsyncCallbackJsonWebHandler("/send", [](AsyncWebServerRequest * request, JsonVariant & variant) {
    StaticJsonDocument<100> data;
    if (variant.is<JsonArray>())
    {
      data = variant.as<JsonArray>();
    }
    else if (variant.is<JsonObject>())
    {
      data = variant.as<JsonObject>();
    }
    JsonObject json = data.as<JsonObject>();
    int led = json["led"];
    int red = json["color"]["R"];
    int green = json["color"]["G"];
    int blue = json["color"]["B"];
    leds[led].setRGB(red, green, blue);
    FastLED.show();

    int active = getActiveLed(ledAry, led);
    if (active != -1) {
      // modify active led color
      ledAry[active]["color"]["R"] = red;
      ledAry[active]["color"]["G"] = green;
      ledAry[active]["color"]["B"] = blue;
    }
    else {
      // add new leds color
      JsonObject ledObj = ledAry.createNestedObject();
      ledObj["led"] = led;
      JsonObject ledColor = ledObj.createNestedObject("color");
      ledColor["R"] = red;
      ledColor["G"] = green;
      ledColor["B"] = blue;
    }

    // print request data
    String jsonStr;
    serializeJson(data, jsonStr);
    Serial.println(jsonStr);
    request->send(200, "application/json", "200");
  });
  server.addHandler(handler);

  server.begin();
}

void loop() {
}

int getActiveLed(JsonArray array, int element) {
  for (int i = 0; i < array.size(); i++) {
    if ((int) array[i]["led"] == element) {
      return i;
    }
  }
  return -1;
}
