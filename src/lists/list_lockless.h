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

	private:
	listnode search(int value, listnode **node);
};

//Adds to front of the list
void FreeList::insert(int value){
	listnode *new_node = new listnode(value);
	listnode *right_node, *left_node;

	while (true) {
		right_node = search(value, &left_node);
		if ((right_node != tail) && (right_node->value == value)) return false;
		new_node->next = right_node;
		if (__sync_bool_compare_and_swap(&(left_node->next), right_node, new_node)) return true;
	}
}

//First occurence
void FreeList::remove(int value){
	listnode *right_node, *right_node_next, *left_node;
	while (true) {
		right_node = search(value, &left_node);
		if ((right_node == tail) || (right_node->value != value)) return false;
		right_node_next = right_node->next;
		if (!is_marked_reference(right_node_next)) {
			if (__sync_bool_compare_and_swap(&(right_node_next), right_node_next, get_marked_reference(right_node_next))) break;
		}
	}
	if (!__sync_bool_compare_and_swap(&(left_node->next), right_node, right_node_next)) {
		right_node = search(right_node->value, &left_node);
	}
	return true;
}

bool FreeList::find(int value){
	listnode *right_node, *left_node;
	right_node = search(value, &left_node);
	if ((right_node == tail) || (right_node->value != value)) return false;
	else return true;
}

listnode FreeList::search(int value, listnode **node) {

}


