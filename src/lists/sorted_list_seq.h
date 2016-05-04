#include <iostream>
#include <sstream>
#include <string>
#include "mylist.h"

using namespace std;

class SeqList : public List{
	public:
	void insert(int value);
	void remove(int value);
	bool find(int value);
	//string printlist();
	SeqList() : List(){ };
};

//Adds to front of the list
void SeqList::insert(int value){
	listnode *ptr = new listnode(value);
	listnode *curr = head;
	//Special case for head end
	if (head->next == tail) {
		ptr->next = tail;
		head->next = ptr;
	}
	else {
		while (curr->next != tail && curr->next->value < value) {
			curr = curr->next;
		}
		ptr->next = curr->next;
		curr->next = ptr;
	}
}

//First occurrence
void SeqList::remove(int value){
	listnode *curr = head->next;
	listnode *prev = head;
	while(curr != tail && curr->value <= value) {
		if(curr->value == value){
		  prev->next = curr->next;
		  delete curr; // <----- this work?
		  return;
		}
		prev = curr;
		curr = curr->next;
	}
}

bool SeqList::find(int value){
	listnode *curr = head;
	while(curr->next != tail && curr->next->value <= value){
		if (curr->next->value == value) return true;
		else curr = curr->next;
	}
	return false;
}


