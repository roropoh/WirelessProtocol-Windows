/*-----------------------------------------------------------------------------
--  SOURCE FILE:    Session.cpp
--
--  PROGRAM:        Wireless Protocol (GRVM)
--
--  FUNCTIONS:      BOOL SetupPort(LPTSTR)
--                  BOOL ConfPort(HWND*, LPTSTR)
--  
--
--  DATE:           Nov. 16th, 2013
--
--  REVISIONS:      ...
--
--  DESIGNER:       
--
--  PROGRAMMER:     
--
--  NOTES:
--		This layer will manage the connection between the wireless device and
--		the computer via Serial Port.
--		
--		
-----------------------------------------------------------------------------*/

#include "Session.h"

/*-----------------------------------------------------------------------------
-	FUNCTION:	SetupPort
-
-	DATE:		September 20th, 2013
-
-	REVISIONS:	2013/11/24 - Vincent - Trying to do Overlapped I/O... imminent regret
-
-	DESIGNER:	Vincent Lau
-
-	PROGRAMMER:	Vincent Lau
-
-	INTERFACE:	BOOL SetupPort (LPTSTR lpszPortName)
-
-	PARAMETERS:	LPTSTR lpszPortName - Name of the port that the device is 
-									connected to that we want to set up.
-
-	RETURNS:	TRUE	- if the port was found and opened
-				FALSE	- if an error occured while finding/opening the port
-
-	NOTES:	Closing the handle ensure that the given handle does not stay in use
-			While trying to grab a new one, which will generate an 
-			INVALID_HANDLE_VALUE for hComm.
-			Will generate a MsgBox if the port doesn't exist or other errors 
-			like if a handle was not closed before opening again.
-
-----------------------------------------------------------------------------*/
BOOL SetupPort (LPTSTR lpszPortName)
{
    CloseHandle(hComm); // Ensures that the port can be opened more than once
	if ((hComm = CreateFile (lpszPortName, GENERIC_READ | GENERIC_WRITE, 0,
		NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, NULL)) == INVALID_HANDLE_VALUE)
	{
		// Failed to find COM port
		MessageBox (NULL, TEXT("Error opening COM Port"), lpszPortName, MB_OK);
		return FALSE;
	}
	return TRUE; // success!
}


/*-----------------------------------------------------------------------------
-	FUNCTION:	ConfPort
-
-	DATE:		September 20th, 2013
-
-	REVISIONS:	VincentLau - Edited parameters to take in an opened port name - 2013/11/15
-
-	DESIGNER:	Vincent Lau
-
-	PROGRAMMER:	Vincent Lau
-
-	INTERFACE:	bool ConfigurePort (HWND* lphwnd, LPTSTR* lpszOpenedPort)
-
-	PARAMETERS:	HWND* lphwnd - Pointer to the main window's handle
-				LPTSTR lpszOpenedPort - Name of the serial port that is connected
-
-	RETURNS:	TRUE	- if the configuration was successful
-				FALSE	- if a configuration error occured or the dialog was canceled
-
-	NOTES: Brings up the Win32 COMMCONFIG dialog for the user to adjust any 
-			settings for communication. Will also set the state of the comm
-			device and get+set the default timeouts for the specified port.
-
-----------------------------------------------------------------------------*/
BOOL ConfPort (HWND* lphwnd, LPTSTR lpszOpenedPort)
{
	COMMCONFIG	cc;
	COMMTIMEOUTS comTimeout;

    cc.dwSize = sizeof (COMMCONFIG);
	cc.wVersion = 0x100;
    
	// Get defaults
	GetCommConfig (hComm, &cc, &cc.dwSize);
	// Allow user to change settings from this config dialog
	if (!CommConfigDialog (lpszOpenedPort, *lphwnd, &cc))
		return FALSE;
	PurgeComm(hComm, PURGE_RXCLEAR | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_TXABORT);

    return TRUE;
}
