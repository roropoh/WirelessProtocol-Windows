/*------------------------------------------------------------------------------------------------------------------
-- SOURCE FILE: ErrorCheck.cpp
--
-- PROGRAM: Wireless-MRGV
--
-- FUNCTIONS:
-- unsigned short CRCCCITT(unsigned char *data, size_t length, unsigned short seed, unsigned short final)
-- BOOL ErrorCheck(char pkt[ERROR_CHECK_TEST_SIZE])
-- char* GenerateCRC(char pkt[GENERATE_CRC_TEST_SIZE], char generatedCRC[2])
-- 
-- DATE: November 18, 2013
--
-- REVISIONS:  
-- 
--
-- DESIGNER: Robin Hsieh
--
-- PROGRAMMER: Robin Hsieh
--
-- NOTES:
-- Using the CRC-CCITT code, taken from http://automationwiki.com/index.php?title=CRC-16-CCITT.
-- 
--
----------------------------------------------------------------------------------------------------------------------*/
#include "Includes.h"
#include "resource.h"
#include "Main.h"
#include "ErrorCheck.h"



/**************************************************************************
//
// crc16.c - generate a ccitt 16 bit cyclic redundancy check (crc)
//
//      The code in this module generates the crc for a block of data.
//
//                                16  12  5
// The CCITT CRC 16 polynomial is X + X + X + 1.
// In binary, this is the bit pattern 1 0001 0000 0010 0001, and in hex it
//  is 0x11021.
// A 17 bit register is simulated by testing the MSB before shifting
//  the data, which affords us the luxury of specifiy the polynomial as a
//  16 bit value, 0x1021.
// Due to the way in which we process the CRC, the bits of the polynomial
//  are stored in reverse order. This makes the polynomial 0x8408.
// note: when the crc is included in the message, the valid crc is:
//      0xF0B8, before the compliment and byte swap,
//      0x0F47, after compliment, before the byte swap,
//      0x470F, after the compliment and the byte swap.
//
// crc16() - generate a 16 bit crc
//
//
// PURPOSE
//      This routine generates the 16 bit remainder of a block of
//      data using the ccitt polynomial generator.
//
// CALLING SEQUENCE
//      crc = crc16(data, len);
//
// PARAMETERS
//      data    <-- address of start of data block
//      len     <-- length of data block
//
// RETURNED VALUE
//      crc16 value. data is calcuated using the 16 bit ccitt polynomial.
//
// NOTES
//      The CRC is preset to all 1's to detect errors involving a loss
//        of leading zero's.
//      The CRC (a 16 bit value) is generated in LSB MSB order.
//      Two ways to verify the integrity of a received message
//        or block of data:
//        1) Calculate the crc on the data, and compare it to the crc
//           calculated previously. The location of the saved crc must be
//           known.
/         2) Append the calculated crc to the end of the data. Now calculate
//           the crc of the data and its crc. If the new crc equals the
//           value in "crc_ok", the data is valid.
//
// PSEUDO CODE:
//      initialize crc (-1)
//      DO WHILE count NE zero
//        DO FOR each bit in the data byte, from LSB to MSB
//          IF (LSB of crc) EOR (LSB of data)
//            crc := (crc / 2) EOR polynomial
//          ELSE
//            crc := (crc / 2)
//          FI
//        OD
//      OD
//      1's compliment and swap bytes in crc
//      RETURN crc
//
**************************************************************************/
unsigned short crc16(char *data_p, unsigned short length)
{
       unsigned char i;
       unsigned int data;
       unsigned int crc;
        
       crc = 0xffff;
        
       if (length == 0)
              return (~crc);
        
       do {
              for (i = 0, data = (unsigned int)0xff & *data_p++; i < 8; i++, data >>= 1) {
                    if ((crc & 0x0001) ^ (data & 0x0001))
                           crc = (crc >> 1) ^ POLY;
                    else
                           crc >>= 1;
              }
       } while (--length);
        
       crc = ~crc;
        
       data = crc;
       crc = (crc << 8) | (data >> 8 & 0xFF);
        
       return (crc);
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: GenerateCRC
--
-- DATE: November 27, 2013
--
-- REVISIONS: 
-- November 28, 2013 - Robin Hsieh: Modified CRC16 being used.
--
-- DESIGNER: Robin Hsieh
--
-- PROGRAMMER: Robin Hsieh
--
-- INTERFACE: char* GenerateCRC(char pkt[GENERATE_CRC_TEST_SIZE], char generatedCRC[PACKET_BYTES_CRC])
--				char pkt[GENERATE_CRC_TEST_SIZE]:			Packet of data that needs to be error checked using the CRC16 algorithm.
--				char generatedCRC[PACKET_BYTES_CRC]:		The two crc bytes generated.
--
-- RETURNS: Returns the 2 CRC characters.
--
-- NOTES:
-- This function generates the 2 CRC characters so it can be appended to the end of every packet being sent.
------------------------------------------------------------------------------------------------------------------*/
void GenerateCRC(char pkt[GENERATE_CRC_TEST_SIZE], char generatedCRC[PACKET_BYTES_CRC]){ // GENERATE_CRC_TEST_SIZE = 1020
	unsigned short the_crc;

	the_crc = crc16(pkt, PACKET_BYTES_DATA);
	generatedCRC[0] = (unsigned char)((the_crc >> 8) & 0xff);
	generatedCRC[1] = (unsigned char)(the_crc & 0xff);

}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: ErrorCheck
--
-- DATE: November 18, 2013
--
-- REVISIONS: 
--
-- DESIGNER: Robin Hsieh
--
-- PROGRAMMER: Robin Hsieh
--
-- INTERFACE: BOOL ErrorCheck(char pkt[ERROR_CHECK_TEST_SIZE])
--				char pkt[1020]:					Packet of data that needs to be error checked using the CRC16 algorithm
--				unsigned short the_crc:			Remainder for the crc
--				unsigned short checking_crc:	Used to check the crc
--
-- RETURNS: Returns the true if error check passes.
--
-- NOTES:
-- This function does the error checking on the packets received.
------------------------------------------------------------------------------------------------------------------*/
BOOL ErrorCheck(char pkt[PACKET_BYTES_DATA + PACKET_BYTES_CRC]){ // ERROR_CHECK_TEST_SIZE = 1022

	unsigned short the_crc, checking_crc;
	unsigned short first_crc, second_crc;

	//first_crc = pkt [10];
	//second_crc = pkt [11];

	//checking_crc = (unsigned char)((first_crc << 8) + second_crc);
	
	the_crc = crc16(pkt, PACKET_BYTES_DATA + PACKET_BYTES_CRC);
	//the_crc = CRCCCITT(pkt, 12, 0xffff, checking_crc);
	//printf("Final CRC value is 0x%04X\n", the_crc);
	
	// If the remainder of "the_crc" should be 0 if everything is correct.
	if(the_crc == crc_ok)
	{
		printf("TRUE\t");
		return TRUE;
	}
	MessageBox(NULL, TEXT("ErrorCheck Fail"), NULL, NULL);
	return FALSE;
}