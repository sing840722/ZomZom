#include "CircularBuffer.h"

#define min(a, b) (((a) < (b)) ? (a) : (b));
#define max(a, b) (((a) > (b)) ? (a) : (b));

CircularBuffer::CircularBuffer():length(0), tail(0), buffer(NULL), accessible(0) {

}

CircularBuffer::CircularBuffer(int l) :length(l), tail(0), buffer(new float[l]), accessible(0) {
	//buffer.resize(l);
}


void CircularBuffer::Put(float v) {
	//buffer[tail % length] = v;
	//tail++;
}

void CircularBuffer::Put(int i, float v) {
	buffer[i%length] = v;
	tail++;

	accessible = max(0, tail - length);
}


float CircularBuffer::AtPosition(int i) {
	if (i < accessible) {
		tail = 0;
		accessible = 0;
		return NULL;
	}

	return buffer[i%length];
}