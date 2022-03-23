/*  Codes that Receives  JSON data using ESPNOW Protocol

    Developed by  Jay Joshi
    github.com/JayJoshi16

    Modified by Sachin Soni
      
    Developed for techiesms 
      https://www.youtube.com/techiesms
*/


#include <esp_now.h>
#include <WiFi.h>
#include <ArduinoJson.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>


#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

String jsondata;
StaticJsonDocument<200> doc;

void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {

  char* buff = (char*) incomingData;        //char buffer
  jsondata = String(buff);                  //converting into STRING
  Serial.print("Recieved ");
  Serial.println(jsondata);    //Complete JSON data will be printed here
  DeserializationError error = deserializeJson(doc, jsondata);

  if (!error) {
    float temp   = doc["temp"];
    float hum  =   doc["hum"];
    Serial.println(temp);                   //values of a
    Serial.println(hum);                   //values of b

    display.clearDisplay();
    display.setTextSize(2);             // Normal 1:1 pixel scale
    display.setTextColor(SSD1306_WHITE);        // Draw white text
    display.setCursor(0, 0);            // Start at top-left corner
    display.print("Temp:");
    display.println(temp);
    display.print("Hum:");.
    display.println(hum);
    display.display();
  }

  else {
    Serial.print(F("deserializeJson() failed: "));  //Just in case of an ERROR of ArduinoJSon
    Serial.println(error.f_str());
    return;
  }
}

void setup() {

  Serial.begin(115200);
  WiFi.mode(WIFI_STA);

  if (esp_now_init() != ESP_OK) {       //Init ESPNOW
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  esp_now_register_recv_cb(OnDataRecv);         //Reciever CallBack function


  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {                // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }

}

void loop() {

}
