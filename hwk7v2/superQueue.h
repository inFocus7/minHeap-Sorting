#pragma once
#include <algorithm>

template <typename IN>
class node {
private:

public:
	node *next, *prev;
	IN data;
};

template <typename IN2>
class superQueue {
private:
	node<IN2> * head, *tail;
	int numItems;
	node<IN2> * parent(int& i)
	{
		node<IN2> * tmp = head;
		int parentPos = (i - 1) / 2; //Works since it's returned as int, and the value floors.
		for (int i = 0; i < parentPos; i++)
		{
			tmp = tmp->next;
		}
		i = parentPos;
		return tmp;
	}
	node<IN2> * lChild(int& i)
	{
		node<IN2> * tmp = head;
		int lChildPos = i * 2 + 1;
		for (int i = 0; i < lChildPos && tmp != NULL; i++)
		{
			tmp = tmp->next;
		}
		i = lChildPos;
		return tmp;
	}
	node<IN2> * rChild(int& i)
	{
		node<IN2> * tmp = head;

		int rChildPos = i * 2 + 2;
		for (int i = 0; i < rChildPos && tmp != NULL; i++)
		{
			tmp = tmp->next;
		}
		i = rChildPos;
		return tmp;
	}
public:
	superQueue() //default constructor
	{
		head = NULL;
		tail = NULL;
		numItems = 0;
	}

	void push(IN2 input) //Add to back
	{
		node<IN2> * temp = new node<IN2>;
		temp->data = input;
		if (head == NULL)
		{
			head = temp;
			tail = head;
			//Not sure if these are needed
			head->prev = NULL;
			tail->next = NULL;
		}
		else
		{
			tail->next = temp;
			temp->prev = tail;
			tail = temp;
			tail->next = NULL;
		}
		numItems++;
	} //add back

	IN2 pop() //pops back
	{
		IN2 data = tail->data;
		if (numItems == 1)
		{
			head = NULL;
			tail = NULL;
		}
		else
		{
			node<IN2> * doomed = tail;
			tail = tail->prev;
			delete doomed;
			tail->next = NULL;
		}
		numItems--;
		return data;
	} //remove back

	IN2& operator[](int index) //Overloads operator
	{
		node<IN2> * current = head;
		for (int i = 0; i < index; i++)
		{
			current = current->next;
		}
		return current->data;
	}

	bool empty() //Check if empty
	{
		if (numItems == 0)
			return true;
		return false;
	}

	void enqueue(IN2 input) //Add front
	{
		node<IN2> * tmp = new node<IN2>;
		tmp->data = input;
		if (head == NULL)
		{
			head = tmp;
			tail = tmp;
			head->prev = NULL;
			tail->next = NULL;
		}
		else
		{
			tmp->next = head;
			tmp->prev = NULL;
			head = tmp;
		}
		numItems++;
	}

	IN2 dequeue() //Remove front
	{
		IN2 output = head->data;
		if (numItems == 1)
		{
			head = NULL;
			tail = NULL;
		}
		else
		{
			node<IN2> * doomed = head;
			head = head->next;
			delete doomed;
			head->prev = NULL;
		}
		numItems--;
		return output;
	}

	void insert(IN2 input)
	{
		//1)Add to stack && can be thought of a dynamic array with overloaded op.
			//Update) Was going to do external minHeap but found it weird making a class for the "superqueue" in the minheap.h, and then a heap class here. Would probably have worked and better since I'd be able to use the [] and treat it like an array... although I wasn't too sure if heap[0].data or something like that wouldve worked to edit stuff.
		push(input);
		if (numItems > 1)
		{
			//2)Bubble up
			int currentNodeIndex = numItems - 1;
			node<IN2> * current = tail; //set to tail since it's latest data input
			//node<IN2> * theParent = parent(currentNodeIndex);
			for (node<IN2> * theParent = parent(currentNodeIndex); current->data < theParent->data; current = theParent, theParent = parent(currentNodeIndex))
			{
				swap(current->data, theParent->data);
			}

		}
	}
	
	IN2 extractMin()
	{
		IN2 output = head->data;
		if (numItems == 1)
		{
			numItems--;
			head = NULL;
			tail = NULL;
		}
		else
		{
			//Move head
			swap(head->data, tail->data); //swaps root and final in order to easily...
			pop();						 //pop.
			//Bubble down
			int currentNodeIndex = 0;
			int bckup1, bckup2, bckup3, bckup4;
			node<IN2> * current = head; //set to head since it's where the new root was stored (temporarily)
			while (childProblem(currentNodeIndex, current))
			{
				//This backup system came out bad, but was kind of needed since the way I passed by reference when getting child nodes. I could've done it a way better way, like passing two variables and ony one passing by reference, but didn't think of that until the night before turning it in when I was looking over my minHeap which didn't seem right (speed-wise)... Could fix it but trying to focus on studying for exams, and at least I saw the sorting/linked list worked for even 100000s of numbers.
				bckup1 = currentNodeIndex;
				bckup2 = bckup1;
				bckup3 = bckup2;
				bckup4 = bckup3;
				//swap current with smaller
				rChild(bckup4); //did this here to change the value for child index
				if (bckup4 == numItems) //1 child || index of rChild
				{
					//Doing it this way since I pass currentNodeIndex by reference so it'll change before I change current, so a made one that has the index before the pass.
					swap(current->data, lChild(bckup1)->data); 
					current = lChild(bckup2);
				}
				else 
				{
					//currentNodeIndex = bckup3; //reset it
					bckup1 = bckup3;
					bckup2 = bckup1;
					if (lChild(bckup1)->data < rChild(bckup2)->data)
					{
						currentNodeIndex = bckup1;
						bckup1 = bckup3;
						bckup2 = bckup1;
						swap(current->data, lChild(bckup1)->data);
						current = lChild(bckup2);
					}
					else
					{
						currentNodeIndex = bckup2;
						bckup1 = bckup3;
						bckup2 = bckup1;
						swap(current->data, rChild(bckup1)->data);
						current = rChild(bckup2);
					}
				}
				bckup1 = bckup3;
				bckup2 = bckup1;
				//currentNodeIndex = bckup1;
			}
		}
		return output;
	}

	bool childProblem(int current, node<IN2> * parent)
	{
		int bckup = current, bckup2 = bckup;
		rChild(bckup);
		lChild(current);
		if (current >= numItems) //out of bounds, no children
		{
			return false;
		}
		else if (bckup == numItems) //1 child
		{
			bckup = bckup2;
			if (lChild(bckup)->data < parent->data)
				return true;
			else 
				return false;
		}
		else //2 children
		{
			current = bckup2;
			bckup = bckup2;
			if (lChild(current)->data < parent->data || rChild(bckup)->data < parent->data) //could've done the min, but this is the same and can't spend too much time on the hw since i have to study for 3 exams.
				return true;
			else
				return false;
		}
	}
};