#include <stdbool.h>
#include <stdint.h>
#include "utils.h"

void ByteToHex(uint8_t byte, uint8_t* hex) {
    const uint8_t hex_chars[] = "0123456789ABCDEF";
    hex[0]                    = hex_chars[(byte >> 4) & 0x0F];
    hex[1]                    = hex_chars[byte & 0x0F];
}

bool AsciiToNibble(uint8_t hex, uint8_t *out) {
    if(hex >= '0' && hex <= '9') {
        *out = (hex - '0');
        return true;
    }
    if(hex >= 'A' && hex <= 'F') {
        *out = (hex - 'A' + 10);
        return true;
    }
    if(hex >= 'a' && hex <= 'f') {
        *out = (hex - 'a' + 10);
        return true;
    }
    return false;
}


bool AsciiToByte(uint8_t msb_ascii, uint8_t lsb_ascii, uint8_t *out) {
    uint8_t msb, lsb;

    if(!AsciiToNibble(msb_ascii, &msb))
        return false;

    if(!AsciiToNibble(lsb_ascii, &lsb))
        return false;

    *out = (uint8_t)((msb << 4) | lsb);

    return true;
}


bool ModbusAsciiToBytes(const uint8_t *ascii_response, uint16_t ascii_len, uint16_t max_byte_len, uint8_t *byte_response, uint16_t *out_byte_len) {
    uint16_t ascii_index = 1;
    *out_byte_len = 0;

    if(ascii_len < 9)
        return false;

    if (ascii_response[0] != ':' || ascii_response[ascii_len - 2] != '\r' || ascii_response[ascii_len - 1] != '\n')
        return false;

    while((ascii_index + 1) < (ascii_len - 2) && *out_byte_len < max_byte_len) { // remove '\r' and '\n'
        uint8_t byte;
        if(!AsciiToByte(ascii_response[ascii_index], ascii_response[ascii_index+1], &byte))
            return false;

        byte_response[(*out_byte_len)++] = byte;
        ascii_index += 2;
    }

    return true;
}

int16_t BytesToDouble(uint8_t msb, uint8_t lsb) {
    return (int16_t)(((uint16_t)msb << 8) | lsb);
}

uint8_t LRC(uint8_t* data, uint16_t length) {
    uint8_t nLRC = 0;

    for (uint32_t i = 0; i < length; i++) nLRC += *data++;

    return (uint8_t) (-nLRC);
}

bool validateLRC(uint8_t *response, uint16_t len) {
    uint8_t calc_lrc = LRC(response, len - 1);
    uint8_t res_lrc = response[len - 1];

    return calc_lrc == res_lrc;
}

