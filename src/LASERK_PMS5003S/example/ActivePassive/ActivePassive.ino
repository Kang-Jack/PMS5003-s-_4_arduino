#include "LASERK_PMS5003S.h"
#include <SoftwareSerial.h>

SoftwareSerial dust(2,3);

LASERK_PMS5003S pms(&dust);

void setup() {
  Serial.begin(115200);
  pms.begin();
  pms.wakeUp();
}

void loop() {
  pms.setMode(PASSIVE);
  Serial.println("PASSIVE MODE 2");
  for(int i = 0;i < 10; i++){
    pms.request();
    pms.read();
    Serial.print("Ato : ");
    Serial.println(pms.getPmAto(2.5));
    delay(100);
  }
  Serial.println();

  pms.setMode(ACTIVE);
  Serial.println("ACTIVE MODE");
  unsigned long start = millis();
  while(millis() - start < 5000){
    pms.read();
    Serial.print("Ato : ");
    Serial.println(pms.getPmAto(2.5));
  }
  Serial.println();
}
