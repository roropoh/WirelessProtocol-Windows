#ifndef VARS_H
#define VARS_H

#define NUL   	0x00
#define EOT   	0x04
#define ENQ   	0x05
#define ACK   	0x06
#define DC1   	0x11
#define DC2   	0x12
#define NAK   	0x15
#define SYN   	0x16


#define PACKET_BYTES_CTL	2
#define PACKET_BYTES_DATA	1020
#define PACKET_BYTES_CRC	2
#define PACKET_BYTES_TOTAL	PACKET_BYTES_CTL + PACKET_BYTES_DATA + PACKET_BYTES_CRC


#endif