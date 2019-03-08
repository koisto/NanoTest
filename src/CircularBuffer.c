#include "CircularBuffer.h"

void CircularBufferInit(CircularBuffer * ctrl, char * buffer, unsigned short size)
{
	ctrl->buffer = buffer;
	ctrl->size = size;
	ctrl->ip = 0;
	ctrl->op = 0;
}

bool CircularBufferEmpty (CircularBuffer * ctrl)
{
	return ((ctrl->ip - ctrl->op) == 0);
}

bool CircularBufferFull (CircularBuffer * ctrl)
{	
	return ((ctrl->ip - ctrl->op) >= ctrl->size);
}

bool CircularBufferInsert (CircularBuffer * ctrl, char c)
{
	if (CircularBufferFull(ctrl) == false)
	{
		ctrl->buffer[ctrl->ip & (ctrl->size-1)] = c;
		ctrl->ip++;

		return true;
	}
	else
	{
		return false;
	}
}

bool CircularBufferRemove (CircularBuffer * ctrl, char * c)
{
	if (CircularBufferEmpty(ctrl) == false)
	{
		*c = ctrl->buffer[ctrl->op & (ctrl->size-1)];
		ctrl->op++;
		
		return true;
	}
	else
	{
		return false;
	}
}