#include <iostream>
#include "lists/list_seq.h"
#include <vector>
#include <fstream>
#include <string.h>
#include <stdlib.h>
#include "../include/CycleTimer.h"
#include <stdio.h>
#include <unistd.h>
#include <random>
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

	/*	
	while((opt = getopt(argc , argv, "t")) != -1){
	  switch (opt) {
		  case 't': testName = optarg;
			    break;
	  }
	}*/
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
	List testList;

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

void randTrace(trace &tr, int n, double top = 66.66f , double mid = 33.33f ){
	random_device rd;
	uniform_real_distribution<double> distribution(1, 100);
	mt19937 engine(rd());
	
	Op op;
	int val;
	string s;
	vector<cmd> cmdlist; 
	
	int nIns = 0;
	int nRem = 0;
	int nFin = 0;
	
	double yolo;
	for(int i =0; i<n; i++){
	  yolo = distribution(engine);
	  if(yolo > top){
	    op  = INS;
	    nIns++;
	  }
	  else if(yolo > mid){
	    op = REM;
	    nRem++;
	  }
	  else {
	    op = FIN;
	    nFin++;
	  }	
	  cmd plop(op , (int)yolo , 0);
	  cmdlist.push_back(plop);
	}
	tr.nIns = nIns;
	tr.nRem = nRem;
	tr.nFin = nFin;
	tr.cmdlist = cmdlist;
}





