#include "Freenove_WS2812_Lib_for_ESP32.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <WiFiManager.h> // 引入WiFiManager庫，用於管理WiFi連接
#include <ESPmDNS.h> // 引入ESP32的mDNS庫，用於本地名稱解析

#define LEDS_COUNT  320
#define LEDS_PIN  27
#define CHANNEL   1
#define TRIGGER_PIN 15 // 定義用於觸發配置門戶的GPIO引腳號

Freenove_ESP32_WS2812 strip = Freenove_ESP32_WS2812(LEDS_COUNT, LEDS_PIN, CHANNEL);
WiFiManager wm; // 創建WiFiManager類的實例

unsigned int  timeout   = 5; // 設定門戶運行的超時時間為5秒
unsigned int  startTime = millis(); // 記錄程序啟動的起始時間
bool portalRunning      = false; // 標誌位，追踪配置門戶是否正在運行
bool startAP            = true; // 標誌位，用於決定是啟動AP+網頁伺服器(true)還是僅啟動網頁伺服器(false)

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

const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 8*60*60;
const int   daylightOffset_sec = 0;

struct TimeInfo {
  int hour;
  int minute;
  int second;
};


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

void setLedColors(int origin_x, int origin_y, int (*numberArray)[3],bool isLedOn, int r, int g, int b) {
    for (int row = 0; row < 5; row++) {
      for (int col = 0; col < 3; col++) {
        int led_x = origin_x+col;
        int led_y = origin_y+row;
//        Serial.printf("origin_x=%d, col=%d, led_x=%d\n",origin_x,col,led_x );
        
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


void setLedColorsIcon(int origin_x, int origin_y, int (*numberArray)[7],bool isLedOn, int r, int g, int b) {
    for (int row = 0; row < 7; row++) {
      for (int col = 0; col < 7; col++) {
        int led_x = origin_x+col;
        int led_y = origin_y+row;
//        Serial.printf("origin_x=%d, col=%d, led_x=%d\n",origin_x,col,led_x );
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

void setLedColorsPoint(int led_x, int led_y,bool isLedOn, int r, int g, int b) {
    if (isLedOn == true){
        strip.setLedColorData(get_index(led_x, led_y), r, g, b);
    }else{
        strip.setLedColorData(get_index(led_x, led_y), 0, 0, 0); 
    }
}

TimeInfo getLocalTime()
{
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return TimeInfo{-1, -1, -1}; 
  }
  mktime(&timeinfo); // 正規化時間結構

  TimeInfo result;
  result.hour = timeinfo.tm_hour;
  result.minute = timeinfo.tm_min;
  result.second = timeinfo.tm_sec;
  return result;
}

void convertTimeToArray(TimeInfo currentTime, int* timeArray) {
    if (currentTime.hour != -1) {
        // 分解小時、分鐘和秒，並將每一位數字存儲到陣列中
        timeArray[0] = currentTime.hour / 10;
        timeArray[1] = currentTime.hour % 10;
        timeArray[2] = currentTime.minute / 10;
        timeArray[3] = currentTime.minute % 10;
        timeArray[4] = currentTime.second / 10;
        timeArray[5] = currentTime.second % 10;
    } else {
        // 如果獲取時間失敗，設置所有陣列元素為 -1
        for(int i = 0; i < 6; i++) {
            timeArray[i] = -1;
        }
    }
}

void doWiFiManager(){
  if(portalRunning){ // 如果配置門戶當前正在運行
    wm.process(); // 處理WiFiManager任務

    // 檢查門戶是否超過了超時時間
    if((millis()-startTime) > (timeout*1000)){
      Serial.println("portaltimeout"); // 在串行監視器中打印超時消息
      portalRunning = false; // 指示門戶不再運行
      if(startAP){
        wm.stopConfigPortal(); // 如果啟動了配置門戶，則停止配置門戶
      }
      else{
        wm.stopWebPortal(); // 如果啟動了網頁門戶，則停止網頁門戶
      } 
   }
  }

  // 檢查是否按下了觸發按鈕且門戶當前未運行
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


void setup() {
  WiFi.mode(WIFI_STA); // 將WiFi模式設定為STA(站點)，以連接到路由器

  strip.begin();
  
  Serial.begin(115200);
  Serial.setDebugOutput(true); // 啟用詳細的調試輸出在串行監視器中
  Serial.println("\n Starting");

  pinMode(TRIGGER_PIN, INPUT_PULLUP); // 將觸發引腳設置為輸入模式並啟用上拉電阻
  wm.setHostname("MDNSEXAMPLE"); // 為mDNS回應器設置主機名
  wm.autoConnect(); // 自動嘗試連接到之前保存的WiFi網絡
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
}


void loop() {
  doWiFiManager(); 
  
  int time_r = 60; int time_g = 20; int time_b = 5;
  int point_r = 60; int point_g = 20; int point_b = 0;
  int icon_r = 60; int icon_g = 20; int icon_b = 0;
  
  TimeInfo currentTime = getLocalTime();
  convertTimeToArray(currentTime, timeArray); // 將時間轉換為數字陣列
  Serial.printf("Current Time: %02d:%02d:%02d\n", currentTime.hour, currentTime.minute, currentTime.second);
  Serial.printf("icon_idx: %d\n",icon_idx);

  if (icon_idx < 4){
    icon_idx += 1;
  }else{
    icon_idx = 0;
  }

  for (int l = 1; l < 10; l++) {
    strip.setBrightness(l*(l-1));

    setLedColors(1, 2, numbers[timeArray[0]],true, time_r, time_g, time_b); 
    setLedColors(5, 2, numbers[timeArray[1]],true, time_r, time_g, time_b); 
    setLedColors(11, 2, numbers[timeArray[2]],true, time_r, time_g, time_b); 
    setLedColors(15, 2, numbers[timeArray[3]],true, time_r, time_g, time_b); 
    setLedColors(21, 2, numbers[timeArray[4]],true, time_r, time_g, time_b); 
    setLedColors(25, 2, numbers[timeArray[5]],true, time_r, time_g, time_b); 
    setLedColorsPoint(9,3,true,point_r, point_g, point_b);
    setLedColorsPoint(9,5,true,point_r, point_g, point_b);
    setLedColorsPoint(19,3,true,point_r, point_g, point_b);
    setLedColorsPoint(19,5,true,point_r, point_g, point_b);
    setLedColorsIcon(29+icon_idx, 1, icons[icon_idx],true, icon_r, icon_g, icon_b);
    
    strip.show();
    delay(10);
  }
  delay(850);
  for (int l = 10; l >1; l--) {
    strip.setBrightness(l*(l-1));
    setLedColors(1, 2, numbers[timeArray[0]],true, time_r, time_g, time_b); 
    setLedColors(5, 2, numbers[timeArray[1]],true, time_r, time_g, time_b); 
    setLedColors(11, 2, numbers[timeArray[2]],true, time_r, time_g, time_b); 
    setLedColors(15, 2, numbers[timeArray[3]],true, time_r, time_g, time_b); 
    setLedColors(21, 2, numbers[timeArray[4]],true, time_r, time_g, time_b); 
    setLedColors(25, 2, numbers[timeArray[5]],true, time_r, time_g, time_b); 
    setLedColorsPoint(9,3,true,point_r, point_g, point_b);
    setLedColorsPoint(9,5,true,point_r, point_g, point_b);
    setLedColorsPoint(19,3,true,point_r, point_g, point_b);
    setLedColorsPoint(19,5,true,point_r, point_g, point_b);
    setLedColorsIcon(29+icon_idx, 1, icons[icon_idx],true, icon_r, icon_g, icon_b);
    
    strip.show();
    delay(10);
  }
}
