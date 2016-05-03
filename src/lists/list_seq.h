#include <iostream>
#include <sstream>
#include <string>
#include <mylist.h>

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
	ptr->next = head->next;
	head->next = ptr;
}

//First occurence
void SeqList::remove(int value){
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

bool SeqList::find(int value){
	listnode *ptr = head->next;
	while(ptr != tail){
		if(ptr->value == value)
		  return true;
		ptr = ptr->next;
	}
	return false;
}


