#include "Freenove_WS2812_Lib_for_ESP32.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <WiFiManager.h> 
#include <ESPmDNS.h> 
#include <ESP32Time.h>

#define LEDS_COUNT  320
#define LEDS_PIN  27
#define CHANNEL   1
#define TRIGGER_PIN 15 

Freenove_ESP32_WS2812 strip = Freenove_ESP32_WS2812(LEDS_COUNT, LEDS_PIN, CHANNEL);
WiFiManager wm; 
ESP32Time rtc; 

int number0[5][3] = {{1,1,1},{1,0,1},{1,0,1},{1,0,1},{1,1,1}};
int number1[5][3] = {{0,1,0},{0,1,0},{0,1,0},{0,1,0},{0,1,0}};
int number2[5][3] = {{1,1,1},{0,0,1},{1,1,1},{1,0,0},{1,1,1}};
int number3[5][3] = {{1,1,1},{0,0,1},{1,1,1},{0,0,1},{1,1,1}};
int number4[5][3] = {{1,0,1},{1,0,1},{1,1,1},{0,0,1},{0,0,1}};
int number5[5][3] = {{1,1,1},{1,0,0},{1,1,1},{0,0,1},{1,1,1}};
int number6[5][3] = {{1,1,1},{1,0,0},{1,1,1},{1,0,1},{1,1,1}};
int number7[5][3] = {{1,1,1},{0,0,1},{0,0,1},{0,0,1},{0,0,1}};
int number8[5][3] = {{1,1,1},{1,0,1},{1,1,1},{1,0,1},{1,1,1}};
int number9[5][3] = {{1,1,1},{1,0,1},{1,1,1},{0,0,1},{1,1,1}};

int icon0[7][7] = {{0,0,1,1,1,0,0},{0,1,1,1,1,1,0},{1,1,1,1,0,1,1},{1,1,1,1,1,1,1},{1,1,1,1,0,0,0},{0,1,1,1,1,0,0},{0,0,1,1,1,1,0}};
int icon1[7][7] = {{0,0,1,1,1,0,0},{0,1,1,1,1,1,0},{1,1,1,1,0,1,1},{1,1,1,1,1,1,1},{1,1,1,1,1,0,0},{0,1,1,1,1,1,1},{0,0,1,1,1,1,0}};
int icon2[7][7] = {{0,0,1,1,1,0,0},{0,1,1,1,1,1,0},{1,1,1,1,0,1,1},{1,1,1,1,1,1,1},{1,1,1,1,0,0,0},{0,1,1,1,1,0,0},{0,0,1,1,1,1,0}};
int icon3[7][7] = {{0,0,1,1,1,0,0},{0,1,1,1,1,1,0},{1,1,1,1,0,1,1},{1,1,1,1,1,1,1},{1,1,1,1,1,0,0},{0,1,1,1,1,1,1},{0,0,1,1,1,1,0}};
int icon4[7][7] = {{0,0,1,1,1,0,0},{0,1,1,1,1,1,0},{1,1,1,1,0,1,1},{1,1,1,1,1,1,1},{1,1,1,1,0,0,0},{0,1,1,1,1,0,0},{0,0,1,1,1,1,0}};

int icon_idx = 0;
int timeArray[6];
int (*numbers[10])[3] = {number0, number1, number2, number3, number4, number5, number6, number7, number8, number9};
int (*icons[7])[7] = {icon0, icon1, icon2, icon3, icon4};
int time_r = 60; int time_g = 20; int time_b = 5;
int point_r = 60; int point_g = 20; int point_b = 0;
int icon_r = 60; int icon_g = 20; int icon_b = 0;

  
unsigned int  timeout   = 5; // 設定門戶運行的超時時間為5秒
unsigned int  startTime = millis(); // 記錄程序啟動的起始時間
bool portalRunning      = false; // 追踪配置門戶是否正在運行
bool startAP            = true; // 用於決定是啟動AP+網頁伺服器(true)還是僅啟動網頁伺服器(false)
const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 8*60*60;
const int   daylightOffset_sec = 0;


int get_index(int x, int y) {
    int led_index;
    if (x <= 7) {
        led_index = x + 8 * y;
    } else if (x >= 8 && x <= 15) {
        led_index = 64 + (x - 8) + 8 * y;
    } else if (x >= 16 && x <= 23) {
        led_index = 128 + (x - 16) + 8 * y;
    } else if (x >= 24 && x <= 31) {
        led_index = 192 + (x - 24) + 8 * y;
    } else if (x >= 32 && x <= 39) {
        led_index = 256 + (x - 32) + 8 * y;
    }
    return led_index;
}

void setLedColorsTime(int origin_x, int origin_y, int (*numberArray)[3],bool isLedOn, int r, int g, int b) {
    for (int row = 0; row < 5; row++) {
      for (int col = 0; col < 3; col++) {
        int led_x = origin_x+col;
        int led_y = origin_y+row;
        //Serial.printf("origin_x=%d, col=%d, led_x=%d\n",origin_x,col,led_x );
        
        if (numberArray[row][col] == 1){
          if (isLedOn == true){
            strip.setLedColorData(get_index(led_x, led_y), r, g, b); 
          }else{
             strip.setLedColorData(get_index(led_x, led_y), 0, 0, 0); 
          }
        }
      }
    }
}

void drawTime(int x, int y, bool isLEDOn, int time_r, int time_g, int time_b)
{
    setLedColorsTime(x,    y, numbers[timeArray[0]],isLEDOn, time_r, time_g, time_b); 
    setLedColorsTime(x+4,  y, numbers[timeArray[1]],isLEDOn, time_r, time_g, time_b); 
    setLedColorsTime(x+10, y, numbers[timeArray[2]],isLEDOn, time_r, time_g, time_b); 
    setLedColorsTime(x+14, y, numbers[timeArray[3]],isLEDOn, time_r, time_g, time_b); 
    setLedColorsTime(x+20, y, numbers[timeArray[4]],isLEDOn, time_r, time_g, time_b); 
    setLedColorsTime(x+24, y, numbers[timeArray[5]],isLEDOn, time_r, time_g, time_b); 
}

void eraseTime(int org_x, int org_y)
{
  int x;
  int y;
  for(y=org_y; y<org_y+7; y++)
    for(x=org_x; x<org_x+27; x++){
      strip.setLedColorData(get_index(x, y), 0, 0, 0);
    }
}

void setLedColorsIcon(int origin_x, int origin_y, int (*numberArray)[7],bool isLedOn, int r, int g, int b) {
    for (int row = 0; row < 7; row++) {
      for (int col = 0; col < 7; col++) {
        int led_x = origin_x+col;
        int led_y = origin_y+row;
//        ///////  
        //Serial.printf("origin_x=%d, col=%d, led_x=%d\n",origin_x,col,led_x );
        if (numberArray[row][col] == 1){
          if (isLedOn == true){
            strip.setLedColorData(get_index(led_x, led_y), r, g, b); 
          }else{
             strip.setLedColorData(get_index(led_x, led_y), 0, 0, 0); 
          }
        }
      }
    }
}

void drawIcon(int x, int y, int icon_idx, bool isLEDOn, int icon_r, int icon_g, int icon_b)
{
  setLedColorsIcon(x+icon_idx, y, icons[icon_idx], isLEDOn, icon_r, icon_g, icon_b);
}

void eraseIcon(int org_x, int org_y, int icon_idx)
{
  int x;
  int y;
  org_x = org_x + icon_idx;
  for(y=org_y; y < org_y+7; y++)
    for(x=org_x; x < org_x+7; x++)
      strip.setLedColorData(get_index(x, y), 0, 0, 0);
}

void setLedColorsPoint(int led_x, int led_y,bool isLedOn, int r, int g, int b) {
    if (isLedOn == true){
        strip.setLedColorData(get_index(led_x, led_y), r, g, b);
    }else{
        strip.setLedColorData(get_index(led_x, led_y), 0, 0, 0); 
    }
}

void drawPoint(int x, int y, bool isLEDOn, int point_r, int point_g, int point_b)
{
  setLedColorsPoint(x,   y,  true,point_r, point_g, point_b);
  setLedColorsPoint(x,   y+2,true,point_r, point_g, point_b);
  setLedColorsPoint(x+10,y,  true,point_r, point_g, point_b);
  setLedColorsPoint(x+10,y+2,true,point_r, point_g, point_b);
}

void convertTimeToArray(int* timeArray) 
{
  timeArray[0] = rtc.getHour(true)/10;
  timeArray[1] = rtc.getHour(true) % 10;
  timeArray[2] = rtc.getMinute() / 10;
  timeArray[3] = rtc.getMinute() % 10;
  timeArray[4] = rtc.getSecond() / 10;
  timeArray[5] = rtc.getSecond() % 10;
  Serial.printf("timeArrayt=%d%d:%d%d:%d%d\n",timeArray[0],timeArray[1],timeArray[2],timeArray[3],timeArray[4],timeArray[5] );
}

void doWiFiManager(){
  if(portalRunning){ // 如果配置portal當前正在運行
    wm.process(); // 處理WiFiManager任務

    // 檢查portal是否超過了超時時間
    if((millis()-startTime) > (timeout*1000)){
      Serial.println("portaltimeout");
      portalRunning = false; // 指示portal不再運行
      if(startAP){
        wm.stopConfigPortal(); // 如果啟動了配置portal，則停止配置portal
      }
      else{
        wm.stopWebPortal(); // 如果啟動了網頁portal，則停止網頁portal
      } 
   }
  }

  // 檢查是否按下了觸發按鈕且portal當前未運行
  if(digitalRead(TRIGGER_PIN) == LOW && (!portalRunning)) {
    if(startAP){
      Serial.println("Button Pressed, Starting Config Portal");
      wm.setConfigPortalBlocking(false);
      wm.startConfigPortal();
    }  
    else{
      Serial.println("Button Pressed, Starting Web Portal");
      wm.startWebPortal();
    }  
    portalRunning = true;
    startTime = millis();
  }
}

void updateIconTask(void *parameter) {
  for(;;) { 
    eraseIcon(29, 1, icon_idx);
    icon_idx = (icon_idx + 1) % 5;
    drawIcon(29, 1, icon_idx, true, icon_r, icon_g, icon_b);
    strip.show();
    vTaskDelay(pdMS_TO_TICKS(500)); 
  }
}

void updateTimeDisplayTask(void *parameter) {
  for(;;) { 
    convertTimeToArray(timeArray);
    eraseTime(1, 2);
    drawTime(1, 2, true, time_r, time_g, time_b);
    drawPoint(9, 3, true, point_r, point_g, point_b);
    strip.show();
    vTaskDelay(pdMS_TO_TICKS(1000)); 
  }
}

void syncTimeTask(void *parameter) {
  struct tm timeinfo;
  for(;;) { 
    if(getLocalTime(&timeinfo)) {
      rtc.setTimeStruct(timeinfo);
    }
    vTaskDelay(pdMS_TO_TICKS(1000 * 60 * 60 * 12)); 
  }
}

void setup() {
  // Setting
  pinMode(TRIGGER_PIN, INPUT_PULLUP); // 將觸發引腳設置為輸入模式並啟用上拉電阻
  
  // Serial
  Serial.begin(115200);
  Serial.setDebugOutput(true);
  Serial.println("\n Starting");
  
  // WiFi
  WiFi.mode(WIFI_STA); // 將WiFi模式設定為STA(站點)，以連接到路由器
  wm.setHostname("MDNSEXAMPLE"); // 為mDNS回應器設置主機名
  wm.autoConnect(); // 自動嘗試連接到之前保存的WiFi網絡

  // Time
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  struct tm timeinfo;
  if (getLocalTime(&timeinfo)){
     rtc.setTimeStruct(timeinfo);
  }

  // Draw
  strip.begin();

  // Multi-Task
  delay(10);
  xTaskCreate(updateIconTask, "Update Icon", 2048, NULL, 2, NULL);
  delay(10);
  xTaskCreate(updateTimeDisplayTask, "Update Time", 2048, NULL, 3, NULL);
  delay(10);
  xTaskCreate(syncTimeTask, "Sync Time", 2048, NULL, 1, NULL);
  delay(10);
}


void loop() {
  doWiFiManager();
  delay(10);
}
