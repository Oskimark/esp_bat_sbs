#ifndef _KUNDARSA_SBS_H
#define _KUNDARSA_SBS_H

#ifndef Arduino_h
#include "Arduino.h"
#endif

#include "Wire.h"

class SBS {
public:
  struct commandSet {
    const char* slaveFunction;
    uint8_t code;
    bool writeable;
    int bytes;
    const char* type;
  } commands[38];
  
  SBS(uint8_t address, int sda, int scl);
  byte sbsReadByte(uint8_t command);
  uint16_t sbsReadWord(uint8_t command);
  int16_t sbsReadInt(uint8_t command);
  void sbsReadString(char str[], uint8_t command);
  int sbsReadBlock(uint8_t command, uint8_t* data, uint8_t max_len);
  void sbsWriteWord(uint8_t command, uint16_t data);

private:
  uint8_t smbusAddress;
};

#endif
