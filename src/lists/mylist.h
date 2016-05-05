#include <iostream>
#include <sstream>
#include <string>
#include <mutex>
#include <thread>
#include <pthread.h>

using namespace std;

//Simple list node with ints
//ASSUMING ALL OUR LISTS WILL BE IN THIS FORMAT
//ILL TAKE A GANDER WE CAN VERRIDE LATER
#ifndef __MYLIST__
#define __MYLIST__
typedef struct listnode {
	listnode *next;
	int value;
	pthread_spinlock_t lock;
	int pshared;

	listnode (int v = 0) : value(v) {
		pthread_spin_init(&lock, pshared);
	};
}listnode;

class List{
	public:
	virtual void insert(int value){};
	virtual void remove(int value){};
	virtual bool find(int value){return false;};
	string printlist();
	List();
	~List();
	listnode *head;
	listnode *tail;
	//changed to public yolo
};

List::List(void){
	head = new listnode();
	tail = new listnode();
	head->next = tail;
}

List::~List(){

	listnode *ptr = head->next;
	while(ptr != tail){
	  listnode *temp = ptr;
	  ptr = ptr->next;
	  delete(temp);
	}
	delete(head);
	delete(tail);
}

string List::printlist(){
	listnode *ptr = head->next;
	stringstream s;
	while(ptr != tail){
		s << ptr->value << " ";  
		ptr = ptr->next;
	}
	return s.str();
}

bool checkEq(const List &a, const List &b){
	
	listnode *aptr = a.head->next;
	listnode *bptr = b.head->next;

	while(aptr != a.tail && bptr != b.tail){
		if(aptr->value != bptr->value)
			return false;
		aptr = aptr->next;
		bptr = bptr->next;
	}
	return ((aptr == a.tail) && (bptr == b.tail));
}

#endif 
