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
	listnode *FreeList::search(int value, listnode **node);
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

listnode *FreeList::search(int value, listnode **left_node) {
	listnode *left_node_next, *right_node;
	SEARCH_AGAIN:do {
		listnode *t = head;
		listnode *t_next = head->next;

		/* Find left and right node */
		do {
			if (!is_marked_reference(t_next)) {
				(*left_node) = t;
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

		/* Remove one or more marked nodes*/
		if (__sync_bool_compare_and_swap(&(left_node->next), left_node_next, right_node)) {
			if ((right_node != tail) && is_marked_reference(right_node->next)) {
				goto SEARCH_AGAIN;
			}
			else {
				return right_node;
			}
		}
	} while (true);
}

listnode *get_marked_reference(listnode *node) {
	return node | 1;
}

listnode *get_unmarked_reference(listnode *node) {
	return node & ((~0)-1);
}

bool is_marked_reference(listnode *node) {
	return (node & 1 == 1);
}

