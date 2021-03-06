void divide_secs_2() {
  totalSecs = totalSecs / 2;
}

void divide_secs_3() {
  totalSecs = totalSecs / 3;
}

void set_total_secs_10() {
  totalSecs = 10;
}

void stopTimer() {
  timerOn = false;
}

void startTimer() {
  if (timerOn == false) {
    timerStartMillis = millis();
    timerOn = true;
  }
}

void resetTimer() {
  stopTimer();
  totalSecs = initialTimer;
  showTime(totalSecs);
  tm_display.clear();
}

void remoteReset() {
  stopTimer();
  totalSecs = remoteTimer;
  showStatus();
  delay(ONE_SECOND);
  showTime(totalSecs);
}
