#ifndef PACKET_H
#define PACKET_H

#include "Includes.h"
#include "Vars.h"
#include "Physical.h"
#include "Main.h"
#include "Presentation.h"

#define DATA_PACKET			6000
#define CTL_PACKET			6001

// Function Prototypes
BOOL Packetize(CHAR*, int);
BOOL PacketCheck(HWND, CHAR*);
BOOL PacketCheckControl(HWND hwnd, CHAR* packet);
void GetData(CHAR*, CHAR*);

// vars
extern HANDLE hWaitForLineSemaphore;
extern HANDLE hACKWaitSemaphore;
extern HANDLE hFileWaitSemaphore;
extern HANDLE hComm;
extern INT waitForType;
extern BOOL bENQToSend;
extern BOOL bENQReceived;
extern BOOL bWantLine;
extern BOOL bWantToSendACK;
extern BOOL bHaveFileToSend;
extern LPSTR displayBuffer;

#endif