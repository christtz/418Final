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

//Inserts a new node to sorted list (assume duplicates are allowed)
void List::insert(int value){
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

//Remove first occurrence of node with value 
//Does nothing if value is not in the list
void List::remove(int value){
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

bool List::find(int value){
	listnode *curr = head;
	while(curr->next != tail && curr->next->value <= value){
		if (curr->next->value == value) return true;
		else curr = curr->next;
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
