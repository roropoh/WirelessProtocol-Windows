/*-----------------------------------------------------------------------------
--  SOURCE FILE:    Transport.cpp
--
--  PROGRAM:        Wireless Protocol (GRVM)
--
--  FUNCTIONS:      void Transmit(LPSTR* lpszFileBuffer)
--                  bool PacketCheck(char[1024] packet)
--  
--
--  DATE:			Nov. 16th, 2013
--
--  REVISIONS:      ...
--
--  DESIGNER:       Vincent Lau
--
--  PROGRAMMER:     
--
--  NOTES:			
--		Provides the logic for data transfer between the computer and the 
--		wireless device (which would be transmitting to the other wireless 
--		device conneceted to the other computer). Error checking will be done
--		via open-source CRC.
-----------------------------------------------------------------------------*/

#include "Transport.h"
#include "CircularBuffer.h"
#include "Physical.h"
INT waitForType = ENQ;
INT sentPacketCounter = 0;	/* Counter to keep track of our file location as
							 as well as how many packets we've sent. */
BOOL bHaveFileToSend = FALSE;
BOOL bENQToSend	= FALSE;
BOOL bENQReceived = FALSE;
BOOL bWantLine = FALSE;
BOOL bWantToSendACK = FALSE;
BOOL bWantToResendData = FALSE;

/*-----------------------------------------------------------------------------
-	FUNCTION:	Transmit
-
-	DATE:		November 21st, 2013
-
-	REVISIONS:	
-	2013/11/25 - Vincent - Semaphore for blocking when hitting maximum packets sent
-	2013/11/26 - Vincent - Added line bidding
-   2013/11/27 - Vincent/German - Modified line bidding solve issues with simultaneous file sending
-								- Modified Packetize/sentPacketCounter call order
-								- Added basic timeout for end of 5 packet transmission
-								- Added line rebidding
-
-	DESIGNER:	Vincent Lau
-
-	PROGRAMMER:	Vincent Lau
-				German Villarreal
-
-	INTERFACE:	DWORD WINAPI TransmitThread(LPVOID param)
-		
-	RETURNS:	void, nothing.
-				
-	PARAMETERS:	LPVOID param - a dynamic char buffer that contains the whole file
-							we are trying to send over the serial port.
-
-	NOTES:	This is the function that contains the logic for transmitting data
-			and following those rules for doing so, such as the upper send limit
-			per round (5 packets per round in the (BE CREATIVE) Protocol).
-			It will stop when it has sent one packet out and wait for a response.
-			
-
-----------------------------------------------------------------------------*/
DWORD WINAPI TransmitThread(LPVOID param)
{
	
	LPSTR	file = (LPSTR)param;
	BOOL	bDoneSending = FALSE;
	DWORD	dwenqTimeout;
	//COMSTAT* cs = (COMSTAT*)malloc(sizeof(COMSTAT));
	sentPacketCounter = 0;
	bHaveFileToSend = TRUE;
	LONG_PTR bytesSent;
	int count = 1;
	int c = rand() % 100 + 1;
	Sleep(c);
	do
	{
		// Send ENQ - for getting the right-of-way to send
		bENQToSend = TRUE;
		if (bENQReceived) // if The other guy is sending something
		{
			WaitForSingleObject(hWaitForLineSemaphore, INFINITE);
		}
		
		SendControl(hComm, ENQ);// REQUEST DA LINE

		//// Wait for ACK
		waitForType = ACK;
		bWantLine = TRUE;
		//Sleep(5000);
		WaitForSingleObject(hWaitForLineSemaphore, INFINITE);
		bWantLine = FALSE;
		
			while (++sentPacketCounter % 6 != 0 && !bDoneSending) // ++ mod 6 allows sending of 5 packets, ++ mod 5 allows 4
			{	
					bDoneSending = Packetize(file, (sentPacketCounter - 1));	
					// set "what we're waiting for" to ack
					waitForType = ACK;
					 bytesSent = 0;
					 while(1){
						if(SendData(hComm, Packet)) {
							break;
					 }
					}
					 DWORD hACKWaitstuff = 0;
					while(count < 6)
					{
						
						hACKWaitstuff = WaitForSingleObject(hACKWaitSemaphore, 2500);
						if( hACKWaitstuff == WAIT_OBJECT_0){
							count = 0;
							bDoneSending = FALSE;
							bWantToResendData = FALSE;
							break;
						}
						if( hACKWaitstuff == WAIT_TIMEOUT)
						{
							PurgeComm(hComm, PURGE_RXCLEAR | PURGE_TXCLEAR);
							SendData(hComm, Packet);
						}
						bDoneSending = TRUE;
						count++;
					}
					
			}
			count = 0;
			SendControl(hComm, EOT);
			 //another semaphore to determine when to start again.
			
			Sleep(1000);
		// DONE SENDING
		bENQToSend = FALSE;
	} while(!bDoneSending); //file not done
	ExitThread(EXIT_SUCCESS);
}


/*-----------------------------------------------------------------------------
-	FUNCTION:	ReceiveThread
-
-	DATE:		November 22st, 2013
-
-	REVISIONS:	
-	2013/11/24 - Vincent - Overlapped i/o 
-	2013/11/25 - Vincent - Semaphore for releasing file sending block, when we've received 5 packets 
-							(other person hit their maximum)
-	2013/11/29 - Mat	- Fixed the check for the cs.cbInQue
-
-	DESIGNER:	Vincent Lau
-
-	PROGRAMMER:	Vincent Lau
-
-	INTERFACE:	DWORD WINAPI ReceiveThread (LPVOID lphwnd)
-		
-	RETURNS:	DWORD - a success or failure value on thread exit.
-				
-	PARAMETERS:	LPVOID lphwnd - reference to the edit window's handle (hEdit in main)
-
-	NOTES:	This is the primary thread for receiving packets from the serial
-			port. It runs forever until the program exits, but does not run its
-			read loop until the port has been configured. 
-			This is to be fully event-driven for serial ports. 
-			References were used, mainly the event-driven tty given earlier.
-
-----------------------------------------------------------------------------*/
DWORD WINAPI ReceiveThread(LPVOID lphwnd)
{
	cb mycb1;
	cb *my_cb = &mycb1;
	initBuffer(my_cb, 1024);
	CHAR dataBufferToVerifyWhatItShouldBe[1024] = {};
	DWORD nBytesRead = 0, dwEvent, dwError, dwWaitValue;
	COMSTAT* cs = (COMSTAT*)malloc(sizeof(COMSTAT));
	char* packetBuffer = (char*) malloc(1024);
	char* packetBufferControl = (char*) malloc(2);

	HANDLE m_hThreadTerm = CreateEvent(0,0,0,0);
	
	
	// Create the overlapped event. MUST CLOSE!!!
	memset(&ov,0, sizeof(ov));
	ov.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	if (ov.hEvent == NULL)
	{
		MessageBox(NULL, TEXT("Error creating Overlapped event"), NULL, MB_OK);
		ExitThread(EXIT_FAILURE);
	}
	HANDLE handletoTwoEvents[2];
	handletoTwoEvents[0] = m_hThreadTerm;
	DWORD dwWait;
	BOOL waitCommEventValue = true;

	// Set our Listening/Reading parameter for the serial port, want CHAR events
	SetCommMask(hComm, EV_RXCHAR);
	
	while(1)
	{
		waitCommEventValue = WaitCommEvent(hComm, &dwEvent, &ov);
		if(waitCommEventValue)
		{
			break;
		}
		handletoTwoEvents[1] = ov.hEvent;

		if (bWantToSendACK)
		{
			SendControl(hComm, ACK);
			bWantToSendACK = FALSE;
		}

		dwWaitValue = WaitForMultipleObjects(2, handletoTwoEvents, FALSE, INFINITE);
		ClearCommError(hComm, &dwError, cs);
		switch ( dwWaitValue )
		{
			case WAIT_OBJECT_0+1:
				do 
				{
					ClearCommError(hComm, &dwError, cs);
					ResetEvent(handletoTwoEvents[1]);
					nBytesRead = 0;
					if( (cs->cbInQue >= 1024) && ReadSerialPortData(hComm, packetBuffer, cs->cbInQue, &nBytesRead) && (nBytesRead !=0))
					{
						
						packetBuffer[1024] = '\0';
						PacketCheck(*(HWND*)lphwnd, packetBuffer);
						//nBytesRead = 0;
						
					}
					else if ((cs->cbInQue == 2) && ReadSerialPortControl(hComm, packetBufferControl, cs->cbInQue, &nBytesRead) && (nBytesRead !=0))
					{
						packetBufferControl[2] = '\0';
						PacketCheckControl(*(HWND*)lphwnd, packetBufferControl);
						//nBytesRead = 0;
					}
					ClearCommError(hComm, &dwError, cs);
					dwWait = cs->cbInQue;
				} while (nBytesRead == 0);
				// endif "signaled"
				DisplayText(*(HWND*)lphwnd, displayBuffer);
		}
	}

	CloseHandle(ov.hEvent);
	ExitThread(EXIT_SUCCESS);
	
}