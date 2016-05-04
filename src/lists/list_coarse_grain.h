#include <iostream>
#include <sstream>
#include <thread>
#include <mutex>
#include <string>
#include <mylist.h>

using namespace std;

class CoarseList : public List{
	public:
	void insert(int value);
	void remove(int value);
	bool find(int value);
	//string printlist();
	CoarseList() : List(){ };

	private:
	mutex mylock;
};

//Adds to front of the list
void CoarseList::insert(int value){
	listnode *ptr = new listnode(value);
	mylock.lock();
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
	mylock.unlock();
}

//First occurrence
void CoarseList::remove(int value){
	mylock.lock();
	listnode *curr = head->next;
	listnode *prev = head;
	while(curr != tail && curr->value <= value) {
		if(curr->value == value){
		  prev->next = curr->next;
		  delete curr; // <----- this work?
		  mylock.unlock();
		  return;
		}
		prev = curr;
		curr = curr->next;
	}
	mylock.unlock();
}

bool CoarseList::find(int value){
	mylock.lock();
	listnode *curr = head;
	while(curr->next != tail && curr->next->value <= value){
		if (curr->next->value == value) {
			mylock.unlock();
			return true;
		}
		else curr = curr->next;
	}
	mylock.unlock();
	return false;
}


