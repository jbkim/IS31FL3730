#include <Arduino.h>
#include <Wire.h>
#include <IS31FL3730.h>

#define Default_Mode      0x18
#define Default_reg_le    0x17  // 35mA
#define Matrix_1          0
#define Matrix_2          1

boolean IS31FL3730::begin(uint8_t addr) {
  Wire.begin();

  _i2caddr = addr;
  _brightness = 127;

  writeRegister8(_i2caddr, ISSI_REG_CONFIG, Default_Mode);
  writeRegister8(_i2caddr, ISSI_REG_LE, Default_reg_le);
  writeRegister8(_i2caddr, ISSI_REG_BRIGHT, _brightness);

  for (uint8_t i=0; i < 8; i++) {
      _buf_matrix_1[i] = 0;
      _buf_matrix_2[i] = 0;
  }

  return true;
}

void IS31FL3730::update(void) {

  Wire.beginTransmission(_i2caddr);
  Wire.write((byte)ISSI_REG_MATRIX1);
  for (uint8_t i = 0; i < 8; i++)
    Wire.write((byte)_buf_matrix_1[i]);
  Wire.endTransmission();

  Wire.beginTransmission(_i2caddr);
  Wire.write((byte)ISSI_REG_MATRIX2);
  for (uint8_t i = 0; i < 8; i++)
    Wire.write((byte)_buf_matrix_2[i]);
  Wire.endTransmission();

  writeRegister8(_i2caddr, ISSI_REG_UPDATE, 0x01);
  
}

void IS31FL3730::clear(void) {

  for (uint8_t i=0; i < 8; i++) {
      _buf_matrix_1[i] = 0;
      _buf_matrix_2[i] = 0;
  }
  update();
}

void IS31FL3730::set_bright(uint8_t bright) {
  _brightness = bright;
  if (_brightness > 127) _brightness = 127;

  writeRegister8(_i2caddr, ISSI_REG_BRIGHT, _brightness);
}

void IS31FL3730::set_decimal(uint8_t matrix, uint8_t c) {
  if (matrix == Matrix_1) {
    if (c == 1) _buf_matrix_1[6] |= 0x80;
    else _buf_matrix_1[6] &= 0x7F;

  }else if (matrix == Matrix_2) {
    if (c == 1) _buf_matrix_2[7] |= 0x40;
    else _buf_matrix_2[7] &= 0xBF;
  }

}

void IS31FL3730::set_pixel(uint8_t matrix, uint8_t x, uint8_t y, uint8_t c) {
  if (matrix == Matrix_1) {
    if (c == 1) _buf_matrix_1[y] |= (1 << x);
    else _buf_matrix_1[y] &= ~(1 << x);

  }else if (matrix == Matrix_2) {
    if (c == 1) _buf_matrix_2[x] |= (1 << y);
    else _buf_matrix_2[x] &= ~(1 << y);
  }
}

void IS31FL3730::set_col(uint8_t matrix, uint8_t col, uint8_t data) {
  for (uint8_t y = 0; y < 7; y++)
    set_pixel(matrix, col, y, (data & (1 << y)) > 0);
}

void IS31FL3730::set_row(uint8_t matrix, uint8_t row, uint8_t data) {
  for (uint8_t x = 0; x < 5; x++)
    set_pixel(matrix, x, row, (data & (1 << (4-x))) > 0);
}

void IS31FL3730::set_data(uint8_t matrix, uint8_t *data) {
  for (uint8_t y = 0; y < 7; y++)
    set_row(matrix, y, data[y]);
}

void IS31FL3730::writeRegister8(uint8_t addr, uint8_t reg, uint8_t data) {

  Wire.beginTransmission(addr);
  Wire.write((byte)reg);
  Wire.write((byte)data);
  Wire.endTransmission();
}

