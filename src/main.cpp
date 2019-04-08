/**
 * @file main.cpp
 * @brief rhomb.io SIM868 demo
 * @author rhomb.io Software Team
 *
 * Version 1.0
 *
 * @copyright Copyright (c) 2019
 */

#include <Arduino.h>

// Include rhio-pinmap and enable one MCU
// #define S100_DUINO_UNO_v1_0
// #define S100_DUINO_LEONARDO_v1_0
// #define S200_DUINO_UNO_PRO_v1_0
#define S200_DUINO_ZERO_v1_0
// #define S100_DUINO_MEGA_v2_0
// #define S100_ESP32_WROOM_32_v1_0
// #define S100_ESP_WROOM_02_v1_0
#include "rhio-pinmap.h"

// SIM868 pins on Socket 1
#define GSM_PWREN  IO0
#define GSM_PWRKEY IO1
#define GPS_EN     IO2
#define GSM_STATUS IO3

// SIM868 pins on Socket 2 (only Duino Mega)
// #define GSM_PWREN  IO8
// #define GSM_PWRKEY IO9
// #define GPS_EN     IO10
// #define GSM_STATUS IO11

// Monitor serial (debug) and modem At commands
#ifdef S200_DUINO_ZERO_v1_0
  #define SerialMon SerialUSB
  #define SerialAt Serial1 // Serial1 = UART-A
#endif

#ifdef S100_DUINO_UNO_v1_0
  #include SoftwareSerial
  SoftwareSerial SoftSerial(IO6, IO7); // also may be SDA,SCL, MISO,MOSI and others
  #define SerialMon SoftSerial
  #define SerialAt Serial // Serial1 = UART-A
#endif

#ifdef S100_DUINO_MEGA_v2_0
  #define SerialMon Serial1 // UART-B
  #define SerialAt Serial // UART-A
#endif

// At commands responses will be allocated here
char buffer[60];

void configureIos () {
  pinMode(GSM_PWRKEY, OUTPUT);
  pinMode(GSM_PWREN, OUTPUT);
  pinMode(GPS_EN, OUTPUT);
  pinMode(GSM_STATUS, INPUT);
}

void configureSerial () {
  SerialMon.begin(38400);
  while (!SerialMon) {}

  SerialAt.begin(38400);
  while (!SerialAt) {}
}

/**
 * @brief Turn on the modem
 *
 * To be able to use the SIM868 module we have to activate the PWREN line and
 * make a pulse of 1.5 seconds in the PWRKEY line. The modem will have been
 * turned on when the STATUS line goes to high level.
 *
 * This allows the MCU to control the power of the device, necessary for low
 * power applications. By default the modem will be turned off and will only
 * turn on if we perform the turn on routine as seen in this method.
 *
 * @return true if the modem has been turned on
 * @return false
 */
bool powerOn () {
  uint8_t gsm_status;

  digitalWrite(GSM_PWREN, HIGH);

  for (char i=0; i<5; i++) {
    gsm_status = digitalRead(GSM_STATUS);
    if (gsm_status == HIGH){
      SerialMon.println(F("GSM HIGH!!"));
      break;
    } else {
      SerialMon.println(F("GSM LOW!"));
      digitalWrite(GSM_PWRKEY, HIGH);
      delay(1500);
      digitalWrite(GSM_PWRKEY, LOW);
      delay(1500);
    }
  }

  if (!gsm_status) {
    return false;
  }

  // Turn on the GPS. It is not necessary for modem operation but this demo
  // will make use of GPS.
  digitalWrite(GPS_EN, HIGH);
  SerialAt.println("AT+CGNSPWR=1");
  delay(3000);

  // We obviate the result of enabling GPS
  return true;
}

void setup () {
  configureIos();
  configureSerial();

  if (!powerOn()) {
    SerialMon.println("Fail to power on the modem. Is the SIM868 connected to the Socket 1?");
    for (;;) {}
  }

  // test modem, should return "OK"
  SerialAt.println("AT");
  delay(1000);
  while (SerialAt.available()) {
    SerialMon.write(SerialAt.read());
  }
}

void loop () {
  // Get GPS. It will be necessary a few minutes before obtaining data.
  // The important thing for this demo is not to get GPS data, but to check
  // that we can send a command to the modem and get a response.
  SerialAt.println("AT+CGNSINF");
  delay(1000);
  while (SerialAt.available()) {
    SerialMon.write(SerialAt.read());
  }

  delay(5000);
}
