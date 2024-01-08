#include "Freenove_WS2812_Lib_for_ESP32.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define LEDS_COUNT  128
#define LEDS_PIN  27
#define CHANNEL   0
Freenove_ESP32_WS2812 strip = Freenove_ESP32_WS2812(LEDS_COUNT, LEDS_PIN, CHANNEL);

void get_index(int x, int y, int *result) {
    if (x <= 7) {
        *result = x + 8 * y;
    } else if (x >= 8 && x <= 15) {
        *result = 64 + (x - 8) + 8 * y;
    }
}

void setLedColors(int startA, int startB, int (*numberArray)[2], int size) {
    int index;
    for (int i = 0; i < size; i++) {
        int ledA = startA + numberArray[i][0];
        int ledB = startB + numberArray[i][1];
        printf("LED Position: (%d, %d)\n", ledA, ledB);
        get_index(ledA, ledB, &index);
        strip.setLedColorData(index, 20, 20, 100); // 设置颜色
    }
}

void clearLedColors(int startA, int startB, int (*numberArray)[2], int size) {
    int index;
    for (int i = 0; i < size; i++) {
        int ledA = startA + numberArray[i][0];
        int ledB = startB + numberArray[i][1];
        get_index(ledA, ledB, &index);
        strip.setLedColorData(index, 0, 0, 0); // 清除颜色
    }
}


void setup() {
  strip.begin();
  strip.setBrightness(0);
  srand(time(NULL));
  Serial.begin(115200);
}

void loop() {
  int index;
  int a = 0;
  int b = 0;

  int number0[12][2] = {{a, b}, {a+1, b}, {a+2, b}, {a, b+1}, {a+2, b+1}, {a, b+2},{a+2, b+2}, {a, b+3}, {a+2, b+3}, {a, b+4}, {a+1, b+4}, {a+2, b+4}};
  int number1[5][2] = {{a + 1, b}, {a + 1, b + 1}, {a + 1, b + 2}, {a + 1, b + 3}, {a + 1, b + 4}};
  int number2[11][2] = {{a, b}, {a+1, b}, {a+2, b}, {a+2, b+1}, {a, b+2}, {a+1, b+2}, {a+2, b+2},{a, b+3}, {a, b+4}, {a+1, b+4}, {a+2, b+4}};
  int number3[11][2] = {{a, b}, {a+1, b}, {a+2, b}, {a+2, b+1}, {a, b+2}, {a+1, b+2}, {a+2, b+2},{a+2, b+3}, {a, b+4}, {a+1, b+4}, {a+2, b+4}};
  int number4[9][2] = {{a, b}, {a+2, b}, {a, b+1}, {a+2, b+1}, {a, b+2}, {a+1, b+2}, {a+2, b+2},{a+2, b+3}, {a+2, b+4}};
  int number5[11][2] = {{a, b}, {a+1, b}, {a+2, b}, {a, b+1}, {a, b+2}, {a+1, b+2}, {a+2, b+2},{a+2, b+3}, {a, b+4}, {a+1, b+4}, {a+2, b+4}};
  int number6[12][2] = {{a, b}, {a+1, b}, {a+2, b}, {a, b+1}, {a, b+2}, {a+1, b+2}, {a+2, b+2},{a, b+3}, {a+2, b+3}, {a, b+4}, {a+1, b+4}, {a+2, b+4}};
  int number7[7][2] = {{a, b}, {a+1, b}, {a+2, b}, {a+2, b+1}, {a+2, b+2}, {a+2, b+3}, {a+2, b+4}};
  int number8[13][2] = {{a, b}, {a+1, b}, {a+2, b}, {a, b+1}, {a+2, b+1}, {a, b+2}, {a+1, b+2}, {a+2, b+2},{a, b+3}, {a+2, b+3}, {a, b+4}, {a+1, b+4}, {a+2, b+4}};
  int number9[12][2] = {{a, b}, {a+1, b}, {a+2, b}, {a, b+1}, {a+2, b+1}, {a, b+2}, {a+1, b+2}, {a+2, b+2}, {a+2, b+3}, {a, b+4}, {a+1, b+4}, {a+2, b+4}};

  int (*numbers[10])[2] = {number0, number1, number2, number3, number4, number5, number6, number7, number8, number9};
  int sizes[] = {12, 5, 11, 11, 9, 11, 12, 7, 13, 12}; // Sizes of each array

  for (int n = 0; n < 10; n++) {
      for (int l = 0; l < 20; l++) {
        strip.setBrightness(l);
        setLedColors(0, 2, numbers[n], sizes[n]); 
        setLedColors(4, 2, numbers[n], sizes[n]); 
        setLedColors(8, 2, numbers[n], sizes[n]); 
        setLedColors(12, 2, numbers[n], sizes[n]);
        strip.show();
      }
      delay(100); 
      for (int l = 20; l >1; l--) {
        strip.setBrightness(l);
        setLedColors(0, 2, numbers[n], sizes[n]); 
        setLedColors(4, 2, numbers[n], sizes[n]); 
        setLedColors(8, 2, numbers[n], sizes[n]); 
        setLedColors(12, 2, numbers[n], sizes[n]);
        strip.show();
      }
      clearLedColors(0, 2, numbers[n], sizes[n]); 
      clearLedColors(4, 2, numbers[n], sizes[n]); 
      clearLedColors(8, 2, numbers[n], sizes[n]); 
      clearLedColors(12, 2, numbers[n], sizes[n]);
      strip.show();
      delay(1000); 

 }}
