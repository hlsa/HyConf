#include "FIFO.hpp"
#include <stdlib.h>

FIFO::FIFO(int _size) {
	size = _size;
	head = 0;
	tail = 0;
	buffer = (double*) malloc(_size*sizeof(double));
	full = false;
}

FIFO::~FIFO() {
    free(buffer);
}


bool FIFO::read(double* res) {
	if (head != tail || full) { // there is at least one element in the fifo
		double value = buffer[head];
		head = (head + 1)%(size);
		if (full) {
			full = false;
		}
		(*res) = value;
		return true;
	} else {
		return false;
	}
}

bool FIFO::write(double value) {
	if (!full) {
		(buffer)[tail] = value;
		tail = (tail + 1)%(size);
		if (head == tail) {
			full = true;
		}
		return true;
	} else {
		return false;
	}
}

int FIFO::length() {
	if (tail == head) {
		if (full) return size;
		else return 0;
	} else {
		if (tail > head) return tail - head;
		else return tail + size - head;
	}
}
