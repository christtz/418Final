#include <iostream>
#include "lists/sorted_list_seq.h"
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

//TODO get oparg working
int main(int argc , char *argv[]){
	println("WELCOME TO THE TESTING CLUB");
	int opt;
	string testName = "../tests/insert1.txt";
	ifstream ifs;

		
	while((opt = getopt(argc , argv, "t:")) != -1){
	  switch (opt) {
		  case 't': testName = optarg;
			    break;
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

	//Run Stuff
	trace tr;
	SeqList testList;

	parseFile(ifs , tr);
	runCmdList(tr, testList);
	println(testList.printlist());	

	println("DONE WITH TESTING");
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
	println("Running Sequential");
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
	println("Done with commands");
	println("Doing calculations");
	double totaltime = 0;;
	for(vector<cmd>::iterator it = tr.cmdlist.begin(); it != tr.cmdlist.end(); it++){
		cmd & c = *it; 
		totaltime += c.time;
	}

	//STATS ->> move to another function.
	println("Total time taken");
	cout << totaltime << endl;
	println("Average time taken");
	cout << totaltime / ((double) tr.cmdlist.size()) << endl;
	//STATS ->> move to another function.
}


//Run a parallel version 
void runCmdListPara(trace &tr , List &l ){
	println("Running parallel");
	int i=0;
	int size = tr.cmdlist.size();
	double start = 0;
	double end = 0;

	for(int i=0; i < size; i+= tr.batchsize){
	//TODO SCHEDULE CORRECTLY
	  int num = tr.batchsize > size - i ? size - i : tr.batchsize; 
	  #pragma omp parallel for schedule(static , 1)
	  for(int j = i; j < num; j++){
	    cmd & c  = tr.cmdlist[j];
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
	
	println("Done with commands");
	println("Doing calculations");
	//TODO parallelize in both places and move to another
	double totaltime = 0;;
	for(vector<cmd>::iterator it = tr.cmdlist.begin(); it != tr.cmdlist.end(); it++){
		cmd & c = *it; 
		totaltime += c.time;
	}

	//STATS ->> move to another function.
	println("Total time taken");
	cout << totaltime << endl;
	println("Average time taken");
	cout << totaltime / ((double) tr.cmdlist.size()) << endl;
	//STATS ->> move to another function.
		
}




