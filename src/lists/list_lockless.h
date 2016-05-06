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

	private:
	listnode * search(int value, listnode*& node);
};

//Adds to front of the list
void FreeList::insert(int value){
	cout << "insert " << value << endl;
	listnode *new_node = new listnode(value);
	listnode *right_node, *left_node;

	while (true) {
		right_node = search(value, left_node);
		if ((right_node != tail) && (right_node->value == value)) return; //false;
		new_node->next = right_node;
		if (__sync_bool_compare_and_swap(&(left_node->next), right_node, new_node)) return;// true;
	}
	cout << "insert_e " << value << endl;
}


//First occurence
void FreeList::remove(int value){
	cout << "remove " << value << endl;	
	listnode *right_node, *right_node_next, *left_node;
	while (true) {
		right_node = search(value, left_node);
		cout << "i got here!!" << endl; 
		if ((right_node == tail) || (right_node->value != value)){ 
		cout << "remove_1 " << value << endl;	
		return;
		}// false;
		right_node_next = right_node->next;
		if (!is_marked_reference(right_node_next)) {
			if (__sync_bool_compare_and_swap(&(right_node_next), right_node_next, get_marked_reference(right_node_next))) break;
		}
	}
	if (!__sync_bool_compare_and_swap(&(left_node->next), right_node, right_node_next)) {
		right_node = search(right_node->value, left_node);
	}
	cout << "remove_2 " << value << endl;	
	return;// true;
}

bool FreeList::find(int value){
	listnode *right_node, *left_node;
	right_node = search(value, left_node);
	if ((right_node == tail) || (right_node->value != value)) return false;
	else return true;
}

listnode * FreeList::search(int value, listnode*& left_node) {
	cout << "search " << value << endl;	
	listnode *left_node_next, *right_node;
	cout << "the very first left_node_next" <<left_node_next << endl;
	SEARCH_AGAIN:do {
		cout << "searcha " << value << endl;	
		listnode *t = head;
		if(b(head)) cout << "FUCK head";
		listnode *t_next = head->next;

		cout << "searchb " << value << endl;	
		/* Find left and right node */
		do {
			cout << "is marked ref" << is_marked_reference(t_next) << endl;
			if (!is_marked_reference(t_next)) {
				(left_node) = t;
				left_node_next = t_next;
				cout << "t-next " << t_next << endl;
				cout << "left_node_next " << left_node_next << endl;
			}
			t = get_unmarked_reference(t_next);
			cout << "t is" << t << endl;

			if(b(t)) cout <<  "FUCK 1";
			if(b(t)) cout <<  "FUCK 2";
			if (t == tail) break;
			t_next = t->next;

			cout << "t_next" << t_next << endl;

			if(b(t)) cout <<  "FUCK 3";
		} while (is_marked_reference(t_next) || t->value < value);
		right_node = t;

		cout << "searchc " << value << endl;	
		/* Check nodes are adjacent */
			if(b(left_node_next)) cout <<  "FUCK 4";
			if(b(right_node)) cout <<  "FUCK 5";
		if (left_node_next == right_node) {
			if(b(right_node)) cout <<  "FUCK 6";
			if(b(tail)) cout <<  "FUCK 7";
			if ((right_node != tail) && is_marked_reference(right_node->next)) {
				goto SEARCH_AGAIN;
			}
			else {
			cout << "search1 " << value << endl;	
				return right_node;
			}
		}
		cout << "search dd " << value << endl;
		//&((*left_node)->next);	
		cout << "search d " << value << endl;	
		if(b(right_node)) cout <<  "FUCK 3";
		if(b(left_node_next)) cout <<  "FUCK 3";
		cout << "search e " << value << endl;
		/* Remove one or more marked nodes*/
		listnode *a = left_node_next;
		listnode *c = right_node;
		cout << "bloop " << endl;
		cout << "left node next" << left_node_next << endl;
		cout << "right node" << right_node << endl;
		if (a == NULL) cout << "a is null" << endl;
		if (c == NULL) cout << "c is null" << endl;
		//listnode **blah = &((*left_node)->next);
		//cout << blah << endl;
		cout << &(left_node->next) << "," << left_node_next << endl;
		cout << "left node -> next" << (left_node)->next << endl;
		cout << "blah" << *(&((left_node)->next)) << endl;
		if (*(&((left_node)->next)) == left_node_next) {
			cout << "yay" << endl;
			*(&((left_node)->next)) = right_node;
			cout << "ok we're good" << endl;
		}
		else cout << "yay2" << endl;
		bool ans = __sync_bool_compare_and_swap(&((left_node)->next), left_node_next, right_node);
		cout << "ans: " << ans << endl;
		if (__sync_bool_compare_and_swap(&((left_node)->next), left_node_next, right_node)) { // RIGHT HERE
			if(b(right_node)) cout <<  "FUCK 3";

			cout << "wtf " << value << endl;	
			if ((right_node != tail) && is_marked_reference(right_node->next)) {

				cout << "searchagain2 " << value << endl;	
				goto SEARCH_AGAIN;
			}
			else {
				cout << "search2 " << value << endl;	
				return right_node;
			}
		}

		cout << "wow " << value << endl;	
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

