#include "LASERK_PMS5003S.h"
//#include <SoftwareSerial.h>

//SoftwareSerial dust(2,3);//rx,tx

LASERK_PMS5003S pms(&Serial3);

void setup() {
  Serial.begin(115200);
  Serial3.begin(9600);
  pms.begin();
//  pms.wakeUp();
  pms.setMode(PASSIVE);
  Serial.println("setup");
}

void loop() {
  Serial.println("loop");
  pms.request();
  if(!pms.read()){
    return;
  }
  Serial.print("PM1.0(CF1)\t");
  Serial.print(pms.getPmCf1(1.0));
  Serial.print("ug/m3");
  Serial.println();
  Serial.print("PM2.5(CF1)\t");
  Serial.print(pms.getPmCf1(2.5));
  Serial.print("ug/m3");
  Serial.println();
  Serial.print("PM10(CF1)\t");
  Serial.print(pms.getPmCf1(10));
  Serial.print("ug/m3");
  Serial.println();
  Serial.print("PM1.0(ATO)\t");
  Serial.print(pms.getPmAto(1.0));
  Serial.print("ug/m3");
  Serial.println();
  Serial.print("PM2.5(ATO)\t");
  Serial.print(pms.getPmAto(2.5));
  Serial.print("ug/m3");
  Serial.println();
  Serial.print("PM10(ATO)\t");
  Serial.print(pms.getPmAto(10));
  Serial.print("ug/m3");
  Serial.println();
  Serial.print("  PCS0.3\t");
  Serial.print(pms.getPcs(0.3));
  Serial.print("pcs/0.1L");
  Serial.println();
  Serial.print("  PCS0.5\t");
  Serial.print(pms.getPcs(0.5));
  Serial.print("pcs/0.1L");
  Serial.println();
  Serial.print("  PCS1.0\t");
  Serial.print(pms.getPcs(1));
  Serial.print("pcs/0.1L");
  Serial.println();
  Serial.print("  PCS2.5\t");
  Serial.print(pms.getPcs(2.5));
  Serial.print("pcs/0.1L");
  Serial.println();
  Serial.print("  PCS5.0\t");
  Serial.print(pms.getPcs(5));
  Serial.print("pcs/0.1L");
  Serial.println();
  Serial.print("   PCS10\t");
  Serial.print(pms.getPcs(10));
  Serial.print("pcs/0.1L");
  Serial.println();
  Serial.print("Formalde\t");
  Serial.print(pms.getForm());
  Serial.print("ug/m3");
  Serial.println();
  Serial.println();
  delay(1000);
}
