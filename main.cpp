/* mbed Microcontroller Library
 * Copyright (c) 2019 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 */

#include "Adafruit_SSD1306.h"
#include "Stepper.h"
#include "Ultrasonic.h"
#include "mbed.h"

#define pinStep PA_5
#define pinEnable PA_7
#define pinDir PA_6

// an I2C sub-class that provides a constructed default
class I2CPreInit : public I2C {
public:
  I2CPreInit(PinName sda, PinName scl) : I2C(sda, scl) {
    frequency(100000);
    start();
  };
};

#define D_SDA PA_12
#define D_SCL PA_11

I2CPreInit gI2C(D_SDA, D_SCL);

// an SPI sub-class that provides a constructed default

Adafruit_SSD1306_I2c gOled(gI2C, NC, 0x78, 32, 128);

Ultrasonic ultrasonidos(PB_12, PB_2);
Stepper motor = Stepper(pinStep, pinDir, pinEnable);
enum direcciones { avance, retroceso } direccion;
enum estados {
  inicio,
  avanzando,
  calentando,
  retrocediendo,
  enfriando,
  emergencia
} estado;

DigitalOut led1(PA_10);
DigitalOut led2(PB_3);
DigitalOut led3(PB_5);
DigitalOut rele(PC_4);
DigitalIn bt1(PA_13);
DigitalIn bt2(PB_7);
DigitalIn bt3(PC_2);
PwmOut vent1(PA_9);
PwmOut vent2(PA_8);
DigitalIn final1(PC_7);
DigitalIn final2(PB_6);
InterruptIn hall1(PB_4);
AnalogIn scor(PA_0);
AnalogIn ntc(PC_5);

float vScor;
float vR1;
float R1 = 100000;
float Vcc = 3.3;
float rNTC;
float temp;
//
float R0 = 100000;
float beta = 3950;
float T0 = 298;
float voffset = Vcc / 2;
float sen = 8.108108110 * 2;
float corriente;

int contador = 0;
Ticker contRPM;
float rpm = 0;
float dist = 100;

void fContRPM();
void estadoInicio();
void estadoAvanzando();
void estadoCalentando();
void estadoRetroceciendo();
void estadoEnfriando();
void estadoemergencia();
void medir();
void flanco();
void visualizar();

estados estadoAnterior;

#define WAIT_TIME_MS 500

int main() {
  led1 = 1;
  led2 = 1;
  led3 = 1;
  rele = 0;
  motor.disableStepper();
  ultrasonidos.start();
  thread_sleep_for(100);
  gOled.clearDisplay();
  gOled.printf("Pulsa k1\r");
  gOled.display();
  ntc.set_reference_voltage(Vcc);
  scor.set_reference_voltage(Vcc);

  estado = inicio;
  // start mesuring the distance
  hall1.rise(&flanco);
  contRPM.attach(&fContRPM, 1s);
  vent2 = 0.0;
  direccion = avance;

  while (true) {
    medir();
    switch (estado) {
    case inicio:
      estadoInicio();
      break;
    case avanzando:
      estadoAvanzando();
      break;
    case calentando:
      estadoCalentando();
      break;
    case retrocediendo:
      estadoRetroceciendo();
      break;
    case enfriando:
      estadoEnfriando();
      break;
    case emergencia:
      estadoemergencia();
      break;
    }
    if (dist < 7) {
      estadoAnterior = estado;
      estado = emergencia;
    }
  }
}

void estadoInicio() {
  if (bt1) {
    estado = avanzando;
    motor.enableStepper();
    thread_sleep_for(1000);
  }
  visualizar();
}

void estadoAvanzando() {
  if (final1) {
    estado = calentando;
    rele = 1;
  }
  motor.doStepForward();
}

void estadoCalentando() {
  if (temp > 50) {
    rele = 0;
    estado = retrocediendo;
  }
  visualizar();
}

void estadoRetroceciendo() {
  if (final2) {
    estado = enfriando;
  }
  motor.doStepBackward();
}

void estadoEnfriando() {
  if (rpm > 50000) {
    vent1 = 0;
    estado = avanzando;
  }
  vent1 = vent1 + 0.01;
  visualizar();
  thread_sleep_for(100);
}

void estadoemergencia() {
  rele = 0;
  vent1 = 0;
  gOled.clearDisplay();
  thread_sleep_for(10);
  gOled.setTextCursor(0,0);
  gOled.printf("Emergencia");
  gOled.display();
}

void medir() {
  dist = ultrasonidos.read();
  vScor = scor.read_voltage();
  corriente = (vScor - voffset - 0.04390666671057333337724000004391) * sen;
  vR1 = ntc.read_voltage();
  rNTC = (Vcc - vR1) * R1 / vR1;
  temp = beta / (log(rNTC / R0) + (beta / T0)) - 273;
}

void fContRPM() {
  rpm = contador * 60;
  contador = 0;
  // printf("hola");
}

void flanco() { contador++; }

void visualizar() {
  gOled.clearDisplay();
  gOled.setTextCursor(0,0);
  gOled.printf("temp %.2f\r\n", temp);
  gOled.printf("int %.2fA\r\n", corriente);
  gOled.printf("rpm %.2f\r\n", rpm);
  printf("temp=%.2f intensidad=%.2f dist=%.2f rpm=%.2f\r\n", temp, corriente,
         dist, rpm);
   gOled.display();
}