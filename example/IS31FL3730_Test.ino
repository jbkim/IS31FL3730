#include <Wire.h>
#include <IS31FL3730.h>

#define M1 0
#define M2 1

IS31FL3730 m1;
IS31FL3730 m2;
IS31FL3730 m3;


void setup() {
  Serial.begin(9600);
  Serial.println("ISSI test");

  m1.begin(0x61);
  m2.begin(0x62);
  m3.begin(0x63);

  m1.clear();
  m2.clear();
  m3.clear();

 }



void loop() {
  fill_matrix(m1);
  fill_matrix(m2);
  fill_matrix(m3);
  delay(10);
  clear_matrix(m1);
  clear_matrix(m2);
  clear_matrix(m3);
  delay(10);
}


void clear_matrix(IS31FL3730 m) {
  for (uint8_t y = 0; y < 7; y++) {
    for (uint8_t x = 0; x < 5; x++) {
      m.set_pixel(M1, x, y, 0);
      m.set_pixel(M2, x, y, 0);
      m.update();
      delay(10);
    }
  }
  m.set_decimal(M1, 0);
  m.set_decimal(M2, 0);
  m.update();
  delay(10);
}


void fill_matrix(IS31FL3730 m) {
  for (uint8_t y = 0; y < 7; y++) {
    for (uint8_t x = 0; x < 5; x++) {
      m.set_pixel(M1, x, y, 1);
      m.set_pixel(M2, x, y, 1);
     m.update();
     delay(10);
    }
  }
  m.set_decimal(M1, 1);
  m.set_decimal(M2, 1);
  m.update();
  delay(10);
}
