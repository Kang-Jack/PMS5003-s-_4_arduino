#include "LASERK_PMS5003S.h"
#include <SoftwareSerial.h>

SoftwareSerial dust(2,3);

LASERK_PMS5003S pms(&dust);

void setup() {
  Serial.begin(115200);
  pms.begin();
}

void loop() {
  pms.sleep();
  delay(5000);
  pms.wakeUp();
  delay(5000);
}
