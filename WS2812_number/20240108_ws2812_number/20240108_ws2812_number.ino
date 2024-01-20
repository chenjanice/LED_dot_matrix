#include "Freenove_WS2812_Lib_for_ESP32.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define LEDS_COUNT  320
#define LEDS_PIN  27
#define CHANNEL   1
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
    } else if (x >= 16 && x <= 23) {
        led_index = 128 + (x - 16) + 8 * y;
    } else if (x >= 24 && x <= 31) {
        led_index = 192 + (x - 24) + 8 * y;
    } else if (x >= 32 && x <= 39) {
        led_index = 256 + (x - 32) + 8 * y;
    }
    return led_index;
}

void setLedColors(int origin_x, int origin_y, int (*numberArray)[3],bool isLedOn) {
    for (int row = 0; row < 5; row++) {
      for (int col = 0; col < 3; col++) {
        int led_x = origin_x+col;
        int led_y = origin_y+row;
//        Serial.printf("origin_x=%d, col=%d, led_x=%d\n",origin_x,col,led_x );
        
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
  Serial.begin(115200);
  Serial.println("before strip");
  strip.begin();
  //strip.setBrightness(0);
  Serial.println("after strip");
  
}

void loop() {
  Serial.println("eeeeeeeee");
  int (*numbers[10])[3] = {number0, number1, number2, number3, number4, number5, number6, number7, number8, number9};

  
  for (int n = 0; n < 10; n++) {
    for (int l = 0; l < 15; l++) {
        strip.setBrightness(l*(l-1));
        setLedColors(1, 2, numbers[n],true); 
        setLedColors(5, 2, numbers[n],true);
        setLedColors(11, 2, numbers[n],true);
        setLedColors(15, 2, numbers[n],true);
        setLedColors(21, 2, numbers[n],true);
        setLedColors(25, 2, numbers[n],true);
        setLedColorsPoint(9,3,true);
        setLedColorsPoint(9,5,true);
        setLedColorsPoint(19,3,true);
        setLedColorsPoint(19,5,true); 
        
        strip.show();
        delay(20);
    }
    delay(900);
    for (int l = 15; l >1; l--) {
        strip.setBrightness(l*(l-1));
        setLedColors(1, 2, numbers[n],true); 
        setLedColors(5, 2, numbers[n],true);
        setLedColors(11, 2, numbers[n],true);
        setLedColors(15, 2, numbers[n],true);
        setLedColors(21, 2, numbers[n],true);
        setLedColors(25, 2, numbers[n],true);
        setLedColorsPoint(9,3,true);
        setLedColorsPoint(9,5,true);
        setLedColorsPoint(19,3,true);
        setLedColorsPoint(19,5,true);
        strip.show();
        delay(30);
    }
    setLedColors(1, 2, numbers[n],false); 
    setLedColors(5, 2, numbers[n],false);
    setLedColors(11, 2, numbers[n],false);
    setLedColors(15, 2, numbers[n],false);
    setLedColors(21, 2, numbers[n],false);
    setLedColors(25, 2, numbers[n],false);
    setLedColorsPoint(9,3,false);
    setLedColorsPoint(9,5,false);
    setLedColorsPoint(19,3,false);
    setLedColorsPoint(19,5,false);
    strip.show();
    delay(100); 

  }
 }
