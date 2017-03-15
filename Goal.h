#ifndef GOAL_H
#define	GOAL_H

#include "Node.h"

class Node;

using namespace std;

class Goal : public Node
{
public:
	Goal();
	double get();
	void transferMem();
	void reset();
	void unreset();
	void setMeet(Node* set){meet=set;}
	void setStrive(Node* set){strive=set;}
	void setMem(Node* set){mem=set;}
	void setName(string set);
	void giveNewID();
	void shun(Node*);
	Node* getMeet();
	Node* getStrive();
	Node* getMem() {return mem;}
	void setStriveWeight(double set) {striveWeight=set;}
	void setMeetWeight(double set) {meetWeight=set;}
private:
	Node *meet,*strive,*mem;//the other two
	double striveWeight,meetWeight;
};

#endif

