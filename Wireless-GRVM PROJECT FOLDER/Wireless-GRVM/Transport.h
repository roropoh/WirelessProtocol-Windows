#ifndef _TRANSPORTS_H_
#define _TRANSPORTS_H_
#include "Includes.h"
#include "Vars.h"
#include "Packet.h"
#include "Physical.h"

// Function prototypes
DWORD WINAPI TransmitThread(LPVOID);
DWORD WINAPI ReceiveThread	(LPVOID);

// Vars
extern HANDLE hWaitForLineSemaphore;
extern HANDLE hACKWaitSemaphore;
extern HANDLE hFileWaitSemaphore;
extern HANDLE hComm;
extern OVERLAPPED ov;
extern BOOL bWantToRead;
extern CHAR Packet[];
#endif
