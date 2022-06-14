#ifndef TAQBALAM_H
#define TAQBALAM_H
#include <Arduino.h>

void inicioMPU();
void MPUwrite(byte reg, byte datas);
byte MPUread(byte reg);
void calcoffset(bool console);
void mpuupdate();
void sendArray(int *palabra);

extern float roll, pitch, yaw;

#endif


