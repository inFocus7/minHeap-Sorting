#pragma once
#include "superQueue.h"

template <typename IN3>
class minHeap
{
private:
	int numItems;
public:
	minHeap()
	{
		superQueue<IN3> heap;
		numItems = 0;
	}
	void insert(IN3 input)
	{
		heap[numItems] = input;
		heap->prev = NULL;
	}
};