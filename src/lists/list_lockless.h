#include <iostream>
#include <sstream>
#include <string>
#include "mylist.h"

using namespace std;

//Compare and swap = __sync_bool_compare_and_swap

listnode *get_marked_reference(listnode *node);
listnode *get_unmarked_reference(listnode *node);
bool is_marked_reference(listnode *node);

bool b(listnode *n){
	return ((long)n % 2 == 1);
}


class FreeList : public List{
	public:
	void insert(int value);
	void remove(int value);
	bool find(int value);
	//string printlist();
	FreeList() : List(){ };
	~FreeList() {};

	private:
	listnode * search(int value, listnode*& node);
};

//Adds to front of the list
void FreeList::insert(int value){
	cout << "ins " << value << endl;
	listnode *new_node = new listnode(value);
	listnode *right_node, *left_node;

	while (true) {
		right_node = search(value, left_node);
		if ((right_node != tail) && (right_node->value == value)) return; //false;
		new_node->next = right_node;
		if (__sync_bool_compare_and_swap(&(left_node->next), right_node, new_node)) return;// true;
	}
}


//First occurence
void FreeList::remove(int value){
	cout << "rem " << value << endl;
	/*listnode *p = head;
	while (p != tail) {
		cout << "p: " << p << " with value: " << p->value << endl;
		p = p->next;
	}
	cout << "is p = tail?: " << (p == tail) << endl;*/

	listnode *right_node = NULL, *right_node_next = NULL, *left_node = NULL;
	while (true) {
		right_node = search(value, left_node);
		cout << "right_node is: " << right_node << " with value " << right_node->value << endl;
		cout << "left_node is: " << left_node << " with value " << left_node->value << endl;
		if ((right_node == tail) || (right_node->value != value)){ 
		return;
		}// false;
		right_node_next = right_node->next;
		if (!is_marked_reference(right_node_next)) {
			if (__sync_bool_compare_and_swap(&(right_node->next), right_node_next, get_marked_reference(right_node_next))) break;
		}
	}
	if (!__sync_bool_compare_and_swap(&(left_node->next), right_node, right_node_next)) {
		right_node = search(right_node->value, left_node);
	}
	return;// true;
}

bool FreeList::find(int value){
	listnode *right_node, *left_node;
	right_node = search(value, left_node);
	if ((right_node == tail) || (right_node->value != value)) return false;
	else return true;
}

listnode * FreeList::search(int value, listnode*& left_node) {
	listnode *left_node_next, *right_node;
	SEARCH_AGAIN:do {
		listnode *t = head;
		listnode *t_next = head->next;
		/* Find left and right node */
		do {
			if (!is_marked_reference(t_next)) {
				(left_node) = t;
				left_node_next = t_next;
			}
			t = get_unmarked_reference(t_next);

			if (t == tail) break;
			t_next = t->next;

		} while (is_marked_reference(t_next) || t->value < value);
		right_node = t;

		/* Check nodes are adjacent */
		if (left_node_next == right_node) {
			if ((right_node != tail) && is_marked_reference(right_node->next)) {
				goto SEARCH_AGAIN;
			}
			else {
				return right_node;
			}
		}
		//&((*left_node)->next);	
		/* Remove one or more marked nodes*/
		/*listnode *a = left_node_next;
		listnode *c = right_node;
		//listnode **blah = &((*left_node)->next);
		cout << "hello im here!" << endl;
		cout.flush();*/
		if(left_node == NULL){
			cout << "shit_ho" << endl;
		}
	       	if (*(&(left_node->next)) == left_node_next) {
			*(&(left_node->next)) = right_node;
			cout << "true" << endl;
		}
		else {
			cout << "false" << endl;
		}
		__sync_bool_compare_and_swap(&((left_node)->next), left_node_next, right_node); // YELLOW
			
		if (__sync_bool_compare_and_swap(&((left_node)->next), left_node_next, right_node)) { // RIGHT HERE
			cout << "hiiiii" << endl;
			cout.flush();
			if ((right_node != tail) && is_marked_reference(right_node->next)) {
				goto SEARCH_AGAIN;
			}
			else {
				return right_node;
			}
		}

	} while (true);
}

listnode * get_marked_reference(listnode *node) {
	return (listnode *)(((long)node) | 1);
}

listnode * get_unmarked_reference(listnode *node) {
	return (listnode *)((long)node & ((~0)-1));
}

bool is_marked_reference(listnode *node) {
	return ( (long)node & 1 == 1);
}

