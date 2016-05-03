#include <iostream>
#include <sstream>
#include <string>

using namespace std;

//Simple list node with ints, not embedded
typedef struct listnode {
	listnode *next;
	int value;
	listnode (int v = 0) : value(v) {};
}listnode;

class List{
	public:
	void insert(int value);
	void remove(int value);
	bool find(int value);
	string printlist();
	List();
	
	private:
	listnode *head;
	listnode *tail;
};

List::List(void){
	head = new listnode();
	tail = new listnode();
	head->next = tail;
}

//Adds to front of the list
void List::insert(int value){
	listnode *ptr = new listnode(value);
	ptr->next = head->next;
	head->next = ptr;
}

//First occurence
void List::remove(int value){
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

bool List::find(int value){
	listnode *ptr = head->next;
	while(ptr != tail){
		if(ptr->value == value)
		  return true;
		ptr = ptr->next;
	}
	return false;
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
