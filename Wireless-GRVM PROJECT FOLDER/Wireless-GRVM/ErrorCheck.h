#ifndef ERRORCHECK_H
#define ERRORCHECK_H

#define POLY 0x8408
#define GENERATE_CRC_TEST_SIZE	1020
#define ERROR_CHECK_TEST_SIZE	1022


unsigned short crc16(char, unsigned short);
void GenerateCRC(char*, char*);
BOOL ErrorCheck(char*);


#endif
