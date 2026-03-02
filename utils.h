#ifndef UTILS_H
#define UTILS_H

#include <stdbool.h>
#include <stdint.h>

void ByteToHex(uint8_t byte, uint8_t *hex);
bool AsciiToNibble(uint8_t hex, uint8_t *out);
bool AsciiToByte(const uint8_t msb_ascii, const uint8_t lsb_ascii,
                 uint8_t *out);
bool ModbusAsciiToBytes(const uint8_t *ascii_response, uint16_t ascii_len,
                        uint16_t max_byte_len, uint8_t *byte_response,
                        uint16_t *out_byte_len);
int16_t BytesToDouble(uint8_t msb, uint8_t lsb);
uint8_t LRC(uint8_t *data, uint16_t length);
bool validateLRC(uint8_t *response, uint16_t len);

#endif
