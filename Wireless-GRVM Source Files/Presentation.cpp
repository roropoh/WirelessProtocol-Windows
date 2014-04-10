/*-----------------------------------------------------------------------------
--  SOURCE FILE:    Presentation.cpp
--
--  PROGRAM:        Wireless Protocol (GRVM)
--
--  FUNCTIONS:      BOOL AddToBuffer(const char* packet)
--  
--
--  DATE:           Nov. 30, 2013
--
--  REVISIONS:      ...
--
--  DESIGNER:       
--
--  PROGRAMMER:     
--
--  NOTES:       
--		Appends to the display stream (buffer)
--		
-----------------------------------------------------------------------------*/
#include "Presentation.h"
#define pos  1020

CHAR* displayBuffer;
int counterPacket = 0;
int oldLen;	//stores the current length of the buffer
int newLen;		//stores the new reallocated length

/*-----------------------------------------------------------------------------
-	FUNCTION:	AddToBuffer
-
-	DATE:		November 23, 2013
-
-	REVISIONS:	...
-
-	DESIGNER:	Vincent Lau
-
-	PROGRAMMER:	Vincent Lau
-
-	INTERFACE:	BOOL AddToBuffer(const char* packet)
-		
-	RETURNS:	BOOL - TRUE on appending
-				
-	PARAMETERS: const char* packet: Display stream.				
-				
-
-	NOTES:	Appends to the display stream.
-
-----------------------------------------------------------------------------*/
BOOL AddToBuffer(const char* packet)
{
	int sum = 0;
	if(displayBuffer == NULL)
	{
		oldLen = 0;
	} 
	else
	{
		oldLen = newLen;
	}
	int y = oldLen;
	counterPacket++;
	if(counterPacket>0){
		sum = pos * counterPacket;
	}
	
	displayBuffer = (CHAR*)realloc(displayBuffer, sum);
	int x = oldLen + strlen(packet);
	newLen = sum;
	
	for (size_t i = oldLen, j = 0; j < 1020; i++, j++)
	{
		if (packet[j] == '\0' || packet[j] == 'Í' || packet[j] == 'Í')
		{
			displayBuffer[i] = '\0';
			break;
		}
		displayBuffer[i] = packet[j];
	}
	
	return TRUE;

 }
