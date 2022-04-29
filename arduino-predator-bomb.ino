#include <Arduino.h>
#include <TM1637Display.h>


// === PINS ==
#define DISPLAY_CLK_PIN 2
#define DISPLAY_DIO_PIN 3
#define BUTTON_1_PIN 4
#define BUTTON_2_PIN 5
#define BUTTON_3_PIN 6
#define DISPLAY_SWITCH_PIN 7
#define CARD_PIN 8
#define REMOTE_PIN 9
#define TUMBLER_PIN 10
#define BOOZER_PIN 11
#define BOOM_PIN 11

// === Settings ===
#define initialTimer 120  // в секундах


#define ONE_SECOND 1000
#define DEBOUNCE_DELAY 500

// === Components ===
TM1637Display tm_display(DISPLAY_CLK_PIN, DISPLAY_DIO_PIN);

// === Variables ===
uint8_t (*encodeDigit)(uint8_t);
void (*button1Push)();
void (*button2Push)();
void (*button3Push)();
int8_t totalSecs;
bool timerOn;
bool readTmblr;



void setup() {
  pinMode(BUTTON_1_PIN, INPUT);
  pinMode(BUTTON_2_PIN, INPUT);
  pinMode(BUTTON_3_PIN, INPUT);
  pinMode(DISPLAY_SWITCH_PIN, INPUT);
  pinMode(CARD_PIN, INPUT);
  pinMode(REMOTE_PIN, INPUT);
  pinMode(TUMBLER_PIN, INPUT);
  pinMode(BOOZER_PIN, OUTPUT);
  pinMode(BOOM_PIN, OUTPUT);

  
  encodeDigit = encodePredatorDigit; // tm_display.encodeDigit;
  button1Push = divide_secs_2;
  button2Push = divide_secs_3;
  button2Push = set_total_secs_10;
  timerOn = false;
  readTmblr = true;
}

void loop() {
  checkControls();
  if (timerOn) {
    tick();
  }
}

void checkControls() {
  if (digitalRead(BUTTON_1_PIN) == HIGH) {
    static uint32_t lastDebounce1Time;
    if ((millis() - lastDebounce1Time) > DEBOUNCE_DELAY) {
      button1Push();
      lastDebounce1Time = millis();
    }
  }
  if (digitalRead(BUTTON_2_PIN) == HIGH) {
    static uint32_t lastDebounce2Time;
    if ((millis() - lastDebounce2Time) > DEBOUNCE_DELAY) {
      button2Push();
      lastDebounce2Time = millis();
    }
  }
  if (digitalRead(BUTTON_3_PIN) == HIGH) {
    static uint32_t lastDebounce3Time;
    if ((millis() - lastDebounce3Time) > DEBOUNCE_DELAY) {
      button3Push();
      lastDebounce3Time = millis();
    }
  }
  if (digitalRead(DISPLAY_SWITCH_PIN) == HIGH) {
    encodeDigit = encodeHumanDigit;
  } else {
    encodeDigit = encodePredatorDigit;
  }
  if (digitalRead(CARD_PIN) == HIGH) {
    button1Push = stopTimer;
    button2Push = startTimer;
    button2Push = resetTimer;
  } else {
    button1Push = divide_secs_2;
    button2Push = divide_secs_3;
    button2Push = set_total_secs_10;
  }
  if (digitalRead(REMOTE_PIN) == HIGH) {
    static uint32_t lastDebounceRemoteTime;
    if ((millis() - lastDebounceRemoteTime) > DEBOUNCE_DELAY) {
      resetTimer();
      startTimer();
      lastDebounceRemoteTime = millis();
    }
  }
  if (digitalRead(TUMBLER_PIN) == HIGH) {
    if (readTmblr) {
      timerOn = true;
      readTmblr = false;
    }
  } else {
    readTmblr = true;
  }
}

void tick()
{
  if (totalSecs < 0) {
    return boom();
  }
  
  static uint32_t previousMillis;
  uint32_t currentMillis = millis();
  if (currentMillis - previousMillis >= ONE_SECOND) {
    previousMillis = currentMillis;

    totalSecs -= 1;
    showTime(totalSecs);
  }
}

void boom()
{
  digitalWrite(BOOM_PIN, HIGH);
}
