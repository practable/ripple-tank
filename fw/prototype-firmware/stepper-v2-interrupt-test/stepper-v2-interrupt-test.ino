
// Pump
const int pump_step_pin = 6;
const int pump_dir_pin = 4;
const int pump_EN_pin = 2;
const int pump_fault_pin = A3;
const int pump_sleep_pin = A6;
const int pump_reset_pin = A7;

void pump_setup() {
 pinMode(pump_step_pin, OUTPUT);  
  pinMode(pump_dir_pin, OUTPUT);
  pinMode(pump_EN_pin, OUTPUT);
  pinMode(pump_sleep_pin, OUTPUT);
  pinMode(pump_reset_pin, OUTPUT);
}

void enable_pump() {
  digitalWrite(pump_EN_pin, 0);
  digitalWrite(pump_sleep_pin, 1);
  digitalWrite(pump_reset_pin, 1);
}


void disable_pump() {
  digitalWrite(pump_step_pin, 0);
  digitalWrite(pump_dir_pin, 0);
  digitalWrite(pump_EN_pin, 0);
  digitalWrite(pump_sleep_pin, 0);
  digitalWrite(pump_reset_pin, 1);
}

void set_direction(bool direction = false) {
  digitalWrite(pump_dir_pin, direction);
}


volatile bool pump_enabled = false;

// ===== STEP PULSE ISR =====
void TC5_Handler(){
  if (TC5->COUNT16.INTFLAG.bit.MC0)  {
    TC5->COUNT16.INTFLAG.reg = TC_INTFLAG_MC0; // clear interrupt

    if (!pump_enabled) return;

    // STEP HIGH
    PORT->Group[g_APinDescription[pump_step_pin].ulPort]
      .OUTSET.reg = (1 << g_APinDescription[pump_step_pin].ulPin);

    // ~3 µs delay @ 48 MHz
    for (volatile int i = 0; i < 150; i++) __NOP();

    // STEP LOW
    PORT->Group[g_APinDescription[pump_step_pin].ulPort]
      .OUTCLR.reg = (1 << g_APinDescription[pump_step_pin].ulPin);
  }
}

void setupTimerTC5(uint16_t compare){
  // Enable TC5 clock
  GCLK->CLKCTRL.reg =
    GCLK_CLKCTRL_ID(GCM_TC4_TC5) |
    GCLK_CLKCTRL_CLKEN |
    GCLK_CLKCTRL_GEN_GCLK0;

  while (GCLK->STATUS.bit.SYNCBUSY);

  TC5->COUNT16.CTRLA.reg = TC_CTRLA_SWRST;
  while (TC5->COUNT16.STATUS.bit.SYNCBUSY);

  TC5->COUNT16.CTRLA.reg =
    TC_CTRLA_MODE_COUNT16 |
    TC_CTRLA_PRESCALER_DIV1024 |
    TC_CTRLA_WAVEGEN_MFRQ;

  TC5->COUNT16.CC[0].reg = compare;
  while (TC5->COUNT16.STATUS.bit.SYNCBUSY);

  TC5->COUNT16.INTENSET.reg = TC_INTENSET_MC0;

  NVIC_EnableIRQ(TC5_IRQn);

  TC5->COUNT16.CTRLA.bit.ENABLE = 0; // start disabled
  while (TC5->COUNT16.STATUS.bit.SYNCBUSY);
}


void start_pump(){
  digitalWrite(pump_step_pin, LOW);

  pump_enabled = true;

  TC5->COUNT16.COUNT.reg = 0;   // reset phase
  while (TC5->COUNT16.STATUS.bit.SYNCBUSY);

  TC5->COUNT16.CTRLA.bit.ENABLE = 1;
  while (TC5->COUNT16.STATUS.bit.SYNCBUSY);
}

void stop_pump(){
  TC5->COUNT16.CTRLA.bit.ENABLE = 0;
  while (TC5->COUNT16.STATUS.bit.SYNCBUSY);

  pump_enabled = false;
  digitalWrite(pump_step_pin, LOW);
}




void setup() {
  Serial.begin(115200);
  delay(3000);
  Serial.println("StepperTesting V2!");
   pump_setup();
  set_direction(true);
  enable_pump();
  setupTimerTC5(35); //
}

void loop(){
  enable_pump();
  start_pump();
  delay(2000);

  stop_pump();
  disable_pump();
  delay(2000);
}
