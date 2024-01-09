#include "Freenove_WS2812_Lib_for_ESP32.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define LEDS_COUNT  128
#define LEDS_PIN  27
#define CHANNEL   0
Freenove_ESP32_WS2812 strip = Freenove_ESP32_WS2812(LEDS_COUNT, LEDS_PIN, CHANNEL);

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

int get_index(int x, int y) {
    int led_index;
    if (x <= 7) {
        led_index = x + 8 * y;
    } else if (x >= 8 && x <= 15) {
        led_index = 64 + (x - 8) + 8 * y;
    }
    return led_index;
}

void setLedColors(int origin_x, int origin_y, int (*numberArray)[3],bool isLedOn) {
    for (int row = 0; row < 5; row++) {
      for (int col = 0; col < 3; col++) {
        int led_x = origin_x+col;
        int led_y = origin_y+row;
        
        if (numberArray[row][col] == 1){
          if (isLedOn == true){
            strip.setLedColorData(get_index(led_x, led_y), row*40, 20, 100); 
          }else{
             strip.setLedColorData(get_index(led_x, led_y), 0, 0, 0); 
          }
        }
      }
    }
}

void setLedColorsPoint(int led_x, int led_y,bool isLedOn) {
    if (isLedOn == true){
        strip.setLedColorData(get_index(led_x, led_y), 20, 20, 100);
    }else{
        strip.setLedColorData(get_index(led_x, led_y), 0, 0, 0); 
    }
}

void setup() {
  strip.begin();
  strip.setBrightness(0);
  srand(time(NULL));
  Serial.begin(115200);
}

void loop() {  
  int (*numbers[10])[3] = {number0, number1, number2, number3, number4, number5, number6, number7, number8, number9};

  
  for (int n = 0; n < 10; n++) {
    for (int l = 0; l < 15; l++) {
        strip.setBrightness(l*(l-1));
        setLedColors(0, 2, numbers[n],true); 
        setLedColors(8, 2, numbers[n],true);
        setLedColorsPoint(5,3,true);
        setLedColorsPoint(5,5,true);
        strip.show();
        delay(20);
    }
    delay(900);
    for (int l = 15; l >1; l--) {
        strip.setBrightness(l*(l-1));
        setLedColors(0, 2, numbers[n],true);
        setLedColors(8, 2, numbers[n],true);
        setLedColorsPoint(5,3,true);
        setLedColorsPoint(5,5,true);
        strip.show();
        delay(30);
    }
    setLedColors(0, 2, numbers[n],false);
    setLedColors(8, 2, numbers[n],false);
    setLedColorsPoint(5,3,false);
    setLedColorsPoint(5,5,false);
    strip.show();
    delay(100); 

  }
 }
