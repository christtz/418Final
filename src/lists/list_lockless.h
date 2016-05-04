#include <iostream>
#include <sstream>
#include <string>
#include "mylist.h"

using namespace std;

//Compare and swap = __sync_bool_compare_and_swap

class FreeList : public List{
	public:
	void insert(int value);
	void remove(int value);
	bool find(int value);
	//string printlist();
	FreeList() : List(){ };
};

//Adds to front of the list
void FreeList::insert(int value){
	listnode *ptr = new listnode(value);
	ptr->next = head->next;
	head->next = ptr;
}

//First occurence
void FreeList::remove(int value){
	listnode *curr = head->next;
	listnode *prev = head;
	while(curr != tail){
		if(curr->value == value){
		  prev->next = curr->next;
		  delete curr;// <----- this work?
		  return;
		}
		prev = curr;
		curr = curr->next;
	}
}

bool FreeList::find(int value){
	listnode *ptr = head->next;
	while(ptr != tail){
		if(ptr->value == value)
		  return true;
		ptr = ptr->next;
	}
	return false;
}


