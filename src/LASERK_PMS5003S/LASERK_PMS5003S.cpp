#include "LASERK_PMS5003S.h"

LASERK_PMS5003S::LASERK_PMS5003S(SoftwareSerial *serial) {
    sofSeri = serial;
    Seri = sofSeri;
}

LASERK_PMS5003S::LASERK_PMS5003S(HardwareSerial *serial) {
    hwSeri = serial;
    Seri = hwSeri;
}

int LASERK_PMS5003S::begin() {
    if (hwSeri) { hwSeri->begin(UART_BAUD); return 0; }
    if (sofSeri) { sofSeri->begin(UART_BAUD); return 0; }
    return -1;
}

void LASERK_PMS5003S::send(byte cmd, byte dataH, byte dataL) {
    Packet[2] = cmd;
    Packet[3] = dataH;
    Packet[4] = dataL;
    LRC = 0;
    foreach(Packet) {
        Seri->write(row);
        LRC += row;
    }
    Seri->write((LRC >> 8) & 0xff);
    Seri->write(LRC & 0xff);
}

void LASERK_PMS5003S::sleep() {
    send(PMS_CMD_SLEEP, 0x00, PMS_CMD_SLEEP_SLEEP);
}

void LASERK_PMS5003S::wakeUp() {
    send(PMS_CMD_SLEEP, 0x00, PMS_CMD_SLEEP_WAKEUP);
}

void LASERK_PMS5003S::setMode(byte mode) {
    send(PMS_CMD_MODE, 0x00, mode);
    while (Seri->available() > 0) {
        Seri->read();
    }
}

void LASERK_PMS5003S::request() {
    send(PMS_CMD_READ, 0x00, 0x00);
}
void LASERK_PMS5003S::parseG5s(unsigned char ucData)
{
    static unsigned int ucRxBuffer[250];
    static unsigned int ucRxCnt = 0;
    ucRxBuffer[ucRxCnt++] = ucData;
    if (ucRxBuffer[0] != 0x42 && ucRxBuffer[1] != 0x4D)
    {
        ucRxCnt = 0;
        return;
    }
    if (ucRxCnt > 32)//G5S-32，G5ST -38
    {
        data[DATA_PM10CF1] = (int)ucRxBuffer[4] * 256 + (int)ucRxBuffer[5];
        data[DATA_PM25CF1] = (int)ucRxBuffer[6] * 256 + (int)ucRxBuffer[7];
        data[DATA_PM100CF1] = (int)ucRxBuffer[8] * 256 + (int)ucRxBuffer[9];
        data[DATA_PM10ATO] = (int)ucRxBuffer[10] * 256 + (int)ucRxBuffer[11];
        data[DATA_PM25ATO] = (int)ucRxBuffer[12] * 256 + (int)ucRxBuffer[13];
        data[DATA_PM100ATO] = (int)ucRxBuffer[14] * 256 + (int)ucRxBuffer[15];
        data[DATA_PCS03ug] = (int)ucRxBuffer[16] * 256 + (int)ucRxBuffer[17];
        data[DATA_PCS05ug] = (int)ucRxBuffer[18] * 256 + (int)ucRxBuffer[19];
        data[DATA_PCS10ug] = (int)ucRxBuffer[20] * 256 + (int)ucRxBuffer[21];
        data[DATA_PCS25ug] = (int)ucRxBuffer[22] * 256 + (int)ucRxBuffer[23];
        data[DATA_PCS50ug] = (int)ucRxBuffer[24] * 256 + (int)ucRxBuffer[25];
        data[DATA_PCS100ug] = (int)ucRxBuffer[26] * 256 + (int)ucRxBuffer[27];
        data[DATA_FORMALDE] = ((int)ucRxBuffer[28] * 256 + (int)ucRxBuffer[29]);
        if (data[DATA_PM25CF1] > 999)
        {
            ucRxCnt = 0;
            return;
        }
        ucRxCnt = 0;
        return;
    }

}
int LASERK_PMS5003S::read(unsigned long timeout) {
    static unsigned long start;
    static int cnt;
    static unsigned int check;
    cnt = 0;
    check = 0;
    start = millis();
    while (Seri->available() < 32) {
        if (millis() - start > timeout) { return false; }
    }
    while (Seri->available() > 0) {
        parseG5s(Seri->read());
    }
    if (data[DATA_PM25CF1] > 999)
        return false;
    return true;
}

double LASERK_PMS5003S::getForm() {
    return data[DATA_FORMALDE] / 1000.000;
}

unsigned int LASERK_PMS5003S::getPcs(double pcs) {
    switch ((int)(pcs * 10)) {
    case 100: return data[DATA_PCS100ug];
    case 50: return data[DATA_PCS50ug];
    case 25: return data[DATA_PCS25ug];
    case 10: return data[DATA_PCS10ug];
    case 5: return data[DATA_PCS05ug];
    case 3: return data[DATA_PCS03ug];
    }
    return 0;
}

unsigned int LASERK_PMS5003S::getPmAto(double pm) {
    switch ((int)(pm * 10)) {
    case 100: return data[DATA_PM100ATO];
    case 25: return data[DATA_PM25ATO];
    case 10: return data[DATA_PM10ATO];
    }
    return 0;
}

unsigned int LASERK_PMS5003S::getPmCf1(double pm) {
    switch ((int)(pm * 10)) {
    case 100: return data[DATA_PM100CF1];
    case 25: return data[DATA_PM25CF1];
    case 10: return data[DATA_PM10CF1];
    }
    return 0;
}
