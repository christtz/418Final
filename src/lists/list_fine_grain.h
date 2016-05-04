#include <iostream>
#include <sstream>
#include <string>
#include "mylist.h"

using namespace std;

class FineList : public List{
	public:
	void insert(int value);
	void remove(int value);
	bool find(int value);


	//string printlist();
	FineList() : List(){ };

	private:
	mutex mylock;
};

//Adds to front of the list
void FineList::insert(int value){
	listnode *ptr = new listnode(value);
	listnode *prev, *curr;
	head->lock.lock();
	prev = head;
	curr = head->next;
	//Special case for head end

	if (curr != tail) curr->lock.lock();

	while (curr != tail) {
		if (curr->value > value) break;
		listnode *old_prev = prev;
		prev = curr;
		curr = curr->next;
		old_prev->lock.unlock();
		if (curr != tail) curr->lock.lock();
	}
	ptr->next = curr;
	prev->next = ptr;
	cout << "inserting" << value << endl;
	prev->lock.unlock();
	if (curr != tail) curr->lock.unlock();
}

//First occurrence
void FineList::remove(int value){
	listnode *prev, *curr;
	head->lock.lock();
	prev = head;
	curr = head->next;;
	if (curr != tail) curr->lock.lock();
	while(curr != tail && curr->value <= value) {
		if(curr->value == value){
		  prev->next = curr->next;
		  prev->lock.unlock();
		  curr->lock.unlock();
		  delete curr; // <----- this work?
		  return;
		}
		listnode *old_prev = prev;
		prev = curr;
		curr = curr->next;
		if (curr != tail) curr->lock.lock();
		old_prev->lock.unlock();
	}
	prev->lock.unlock();
}

bool FineList::find(int value){

	listnode *prev, *curr;
	head->lock.lock();
	prev = head;
	curr = head->next;
	if (curr != tail) curr->lock.lock();
	while(curr != tail && curr->value <= value) {
		if(curr->value == value){
		  prev->lock.unlock();
		  curr->lock.unlock();
		  return true;
		}
		listnode *old_prev = prev;
		prev = curr;
		curr = curr->next;
		if (curr != tail) curr->lock.lock();
		old_prev->lock.unlock();
	}
	prev->lock.unlock();
	return false;
}


