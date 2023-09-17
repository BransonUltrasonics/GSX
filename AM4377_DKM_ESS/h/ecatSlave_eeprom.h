/* 
 * ecatSlave_eeprom.h
 * 
*/

#include <ecat_def.h>
unsigned int ecatSlave_eeprom_size=2048;

#ifdef POWER_CONTROLLER_BUILD
const unsigned char ecatSlave_eeprom[]= {
0x80,0x0c,0xe0,0x88,0xe8,0x03,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x8b,0x00,0x9d,0x05,0x00,0x00,
0x03,0x00,0x49,0x54,0x11,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x10,0x80,0x00,0x00,0x14,0x80,0x00,0x00,0x10,
0x80,0x00,0x00,0x14,0x80,0x00,0x0e,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x0f,0x00,0x01,0x00,0x0a,0x00,
0x98,0x00,0x1f,0x09,0x54,0x49,0x45,0x53,0x43,0x2d,
0x30,0x30,0x32,0x08,0x54,0x49,0x20,0x53,0x6c,0x61,
0x76,0x65,0x25,0x54,0x49,0x20,0x50,0x52,0x55,0x2d,
0x49,0x43,0x53,0x53,0x20,0x45,0x74,0x68,0x65,0x72,
0x43,0x41,0x54,0x20,0x73,0x6c,0x61,0x76,0x65,0x20,
0x43,0x6f,0x6e,0x74,0x72,0x6f,0x6c,0x6c,0x65,0x72,
0x1d,0x41,0x4d,0x34,0x33,0x37,0x37,0x2f,0x41,0x4d,
0x34,0x33,0x37,0x39,0x20,0x28,0x50,0x52,0x55,0x2d,
0x49,0x43,0x53,0x53,0x20,0x76,0x32,0x2e,0x30,0x29,
0x08,0x53,0x79,0x6e,0x63,0x68,0x72,0x6f,0x6e,0x02,
0x44,0x43,0x09,0x44,0x49,0x20,0x49,0x6e,0x70,0x75,
0x74,0x73,0x08,0x53,0x77,0x69,0x74,0x63,0x68,0x20,
0x31,0x08,0x53,0x77,0x69,0x74,0x63,0x68,0x20,0x32,
0x08,0x53,0x77,0x69,0x74,0x63,0x68,0x20,0x33,0x08,
0x53,0x77,0x69,0x74,0x63,0x68,0x20,0x34,0x08,0x53,
0x77,0x69,0x74,0x63,0x68,0x20,0x35,0x08,0x53,0x77,
0x69,0x74,0x63,0x68,0x20,0x36,0x08,0x53,0x77,0x69,
0x74,0x63,0x68,0x20,0x37,0x08,0x53,0x77,0x69,0x74,
0x63,0x68,0x20,0x38,0x0f,0x4d,0x6f,0x74,0x6f,0x72,
0x20,0x41,0x49,0x20,0x49,0x6e,0x70,0x75,0x74,0x73,
0x06,0x49,0x6e,0x66,0x6f,0x20,0x31,0x06,0x49,0x6e,
0x66,0x6f,0x20,0x32,0x0a,0x44,0x4f,0x20,0x4f,0x75,
0x74,0x70,0x75,0x74,0x73,0x05,0x4c,0x45,0x44,0x20,
0x31,0x05,0x4c,0x45,0x44,0x20,0x32,0x05,0x4c,0x45,
0x44,0x20,0x33,0x05,0x4c,0x45,0x44,0x20,0x34,0x05,
0x4c,0x45,0x44,0x20,0x35,0x05,0x4c,0x45,0x44,0x20,
0x36,0x05,0x4c,0x45,0x44,0x20,0x37,0x05,0x4c,0x45,
0x44,0x20,0x38,0x0d,0x4d,0x6f,0x74,0x6f,0x72,0x20,
0x4f,0x75,0x74,0x70,0x75,0x74,0x73,0x05,0x43,0x6f,
0x75,0x6e,0x74,0x07,0x43,0x6f,0x6d,0x6d,0x61,0x6e,
0x64,0x04,0x44,0x61,0x74,0x61,0x1e,0x00,0x10,0x00,
0x02,0x00,0x01,0x04,0x01,0x23,0x01,0x01,0x00,0x00,
0x00,0x04,0x00,0x00,0x03,0x00,0x11,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x28,0x00,0x02,0x00,0x01,0x02,0x03,0xff,
0x29,0x00,0x10,0x00,0x00,0x10,0x80,0x00,0x26,0x00,
0x01,0x01,0x00,0x14,0x80,0x00,0x22,0x00,0x01,0x02,
0x00,0x18,0x05,0x00,0x64,0x00,0x01,0x03,0x00,0x1c,
0x07,0x00,0x20,0x00,0x01,0x04,0x32,0x00,0x30,0x00,
0x00,0x1a,0x08,0x03,0x00,0x07,0x11,0x00,0x00,0x60,
0x01,0x08,0x01,0x01,0x00,0x00,0x00,0x60,0x02,0x09,
0x01,0x01,0x00,0x00,0x00,0x60,0x03,0x0a,0x01,0x01,
0x00,0x00,0x00,0x60,0x04,0x0b,0x01,0x01,0x00,0x00,
0x00,0x60,0x05,0x0c,0x01,0x01,0x00,0x00,0x00,0x60,
0x06,0x0d,0x01,0x01,0x00,0x00,0x00,0x60,0x07,0x0e,
0x01,0x01,0x00,0x00,0x00,0x60,0x08,0x0f,0x01,0x01,
0x00,0x00,0x03,0x1a,0x02,0x03,0x00,0x10,0x11,0x00,
0x30,0x60,0x01,0x11,0x04,0x20,0x00,0x00,0x30,0x60,
0x02,0x12,0x03,0x10,0x00,0x00,0x33,0x00,0x34,0x00,
0x01,0x16,0x08,0x02,0x00,0x13,0x11,0x00,0x10,0x70,
0x01,0x14,0x01,0x01,0x00,0x00,0x10,0x70,0x02,0x15,
0x01,0x01,0x00,0x00,0x10,0x70,0x03,0x16,0x01,0x01,
0x00,0x00,0x10,0x70,0x04,0x17,0x01,0x01,0x00,0x00,
0x10,0x70,0x05,0x18,0x01,0x01,0x00,0x00,0x10,0x70,
0x06,0x19,0x01,0x01,0x00,0x00,0x10,0x70,0x07,0x1a,
0x01,0x01,0x00,0x00,0x10,0x70,0x08,0x1b,0x01,0x01,
0x00,0x00,0x02,0x16,0x03,0x02,0x00,0x1c,0x11,0x00,
0x20,0x70,0x01,0x1d,0x05,0x08,0x00,0x00,0x20,0x70,
0x02,0x1e,0x05,0x08,0x00,0x00,0x20,0x70,0x03,0x1f,
0x03,0x10,0x00,0x00,0x3c,0x00,0x18,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x01,0x00,0x05,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0x01,0x00,
0x06,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff };
#else //POWER CONTROLLER 
#ifdef ACTUATION_CONTROLLER_BUILD
const unsigned char ecatSlave_eeprom[]= {
0x80,0x0c,0xe0,0x88,0xe8,0x03,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x8b,0x00,0x9d,0x05,0x00,0x00,
0x04,0x00,0x49,0x54,0x11,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x10,0x80,0x00,0x00,0x14,0x80,0x00,0x00,0x10,
0x80,0x00,0x00,0x14,0x80,0x00,0x0e,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x0f,0x00,0x01,0x00,0x0a,0x00,
0x98,0x00,0x1f,0x09,0x54,0x49,0x45,0x53,0x43,0x2d,
0x30,0x30,0x32,0x08,0x54,0x49,0x20,0x53,0x6c,0x61,
0x76,0x65,0x25,0x54,0x49,0x20,0x50,0x52,0x55,0x2d,
0x49,0x43,0x53,0x53,0x20,0x45,0x74,0x68,0x65,0x72,
0x43,0x41,0x54,0x20,0x73,0x6c,0x61,0x76,0x65,0x20,
0x43,0x6f,0x6e,0x74,0x72,0x6f,0x6c,0x6c,0x65,0x72,
0x1d,0x41,0x4d,0x34,0x33,0x37,0x37,0x2f,0x41,0x4d,
0x34,0x33,0x37,0x39,0x20,0x28,0x50,0x52,0x55,0x2d,
0x49,0x43,0x53,0x53,0x20,0x76,0x32,0x2e,0x30,0x29,
0x08,0x53,0x79,0x6e,0x63,0x68,0x72,0x6f,0x6e,0x02,
0x44,0x43,0x09,0x44,0x49,0x20,0x49,0x6e,0x70,0x75,
0x74,0x73,0x08,0x53,0x77,0x69,0x74,0x63,0x68,0x20,
0x31,0x08,0x53,0x77,0x69,0x74,0x63,0x68,0x20,0x32,
0x08,0x53,0x77,0x69,0x74,0x63,0x68,0x20,0x33,0x08,
0x53,0x77,0x69,0x74,0x63,0x68,0x20,0x34,0x08,0x53,
0x77,0x69,0x74,0x63,0x68,0x20,0x35,0x08,0x53,0x77,
0x69,0x74,0x63,0x68,0x20,0x36,0x08,0x53,0x77,0x69,
0x74,0x63,0x68,0x20,0x37,0x08,0x53,0x77,0x69,0x74,
0x63,0x68,0x20,0x38,0x0f,0x4d,0x6f,0x74,0x6f,0x72,
0x20,0x41,0x49,0x20,0x49,0x6e,0x70,0x75,0x74,0x73,
0x06,0x49,0x6e,0x66,0x6f,0x20,0x31,0x06,0x49,0x6e,
0x66,0x6f,0x20,0x32,0x0a,0x44,0x4f,0x20,0x4f,0x75,
0x74,0x70,0x75,0x74,0x73,0x05,0x4c,0x45,0x44,0x20,
0x31,0x05,0x4c,0x45,0x44,0x20,0x32,0x05,0x4c,0x45,
0x44,0x20,0x33,0x05,0x4c,0x45,0x44,0x20,0x34,0x05,
0x4c,0x45,0x44,0x20,0x35,0x05,0x4c,0x45,0x44,0x20,
0x36,0x05,0x4c,0x45,0x44,0x20,0x37,0x05,0x4c,0x45,
0x44,0x20,0x38,0x0d,0x4d,0x6f,0x74,0x6f,0x72,0x20,
0x4f,0x75,0x74,0x70,0x75,0x74,0x73,0x05,0x43,0x6f,
0x75,0x6e,0x74,0x07,0x43,0x6f,0x6d,0x6d,0x61,0x6e,
0x64,0x04,0x44,0x61,0x74,0x61,0x1e,0x00,0x10,0x00,
0x02,0x00,0x01,0x04,0x01,0x23,0x01,0x01,0x00,0x00,
0x00,0x04,0x00,0x00,0x03,0x00,0x11,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x28,0x00,0x02,0x00,0x01,0x02,0x03,0xff,
0x29,0x00,0x10,0x00,0x00,0x10,0x80,0x00,0x26,0x00,
0x01,0x01,0x00,0x14,0x80,0x00,0x22,0x00,0x01,0x02,
0x00,0x18,0x05,0x00,0x64,0x00,0x01,0x03,0x00,0x1c,
0x07,0x00,0x20,0x00,0x01,0x04,0x32,0x00,0x30,0x00,
0x00,0x1a,0x08,0x03,0x00,0x07,0x11,0x00,0x00,0x60,
0x01,0x08,0x01,0x01,0x00,0x00,0x00,0x60,0x02,0x09,
0x01,0x01,0x00,0x00,0x00,0x60,0x03,0x0a,0x01,0x01,
0x00,0x00,0x00,0x60,0x04,0x0b,0x01,0x01,0x00,0x00,
0x00,0x60,0x05,0x0c,0x01,0x01,0x00,0x00,0x00,0x60,
0x06,0x0d,0x01,0x01,0x00,0x00,0x00,0x60,0x07,0x0e,
0x01,0x01,0x00,0x00,0x00,0x60,0x08,0x0f,0x01,0x01,
0x00,0x00,0x03,0x1a,0x02,0x03,0x00,0x10,0x11,0x00,
0x30,0x60,0x01,0x11,0x04,0x20,0x00,0x00,0x30,0x60,
0x02,0x12,0x03,0x10,0x00,0x00,0x33,0x00,0x34,0x00,
0x01,0x16,0x08,0x02,0x00,0x13,0x11,0x00,0x10,0x70,
0x01,0x14,0x01,0x01,0x00,0x00,0x10,0x70,0x02,0x15,
0x01,0x01,0x00,0x00,0x10,0x70,0x03,0x16,0x01,0x01,
0x00,0x00,0x10,0x70,0x04,0x17,0x01,0x01,0x00,0x00,
0x10,0x70,0x05,0x18,0x01,0x01,0x00,0x00,0x10,0x70,
0x06,0x19,0x01,0x01,0x00,0x00,0x10,0x70,0x07,0x1a,
0x01,0x01,0x00,0x00,0x10,0x70,0x08,0x1b,0x01,0x01,
0x00,0x00,0x02,0x16,0x03,0x02,0x00,0x1c,0x11,0x00,
0x20,0x70,0x01,0x1d,0x05,0x08,0x00,0x00,0x20,0x70,
0x02,0x1e,0x05,0x08,0x00,0x00,0x20,0x70,0x03,0x1f,
0x03,0x10,0x00,0x00,0x3c,0x00,0x18,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x01,0x00,0x05,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0x01,0x00,
0x06,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff };
#else
const unsigned char ecatSlave_eeprom[]= {
0x80,0x0c,0xe0,0x88,0xe8,0x03,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x8b,0x00,0x9d,0x05,0x00,0x00,
0x02,0x00,0x49,0x54,0x11,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x10,0x80,0x00,0x00,0x14,0x80,0x00,0x00,0x10,
0x80,0x00,0x00,0x14,0x80,0x00,0x0e,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x0f,0x00,0x01,0x00,0x0a,0x00,
0x98,0x00,0x1f,0x09,0x54,0x49,0x45,0x53,0x43,0x2d,
0x30,0x30,0x32,0x08,0x54,0x49,0x20,0x53,0x6c,0x61,
0x76,0x65,0x25,0x54,0x49,0x20,0x50,0x52,0x55,0x2d,
0x49,0x43,0x53,0x53,0x20,0x45,0x74,0x68,0x65,0x72,
0x43,0x41,0x54,0x20,0x73,0x6c,0x61,0x76,0x65,0x20,
0x43,0x6f,0x6e,0x74,0x72,0x6f,0x6c,0x6c,0x65,0x72,
0x1d,0x41,0x4d,0x34,0x33,0x37,0x37,0x2f,0x41,0x4d,
0x34,0x33,0x37,0x39,0x20,0x28,0x50,0x52,0x55,0x2d,
0x49,0x43,0x53,0x53,0x20,0x76,0x32,0x2e,0x30,0x29,
0x08,0x53,0x79,0x6e,0x63,0x68,0x72,0x6f,0x6e,0x02,
0x44,0x43,0x09,0x44,0x49,0x20,0x49,0x6e,0x70,0x75,
0x74,0x73,0x08,0x53,0x77,0x69,0x74,0x63,0x68,0x20,
0x31,0x08,0x53,0x77,0x69,0x74,0x63,0x68,0x20,0x32,
0x08,0x53,0x77,0x69,0x74,0x63,0x68,0x20,0x33,0x08,
0x53,0x77,0x69,0x74,0x63,0x68,0x20,0x34,0x08,0x53,
0x77,0x69,0x74,0x63,0x68,0x20,0x35,0x08,0x53,0x77,
0x69,0x74,0x63,0x68,0x20,0x36,0x08,0x53,0x77,0x69,
0x74,0x63,0x68,0x20,0x37,0x08,0x53,0x77,0x69,0x74,
0x63,0x68,0x20,0x38,0x0f,0x4d,0x6f,0x74,0x6f,0x72,
0x20,0x41,0x49,0x20,0x49,0x6e,0x70,0x75,0x74,0x73,
0x06,0x49,0x6e,0x66,0x6f,0x20,0x31,0x06,0x49,0x6e,
0x66,0x6f,0x20,0x32,0x0a,0x44,0x4f,0x20,0x4f,0x75,
0x74,0x70,0x75,0x74,0x73,0x05,0x4c,0x45,0x44,0x20,
0x31,0x05,0x4c,0x45,0x44,0x20,0x32,0x05,0x4c,0x45,
0x44,0x20,0x33,0x05,0x4c,0x45,0x44,0x20,0x34,0x05,
0x4c,0x45,0x44,0x20,0x35,0x05,0x4c,0x45,0x44,0x20,
0x36,0x05,0x4c,0x45,0x44,0x20,0x37,0x05,0x4c,0x45,
0x44,0x20,0x38,0x0d,0x4d,0x6f,0x74,0x6f,0x72,0x20,
0x4f,0x75,0x74,0x70,0x75,0x74,0x73,0x05,0x43,0x6f,
0x75,0x6e,0x74,0x07,0x43,0x6f,0x6d,0x6d,0x61,0x6e,
0x64,0x04,0x44,0x61,0x74,0x61,0x1e,0x00,0x10,0x00,
0x02,0x00,0x01,0x04,0x01,0x23,0x01,0x01,0x00,0x00,
0x00,0x04,0x00,0x00,0x03,0x00,0x11,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x28,0x00,0x02,0x00,0x01,0x02,0x03,0xff,
0x29,0x00,0x10,0x00,0x00,0x10,0x80,0x00,0x26,0x00,
0x01,0x01,0x00,0x14,0x80,0x00,0x22,0x00,0x01,0x02,
0x00,0x18,0x05,0x00,0x64,0x00,0x01,0x03,0x00,0x1c,
0x07,0x00,0x20,0x00,0x01,0x04,0x32,0x00,0x30,0x00,
0x00,0x1a,0x08,0x03,0x00,0x07,0x11,0x00,0x00,0x60,
0x01,0x08,0x01,0x01,0x00,0x00,0x00,0x60,0x02,0x09,
0x01,0x01,0x00,0x00,0x00,0x60,0x03,0x0a,0x01,0x01,
0x00,0x00,0x00,0x60,0x04,0x0b,0x01,0x01,0x00,0x00,
0x00,0x60,0x05,0x0c,0x01,0x01,0x00,0x00,0x00,0x60,
0x06,0x0d,0x01,0x01,0x00,0x00,0x00,0x60,0x07,0x0e,
0x01,0x01,0x00,0x00,0x00,0x60,0x08,0x0f,0x01,0x01,
0x00,0x00,0x03,0x1a,0x02,0x03,0x00,0x10,0x11,0x00,
0x30,0x60,0x01,0x11,0x04,0x20,0x00,0x00,0x30,0x60,
0x02,0x12,0x03,0x10,0x00,0x00,0x33,0x00,0x34,0x00,
0x01,0x16,0x08,0x02,0x00,0x13,0x11,0x00,0x10,0x70,
0x01,0x14,0x01,0x01,0x00,0x00,0x10,0x70,0x02,0x15,
0x01,0x01,0x00,0x00,0x10,0x70,0x03,0x16,0x01,0x01,
0x00,0x00,0x10,0x70,0x04,0x17,0x01,0x01,0x00,0x00,
0x10,0x70,0x05,0x18,0x01,0x01,0x00,0x00,0x10,0x70,
0x06,0x19,0x01,0x01,0x00,0x00,0x10,0x70,0x07,0x1a,
0x01,0x01,0x00,0x00,0x10,0x70,0x08,0x1b,0x01,0x01,
0x00,0x00,0x02,0x16,0x03,0x02,0x00,0x1c,0x11,0x00,
0x20,0x70,0x01,0x1d,0x05,0x08,0x00,0x00,0x20,0x70,
0x02,0x1e,0x05,0x08,0x00,0x00,0x20,0x70,0x03,0x1f,
0x03,0x10,0x00,0x00,0x3c,0x00,0x18,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x01,0x00,0x05,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0x01,0x00,
0x06,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff };
#endif //ACTUATION_CONTROLLER
#endif //POWER_CONTROLLER
