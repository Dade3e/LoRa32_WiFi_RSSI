/*
 *  This sketch demonstrates how to scan WiFi networks.
 *  The API is almost the same as with the WiFi Shield library,
 *  the most obvious difference being the different file you need to include:
 */
#include "WiFi.h"

//Libraries for OLED Display
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

//OLED pins
#define OLED_SDA 4//21  //4
#define OLED_SCL 15//22 //15
#define OLED_RST 16//-1 //16
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

const uint8_t vbatPin = 35;
float VBAT;

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RST);

//SSID of your network
char ssid[] = "yourAP";
//password of your WPA Network
char pass[] = "yourPassword";

void setup()
{
  Serial.begin(115200);

  // Set WiFi to station mode and disconnect from an AP if it was previously connected
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);

  WiFi.begin(ssid, pass);

  

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
    

  pinMode(vbatPin, INPUT);
  //reset OLED display via software
  pinMode(OLED_RST, OUTPUT);
  digitalWrite(OLED_RST, LOW);
  delay(20);
  digitalWrite(OLED_RST, HIGH);
  
  //initialize OLED
  Wire.begin(OLED_SDA, OLED_SCL);
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3c, false, false)) { // Address 0x3C for 128x32
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setCursor(0,0);
  display.print("WIFI RECEIVER ");
  display.display();

  while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
  }
  
  VBAT = (float)(analogRead(vbatPin)) / 4095*2*3.3*1.1;
  
  display.setCursor(0,20);
  display.println("Connected to:");
  display.setCursor(0,30);
  display.println(ssid);
  display.setCursor(0,56);
  display.print("BATT:");
  display.setCursor(32,56);
  display.print(VBAT);
  display.display();
  delay(3000);
}

void loop()
{
  VBAT = (float)(analogRead(vbatPin)) / 4095*2*3.3*1.1;
  long rssi = WiFi.RSSI();
  Serial.print("RSSI:");
  Serial.println(rssi);
  
  display.clearDisplay();
  display.setTextSize(1);

  graph_draw(rssi);
  
  display.setCursor(0,56);
  display.print("RSSI:");
  display.setCursor(32,56);
  display.print(rssi);
  display.setTextSize(1);
  display.setCursor(68,56);
  display.print("BATT:");
  display.setCursor(100,56);
  display.print(VBAT);
  
  display.display();
  delay(1000);
}
int inx = 0;
int value_y[20];
//range da 20 a 95 => 75, 75 sta a 50px =>75/50 = tengo 1.6
//quando ho 20 = 20/1.6 =12.5, calo 10, uguale con 95
//128/16 = 8 => inx*8
void graph_draw(int value){
  if(inx==0){
    for (int x = 0; x < 16; x++)
    {
      value_y[x] = 49;
    }
  }
  
  value_y[inx] = ((value*-1)/1.6)-10;
  //display.setCursor(60,26);
  //display.print(value_y[inx]);
  display.drawRect(0, 0, 128, 50, SSD1306_WHITE);
  for(int i=0;i<inx;i++){
    display.drawLine(i*8, value_y[i],(i+1)*8, value_y[i+1], SSD1306_WHITE);
  }
  inx++;
  if(inx>=16){
    inx=0;
  }
}
