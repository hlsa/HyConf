#pragma once

class FIFO {
	double *buffer;
	int head;
	int tail;
	int size;
	bool full;
public:
    FIFO(int _size);
    ~FIFO();
    bool read(double* res);
    bool write(double value);
    int length();
};