#include <iostream>
#include <sstream>
#include <string>
#include <pthread.h>
#include "mylist.h"

using namespace std;

class FineList : public List{
	public:
	void insert(int value);
	void remove(int value);
	bool find(int value);
	FineList() : List(){ };
};

//Adds to front of the list
void FineList::insert(int value){
	listnode *ptr = new listnode(value);
	listnode *prev, *curr;
	pthread_spin_lock(&(head->lock));
	prev = head;
	curr = head->next;
	//Special case for head end

	if (curr != tail) pthread_spin_lock(&(curr->lock));

	while (curr != tail) {
		if (curr->value > value) break;
		listnode *old_prev = prev;
		prev = curr;
		curr = curr->next;
		pthread_spin_unlock(&(old_prev->lock));
		if (curr != tail) pthread_spin_lock(&(curr->lock));
	}
	ptr->next = curr;
	prev->next = ptr;
	pthread_spin_unlock(&(prev->lock));
	if (curr != tail) pthread_spin_unlock(&(curr->lock));
}

//First occurrence
void FineList::remove(int value){
	listnode *prev, *curr;
	pthread_spin_lock(&(head->lock));
	prev = head;
	curr = head->next;;
	if (curr != tail) pthread_spin_lock(&(curr->lock));
	while(curr != tail && curr->value <= value) {
		if(curr->value == value){
		  prev->next = curr->next;
		  pthread_spin_unlock(&(prev->lock));
		  pthread_spin_unlock(&(curr->lock));
		  delete( curr); // <----- this work?
		  return;
		}
		listnode *old_prev = prev;
		prev = curr;
		curr = curr->next;
		if (curr != tail) pthread_spin_lock(&(curr->lock));
		pthread_spin_unlock(&(old_prev->lock));
	}
	pthread_spin_unlock(&(prev->lock));
}

bool FineList::find(int value){

	listnode *prev, *curr;
	pthread_spin_lock(&(head->lock));
	prev = head;
	curr = head->next;
	if (curr != tail) pthread_spin_lock(&(curr->lock));
	while(curr != tail && curr->value <= value) {
		if(curr->value == value){
		  pthread_spin_unlock(&(prev->lock));
		  pthread_spin_unlock(&(curr->lock));
		  return true;
		}
		listnode *old_prev = prev;
		prev = curr;
		curr = curr->next;
		if (curr != tail) pthread_spin_lock(&(curr->lock));
			pthread_spin_unlock(&(old_prev->lock));
	}
	pthread_spin_unlock(&(prev->lock));
	return false;
}


