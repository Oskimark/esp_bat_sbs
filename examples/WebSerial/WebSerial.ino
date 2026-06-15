#include "../../SBS.h"
#include <Arduino.h>

#define CONSOLE_BAUD 115200
#define BATTERY_ADDRESS 0x0B
// Modify these pins if necessary for your board
#define BATTERY_SDA D2
#define BATTERY_SCL D1

SBS battery = SBS(BATTERY_ADDRESS, BATTERY_SDA, BATTERY_SCL);

void setup() {
  Serial.begin(CONSOLE_BAUD);
  // Wait for serial to initialize
  delay(100);
}

void loop() {
  if (Serial.available()) {
    char c = Serial.peek();
    if (c == '?') {
      Serial.read(); // consume '?'
      printBatteryJSON();
    } else if (c == 'U' || c == 'R' || c == 'S' || c == 'W' || c == 'M' || c == 'Q' || c == 'P') {
      String cmd = Serial.readStringUntil('\n');
      cmd.trim();
      if (cmd.startsWith("U")) {
        // Unseal format: U0414,3672
        int commaIdx = cmd.indexOf(',');
        if (commaIdx > 1) {
          uint16_t key1 = (uint16_t) strtol(cmd.substring(1, commaIdx).c_str(), NULL, 16);
          uint16_t key2 = (uint16_t) strtol(cmd.substring(commaIdx + 1).c_str(), NULL, 16);
          battery.sbsWriteWord(0x00, key1);
          delay(10);
          battery.sbsWriteWord(0x00, key2);
          Serial.println("{\"log\": \"Unseal command sent\"}");
        }
      } else if (cmd == "R") {
        battery.sbsWriteWord(0x00, 0x0028); // Lifetime reset
        Serial.println("{\"log\": \"Lifetime reset sent\"}");
      } else if (cmd == "S") {
        battery.sbsWriteWord(0x00, 0x0030); // Seal
        Serial.println("{\"log\": \"Seal command sent\"}");
      } else if (cmd.startsWith("W")) {
        // Write cycle count format: W0 (decimal)
        uint16_t cycles = (uint16_t) cmd.substring(1).toInt();
        battery.sbsWriteWord(0x17, cycles);
        Serial.println("{\"log\": \"Write cycles command sent\"}");
      } else if (cmd.startsWith("M")) {
        // Generic MAC command format: M0011
        uint16_t mac = (uint16_t) strtol(cmd.substring(1).c_str(), NULL, 16);
        battery.sbsWriteWord(0x00, mac);
        Serial.print("{\"log\": \"MAC command sent: 0x");
        Serial.print(mac, HEX);
        Serial.println("\"}");
      } else if (cmd.startsWith("Q")) {
        // Query standard SBS command: Q09
        uint8_t sbsCmd = (uint8_t) strtol(cmd.substring(1).c_str(), NULL, 16);
        uint16_t val = battery.sbsReadWord(sbsCmd);
        Serial.print("{\"cmd\":\"0x");
        if (sbsCmd < 16) Serial.print("0");
        Serial.print(sbsCmd, HEX);
        Serial.print("\", \"val\":");
        Serial.print(val);
        Serial.println("}");
      } else if (cmd.startsWith("P")) {
        // Poll MAC command: P0054
        uint16_t macCmd = (uint16_t) strtol(cmd.substring(1).c_str(), NULL, 16);
        battery.sbsWriteWord(0x00, macCmd);
        uint8_t blockData[34];
        int len = battery.sbsReadBlock(0x44, blockData, 32);
        Serial.print("{\"cmd\":\"0x");
        if (macCmd < 4096) Serial.print("0");
        if (macCmd < 256) Serial.print("0");
        if (macCmd < 16) Serial.print("0");
        Serial.print(macCmd, HEX);
        Serial.print("\", \"hexVal\":\"0x");
        for(int i = 0; i < len; i++) {
          if (blockData[i] < 16) Serial.print("0");
          Serial.print(blockData[i], HEX);
        }
        Serial.println("\"}");
      }
    } else {
      Serial.read(); // consume unknown char
    }
  }
}

void printBatteryJSON() {
  Serial.print("{");
  bool first = true;
  for(int i = 1; i < 38; i++) {
    // Skip ManufacturerData and OptionalMfgFunction5 as they are custom data blocks
    if (strcmp(battery.commands[i].type, "data") == 0) continue;
    
    if (!first) Serial.print(",");
    first = false;
    
    Serial.print("\"");
    Serial.print(battery.commands[i].slaveFunction);
    Serial.print("\":");
    
    if(strcmp(battery.commands[i].type, "Boolean") == 0) {
      Serial.print(battery.sbsReadByte(battery.commands[i].code) == 0x00 ? "false" : "true");
    } else if(strcmp(battery.commands[i].type, "string") == 0) {
      char s[33];
      s[32] = 0;
      battery.sbsReadString(s, battery.commands[i].code);
      Serial.print("\"");
      // Basic sanitization
      for (int j = 0; j < 32 && s[j] != 0; j++) {
        if (s[j] != '"' && s[j] != '\\') {
          Serial.print(s[j]);
        }
      }
      Serial.print("\"");
    } else if(strcmp(battery.commands[i].slaveFunction, "ManufactureDate") == 0) {
      uint16_t raw = battery.sbsReadWord(battery.commands[i].code);
      Serial.print("\"");
      Serial.print((raw >> 5) & 0xF); Serial.print("/");
      Serial.print(raw & 0x1F); Serial.print("/");
      Serial.print((raw >> 9) + 1980);
      Serial.print("\"");
    } else if(strcmp(battery.commands[i].type, "0.1K") == 0) {
      // Convert to Celsius and send as float
      float tempC = (battery.sbsReadWord(battery.commands[i].code) / 10.0) - 273.15;
      Serial.print(tempC);
    } else if(strcmp(battery.commands[i].type, "mA") == 0) {
      // Cast to int16_t to allow negative current (discharging)
      Serial.print((int16_t)battery.sbsReadWord(battery.commands[i].code));
    } else {
      // General numbers (mV, mAh, minutes, count, etc)
      Serial.print(battery.sbsReadWord(battery.commands[i].code));
    }
  }
  Serial.println("}");
}
