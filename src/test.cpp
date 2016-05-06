#include <iostream>
#include "lists/sorted_list_seq.h"
#include "lists/list_coarse_grain.h"
#include "lists/list_fine_grain.h"
#include "lists/list_lockless.h"
#include <vector>
#include <fstream>
#include <string.h>
#include <stdlib.h>
#include "../include/CycleTimer.h"
#include <stdio.h>
#include <unistd.h>
#include <random>
#include <omp.h>
//The tester file for operations
using namespace std;
enum Op {INS , REM , FIN};

//Struct for information about list operations
typedef struct cmd{
	Op op;
	int val;
	double time;
	cmd (Op o = FIN, int v = 0 , double t = 0) : op(o), val(v), time(t) {};

} cmd;

typedef struct trace{
	vector<cmd> cmdlist;
	int nIns;
	int nRem;
	int nFin;
	int batchsize;
} trace;


void println(string str);
void parseFile(ifstream &ifs, trace &tr);
void runCmdList(trace &cmdlist , List &l);
void runCmdListPara(trace &tr , List &l, int numthreads);

//TODO get oparg working
int main(int argc , char *argv[]){
	int opt;
	string testName = "../tests/ll.txt";
	string outputFile = "";
	//bool print = true;
	bool ceq = false;
	int numthreads = omp_get_max_threads(); // 0 means max on system
	ifstream ifs;


	//options to run
	while((opt = getopt(argc , argv, "cotf:")) != -1){
	  switch (opt) {
		  case 'f': testName = optarg;
			    break;
		  case 't': numthreads = atoi(optarg);
			    break;
		  case 'o': outputFile = optarg;
			    break;
		  case 'c': ceq = true;
	  }
	}
	if(testName == ""){
		println("no file name spec");
		return 0;
	}
	ifs.open(testName.c_str());
	println("ok!");
	if(ifs.fail()){
		println("file failed to open");
		return 0;
	}
	println("Testname: "+testName);
	cout << "Numthreads: " << numthreads << endl;
	
	//Run Stuff
	
	trace tr;
	SeqList *slist = new SeqList();
	FineList *flist = new FineList();
	CoarseList *clist = new CoarseList();
	FreeList *lflist = new FreeList();

	//get Trace file
	parseFile(ifs , tr);
	// SEQ COARS FINE
	
	//Run the
	/* println("Seq");
	runCmdList(tr, *slist);
	delete(slist);
	println("Coarse");
	runCmdListPara(tr, *clist, numthreads);
	delete(clist);
	println("Fine");
	runCmdListPara(tr,*flist, numthreads);
	delete(flist); */
	println("Lock-Free");
	runCmdList(tr,*lflist);
   	delete(lflist);
	//Print Lists
	/*
	println("Seq, Coarse, Fine");
	println(slist.printlist());	
	println(clist.printlist());
	println(flist.printlist());*/
	/*
	if(ceq){
	if(!checkEq(*slist , *clist))
		println("Coarse List is not equal to check");
	if(!checkEq(*slist , *flist))
		println("Fine List is not equal to check");
	}*/
	return 0;
}


void parseFile(ifstream &ifs , trace &tr){
		
	// CMD INT
	Op op;
	int val;
	string s;
	vector<cmd> cmdlist; 
	
	int nIns = 0;
	int nRem = 0;
	int nFin = 0;
	string firstline = "";
	getline(ifs,firstline);
	int batchsize = atoi(firstline.c_str());

	for( string line; getline( ifs, line ); ) {
	  stringstream ss(line);
	  ss >> s;
	  if(s == "ins" || s == "INS"){
	  	op = INS;
		nIns++;
	  }
	  else if(s == "rem" || s == "REM"){
	  	op = REM;
		nRem++;
	  }
	  else if(s == "fin" || s=="FIN"){
		op = FIN;
		nFin++;
	  }
	  // SKIP OVER OTHERWISE
	  ss >> s;
	  val = atoi(s.c_str()); 
	  cmd plop(op , val , 0);
	  cmdlist.push_back(plop);
	}
	tr.nIns = nIns;
	tr.nRem = nRem;
	tr.nFin = nFin;
	tr.cmdlist = cmdlist;
	if(batchsize <= 0)
	  tr.batchsize = cmdlist.size();
	else
	  tr.batchsize = batchsize;
}

void println(string str){
  cout << str << endl;
}



//What about multiple threads yo?
//TODO Find nice way to 
void runCmdList(trace &tr , List &l){
	//Maybe make a log here???
	double start;
	double end;

	for(vector<cmd>::iterator it = tr.cmdlist.begin(); it != tr.cmdlist.end(); it++){
	  cmd & c  = *it;
	  switch( c.op){
		  case REM:
			start = CycleTimer::currentSeconds();
		  	l.remove(c.val);
			end = CycleTimer::currentSeconds();
			c.time = end - start;
			break;
		  case FIN:
			start = CycleTimer::currentSeconds();
			l.find(c.val);
			end = CycleTimer::currentSeconds();
			c.time = end - start;
			break;
		  case INS:
			start = CycleTimer::currentSeconds();
			l.insert(c.val);
			end = CycleTimer::currentSeconds();
			c.time = end - start;
			break;
	  }
	}

	double totaltime = 0;;
	for(vector<cmd>::iterator it = tr.cmdlist.begin(); it != tr.cmdlist.end(); it++){
		cmd & c = *it; 
		totaltime += c.time;
	}

	//STATS ->> move to another function.
	println("Total time taken");
	cout << totaltime << endl;
	/*println("Average time taken");
	cout << totaltime / ((double) tr.cmdlist.size()) << endl;
	//STATS ->> move to another function.
 	println("--------------ENDING SEQ--------------------");*/
}


//Run a parallel version 
void runCmdListPara(trace &tr , List &l, int numthreads ){
	int i=0;
	int size = tr.cmdlist.size();
	double start = 0;
	double end = 0;

	for(int i=0; i < size; i+= tr.batchsize){
	//TODO SCHEDULE CORRECTLY
	  int num = tr.batchsize > size - i ? size - i : tr.batchsize; 
	  // #pragma omp parallel for schedule(static , 1)
	  omp_set_num_threads(numthreads);
	  #pragma omp parallel for schedule(static , 1)
	  for(int j = i; j < i + num; j++){
	    cmd & c  = tr.cmdlist[j];//this is def a pointer in a block so we good
	    switch( c.op){
		  case REM:
			start = CycleTimer::currentSeconds();
		  	l.remove(c.val);
			end = CycleTimer::currentSeconds();
			c.time = end - start;
			break;
		  case FIN:
			start = CycleTimer::currentSeconds();
			l.find(c.val);
			end = CycleTimer::currentSeconds();
			c.time = end - start;
			break;
		  case INS:
			start = CycleTimer::currentSeconds();
			l.insert(c.val);
			end = CycleTimer::currentSeconds();
			c.time = end - start;
			break;
		
	    }
	  }
	}
	
	//TODO parallelize in both places and move to another
	double totaltime = 0;;
	for(vector<cmd>::iterator it = tr.cmdlist.begin(); it != tr.cmdlist.end(); it++){
		cmd & c = *it; 
		totaltime += c.time;
	}

	//STATS ->> move to another function.
	println("Total time taken");
	cout << totaltime << endl;
	/*println("Average time taken");
	cout << totaltime / ((double) tr.cmdlist.size()) << endl;
	//STATS ->> move to another function.
 	println("--------------DONE WITH PARALLEL--------------------");*/
}




