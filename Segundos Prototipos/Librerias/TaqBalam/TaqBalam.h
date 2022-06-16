
#ifndef TAQBALAM_H
#define TAQBALAM_H
#include <Arduino.h>

void inicioMPU();
void MPUwrite(byte reg, byte datas);
byte MPUread(byte reg);
void calcoffset(bool console);
void mpuupdate();
void sendArray(int *palabra);
void conf_bluetooth(int BaudRate);
void recibirdatos();
void sendBLUE();
void limpiararreglos(void);
void cargardatos();

extern float roll, pitch, yaw;
extern float datos_convertidos[10];
extern float datos_revertidos[10];

#endif


