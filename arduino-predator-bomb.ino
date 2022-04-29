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
#define BOOM_PIN 12

// === Settings ===
const uint32_t initialTimer = 30 * 60;  // в секундах
const uint32_t remoteTimer = 2 * 60;  // в секундах

#define ONE_SECOND 1000
#define DEBOUNCE_DELAY 500
#define BOOM_DURATION 5000

// === Components ===
TM1637Display tm_display(DISPLAY_CLK_PIN, DISPLAY_DIO_PIN);

// === Variables ===
uint8_t (*encodeDigit)(uint8_t);
void (*button1Push)();
void (*button2Push)();
void (*button3Push)();
uint32_t totalSecs;
uint32_t timerStartMillis;
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

  digitalWrite(BOOZER_PIN, LOW);
  digitalWrite(BOOM_PIN, LOW);

  
  encodeDigit = encodePredatorDigit; // tm_display.encodeDigit;
  button1Push = divide_secs_2;
  button2Push = divide_secs_3;
  button3Push = set_total_secs_10;
  timerOn = false;
  readTmblr = true;

  tm_display.clear();
  tm_display.setBrightness(7);
}

void loop() {
  checkControls();
  tick();
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
    button3Push = resetTimer;
    encodeDigit = encodeHumanDigit;
  } else {
    button1Push = divide_secs_2;
    button2Push = divide_secs_3;
    button3Push = set_total_secs_10;
    encodeDigit = encodePredatorDigit;
  }
  if (digitalRead(REMOTE_PIN) == HIGH) {
    static uint32_t lastDebounceRemoteTime;
    if ((millis() - lastDebounceRemoteTime) > DEBOUNCE_DELAY) {
      remoteReset();
      startTimer();
      lastDebounceRemoteTime = millis();
    }
  }
  if (digitalRead(TUMBLER_PIN) == HIGH) {
    if (readTmblr) {
      resetTimer();
      startTimer();
      readTmblr = false;
    }
  } else {
    readTmblr = true;
  }
}

void tick() {
  if (!timerOn) {
    return;
  }
  if (totalSecs <= 0) {
    stopTimer();
    boom();
    return;
  }

  uint32_t curMillis = millis();
  uint32_t secondsLeft = (curMillis - timerStartMillis) / 1000ul;
  if (secondsLeft > 0) {
    timerStartMillis = curMillis;
    totalSecs = totalSecs - 1ul;
    showTime(totalSecs);
  }
}

void sad_melody() {
  digitalWrite(BOOZER_PIN, HIGH);
  delay(200);
  digitalWrite(BOOZER_PIN, LOW);
  delay(200);
  digitalWrite(BOOZER_PIN, HIGH);
  delay(200);
  digitalWrite(BOOZER_PIN, LOW);
  delay(200);
  digitalWrite(BOOZER_PIN, HIGH);
  delay(200);
  digitalWrite(BOOZER_PIN, LOW);
  delay(200);
}

void boom()
{
  delay(ONE_SECOND);
  sad_melody();
  digitalWrite(BOOM_PIN, HIGH);
  delay(BOOM_DURATION);
  digitalWrite(BOOM_PIN, LOW);
}
