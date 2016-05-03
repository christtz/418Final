#include <iostream>
#include "lists/list_seq.h"
#include <vector>
#include <fstream>
#include <string.h>
#include <stdlib.h>
#include "../include/CycleTimer.h"
#include <stdio.h>
#include <unistd.h>

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

void println(string str);
vector<cmd> parseFile(ifstream &ifs);
void runCmdList(vector<cmd> &cmdlist , List &l);

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
	vector<cmd> cmdlist = parseFile(ifs);
	List testList;
	runCmdList(cmdlist, testList);
	println(testList.printlist());	

	println("DONE WITH TESTING");
	return 0;
}


vector<cmd> parseFile(ifstream &ifs){
		
	// CMD INT
	Op op;
	int val;
	string s;
	vector<cmd> cmdlist; 
	for( string line; getline( ifs, line ); ) {
	  stringstream ss(line);
	  ss >> s;
	  if(s == "ins" || s == "INS"){
	  	op = INS;
	  }
	  else if(s == "rem" || s == "REM"){
	  	op = REM;
	  }
	  else if(s == "fin" || s=="FIN"){
		  op = FIN;
	  }
	  // SKIP OVER OTHERWISE
	  ss >> s;
	  val = atoi(s.c_str()); 
	  cmd plop(op , val , 0);
	  cmdlist.push_back(plop);
	}	    
	return cmdlist;
}

void println(string str){
  cout << str << endl;
}

//What about multiple threads yo?
//TODO Find nice way to 
void runCmdList(vector<cmd> &cmdlist , List &l){
	
	//Maybe make a log here???
	double start;
	double end;

	for(vector<cmd>::iterator it = cmdlist.begin(); it != cmdlist.end(); it++){
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
	for(vector<cmd>::iterator it = cmdlist.begin(); it != cmdlist.end(); it++){
		cmd & c = *it; 
		totaltime += c.time;
	}

	//STATS
	println("Total time taken");
	cout << totaltime << endl;
	println("Average time taken");
}



