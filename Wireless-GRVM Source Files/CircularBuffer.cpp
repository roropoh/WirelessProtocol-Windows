/* Circular Buffer implementing a queue */
 

#include <stdio.h>
#include <stdlib.h> //malloc
#include "CircularBuffer.h"
 
 
void initBuffer(cb *cbptr, int size)
{
        cbptr->size = size;
        cbptr->count = 0;
        cbptr->first = 0; // could be anywhere actually
        cbptr->elements = (BufferType*)malloc(size * sizeof(BufferType));
        printf("Buffer allocated\n");
}
 
int isFull(cb *cbptr)
{
        if (cbptr->count == cbptr->size) return TRUE;
        else return FALSE;
}
int isEmpty (cb *cbptr)
{
        return (cbptr->count == 0) ?  TRUE : FALSE;
}
 
void writeBuffer(cb *cbptr, BufferType element)
{
        if (isFull(cbptr))
                return;
        cbptr->elements[(cbptr->first + cbptr->count++) % cbptr->size] = element;
        printf("Added %c\n", element);
        return;
}
 
BufferType readBuffer(cb *cbptr)
{
        if (isEmpty(cbptr))
                return 0;
        return (cbptr->elements[cbptr->first++ % cbptr->size]);
}
void restartBuffer(cb *cbptr){
	if (isFull(cbptr)){
		cbptr->count = 0;
		cbptr->first = 0;
	}
}