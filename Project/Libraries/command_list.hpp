#include "hwlib.hpp"

#ifndef command_list
#define command_list

// DATA AND COMMAND MODE IDS //
#define HT1632C_ID_READ 0b110 // ID = 110, read RAM // 
#define HT1632C_ID_WRITE 0b101 // ID = 101, write RAM //
#define HT1632C_ID_COMMAND 0b100 // ID = 100, use commands //
#define HT1632C_ID_LEN 3 // IDS are 3 bits long
// 

// COMMAND LIST //
#define HT1632C_CMD_SYSDIS 0x00 // COMMAND = 0000-0000-X, disables system //
#define HT1632C_CMD_SYSEN 0x01 // COMMAND = 0000-0001-X, enables system //
#define HT1632C_CMD_LEDOFF 0x02 // COMMAND = 0000-0010-X, disables LED //
#define HT1632C_CMD_LEDON 0x03 // COMMAND = 0000-0011-X, enables LED //
#define HT1632C_CMD_BLINKOFF 0x08 // COMMAND = 0000-1000-X, disables BLINK //
#define HT1632C_CMD_BLINKON 0x09 // COMMAND = 0000-1001-X, enables BLINK //
#define HT1632C_CMD_INT_RC 0x18 // COMMAND = 0001-10XX-X, Set master mode and clock source //
#define HT1632C_CMD_COMS00 0x20 // COMMAND = 0010-abXX, N-MOS open drain output and 8 COM OPTION //
#define HT1632C_CMD_COMS01 0x24 // COMMAND = 0010-abXX, N-MOS open drain output and 16 COM OPTION //
#define HT1632C_CMD_COMS10 0x28 // COMMAND = 0010-abXX, P-MOS open drain output and 8 COM OPTION //
#define HT1632C_CMD_COMS11 0x2C // COMMAND = 0010-abXX, P-MOS open drain output and 16 COM OPTION //

// LENGTHS //
#define HT1632C_CMD_LEN 8 // Commands have a length of 8 bits, excluding the X(don't care). //
#define HT1632C_ADDRESS_LEN 7 // Memory Adresses have a length of 7 bits //
#define HT1632C_DATA_LEN 4 // DATA has a length of 4 bits //
#define HT1632C_LEN 24 // length of the HT1632C //
#define HT1632C_WIDTH 16 // width of the HT1632C //



#endif