#include <iostream>
#include <sstream>
#include <string>
#include <mutex>
#include <thread>
using namespace std;

//Simple list node with ints
//ASSUMING ALL OUR LISTS WILL BE IN THIS FORMAT
//ILL TAKE A GANDER WE CAN VERRIDE LATER
#ifndef __MYLIST__
#define __MYLIST__
typedef struct listnode {
	listnode *next;
	int value;
	mutex lock; 
	listnode (int v = 0) : value(v) {};
}listnode;

class List{
	public:
	virtual void insert(int value){};
	virtual void remove(int value){};
	virtual bool find(int value){return false;};
	string printlist();
	List();
	~List(){};
	
	protected:
	listnode *head;
	listnode *tail;
};

List::List(void){
	head = new listnode();
	tail = new listnode();
	head->next = tail;
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

#endif 
