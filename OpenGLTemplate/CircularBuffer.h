#include "Common.h"

class CircularBuffer {
private:
	int tail;
	int length;
	//vector<float> buffer;
	float *buffer;
	int accessible;
public:
	CircularBuffer();
	CircularBuffer(int l);
	void Put(float v);
	void Put(int i, float v);
	float AtPosition(int i);
};