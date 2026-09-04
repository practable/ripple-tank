

void set_brightness(uint16_t brightness = 0) {

  pwm.analogWrite(led_ctrl_pin, brightness);
  // Serial.print(pwm.frequency(1));
  // TCC1->CC[0].reg = brightness;                          // TCC1 CC0 - 1262 is 100% duty cycle
  // while (TCC1->SYNCBUSY.bit.CC0);
}