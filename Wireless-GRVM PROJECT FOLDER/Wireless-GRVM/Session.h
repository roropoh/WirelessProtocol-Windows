#ifndef SESSION_H
#define SESSION_H

#include "Includes.h"

// Function Prototypes
BOOL SetupPort (LPTSTR);
BOOL ConfPort (HWND*, LPTSTR);

// Vars
extern HANDLE hComm;

#endif