#ifndef __CIRCULAR_BUFFER_H__
#define __CIRCULAR_BUFFER_H__ 

#include <stdbool.h>

typedef struct sCircularBuffer {
	char * buffer;
	unsigned short size;
	unsigned short ip;
	unsigned short op;	

} CircularBuffer;

void CircularBufferInit(CircularBuffer * ctrl, char * buffer, unsigned short size);

bool CircularBufferEmpty (CircularBuffer * ctrl);

bool CircularBufferFull (CircularBuffer * ctrl);

bool CircularBufferInsert (CircularBuffer * ctrl, char c);

bool CircularBufferRemove (CircularBuffer * ctrl, char * c);

#endif