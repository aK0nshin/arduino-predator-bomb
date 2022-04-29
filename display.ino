const uint8_t digitToPredatorSegment[] = {
//      A
//     ---
//  F |   | B
//     -G-
//  E |   | C
//     ---
//      D
 // XGFEDCBA
  0b00101101,    // 0
  0b01001001,    // 1
  0b00100110,    // 2
  0b01011100,    // 3
  0b00100011,    // 4
  0b01000110,    // 5
  0b00111100,    // 6
  0b00101011,    // 7
  0b01100101,    // 8
  0b00010101,    // 9
  0b01100001,    // Г
  0b01111111,    // В
  0b01110110,    // Н
  0b00111111,    // О
  0b01111001,    // E
  0b01110001     // F
  };

const uint8_t digitToHumanSegment[] = {
//      A
//     ---
//  F |   | B
//     -G-
//  E |   | C
//     ---
//      D
 // XGFEDCBA
  0b00111111,    // 0
  0b00000110,    // 1
  0b01011011,    // 2
  0b01001111,    // 3
  0b01100110,    // 4
  0b01101101,    // 5
  0b01111101,    // 6
  0b00000111,    // 7
  0b01111111,    // 8
  0b01101111,    // 9
  0b01110111,    // A
  0b01111100,    // b
  0b00111001,    // C
  0b01011110,    // d
  0b01111001,    // E
  0b01110001     // F
  };

uint8_t encodePredatorDigit(uint8_t digit) {
  return digitToPredatorSegment[digit & 0x0f];
}

uint8_t encodeHumanDigit(uint8_t digit) {
  return digitToHumanSegment[digit & 0x0f];
}

uint8_t* encodePair(uint8_t x) {
  static uint8_t r[2];
  r[0] = encodeDigit(x / 10ul);
  r[1] = encodeDigit(x % 10ul);
  return r;
}

void showTime(uint8_t secs) {
//  uint8_t data[] = { secs / , 0, 0, 0 };
  tm_display.showNumberDec(secs);

//  tm_display.setSegments(encodePair(secs / 60ul), 2, 0);
//  tm_display.setSegments(encodePair(secs % 60ul), 2, 2);
}

void showStatus() {
  uint8_t data[] = { encodePredatorDigit(10), encodePredatorDigit(11), encodePredatorDigit(12), encodePredatorDigit(13) };
  tm_display.setSegments(data);
}
