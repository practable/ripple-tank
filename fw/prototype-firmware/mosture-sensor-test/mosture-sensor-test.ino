/*   ripple tank level indicator test
#

*/

const int sensor_pin = A0;

int sensor_val;


void setup() {
  Serial.begin(115200);
  Serial.println("mosture-sense test");
}

void loop() {
  sensor_val = analogRead(sensor_pin);
  Serial.println(sensor_val);
  delay(250);
}
