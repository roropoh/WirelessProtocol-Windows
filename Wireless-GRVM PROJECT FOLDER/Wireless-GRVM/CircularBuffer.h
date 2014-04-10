#ifndef CIRCULARBUFFER_H
#define CIRCULARBUFFER_H

#define TRUE 1
#define FALSE 0
 
typedef char BufferType;
 
struct CircularBuffer {
        int size;
        int first;  // position of first element
        //int end;  // position of last element
        /* Tracking start and end of buffer would waste
         * one position. A full buffer would always have
         * to leave last position empty or otherwise
         * it would look empty. Instead this buffer uses
         * count to track if buffer is empty or full
         */
        int count; // number of elements in buffer
 
        /* Two ways to make buffer element type opaque
         * First is by using typedef for the element
         * pointer. Second is by using void pointer.
         */
        BufferType *elements;
};
 
typedef struct CircularBuffer cb;
 
void initBuffer(cb * cb_ptr, int size);
void writeBuffer(cb *cb_ptr, BufferType element);
BufferType readBuffer(cb *cb_ptr);
int isEmpty(cb *cb_ptr);
int isFull(cb *cb_ptr);
void restartBuffer(cb *cbptr);

#endif