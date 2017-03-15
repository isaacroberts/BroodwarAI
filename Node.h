/* 
 * File:   Node.h
 * Author: Isaac
 *
 * Created on April 8, 2012, 9:17 PM
 */

#ifndef NODE_H
#define	NODE_H

#include <vector>
#include <string>
#include "Util.h"

class Goal;

using namespace std;

class Node {
public:
    Node();
	void setNetname(string set) {netname=set;}
    virtual double get();
    virtual double getState();
    virtual double getLastState();
    bool justFired();
    bool justTurnedOff();
	double getCurVal() {return currentVal;}
    void setCurVal(double);
    void addCurVal(double);
    void setState(double);
    virtual void reset();
	virtual void unreset();
    void setThresh(double);
    void learnThresh(double);
    void setMiscMod(double);
    bool hasMod();
    double getMiscMod(double);
    void learnWeight(Node*,double);
    void addConn(Node*,double);
    void addConn(Node*);
	void setConnWeight(unsigned int n,double weight) {connWeights[n]=weight;}
	void setConnWeight(Node*,double);
    double getThresh();
    virtual void setName(string);
	void setJob(int);
	void setType(int);
	void setCondConn(Node*);
	void setCondConnIsNot(bool isNot);
	virtual void giveNewID() {id=curNodeID; curNodeID++;}
	int getID() { return id; }
	virtual void shun(Node*);
	string getName();
	int getJob();//what the node does (input, determiner, perception,etc)
	int getType();//the type of output (on/off, flash, linear,etc)
	bool subtractsThresh();
	bool comparesToThresh();
    bool isPerc();
    bool isDtrm();
	bool isPerm();
    bool isEditable();
	unsigned int connAmt() {return conns.size();}
	Node* getConn(int n) {return conns[n];}
	double getConnWeight(int n) {return connWeights[n];}
    vector<Node*> getConns();
    vector<double> getWeights();
    Node(const Node& orig);
	//operator == {return }
    virtual ~Node();
	enum Job
	{
		Input=0,//i
		Percp=1,//p
		Expect=2,//e
		PermInput=3,//pI
		ConstPercp=4,//cp
		Determ=5,//d
		Output=6,//o
		Memory=7,//m
		Calc=8,//c
		ShortTermMem=9,//s
		GoalDrive=10,GoalMeet=11,GoalStrive=12,
		PermPercp=13,//pP
		Unemployed
	};
	enum NodeType
	{
		Bool=0,//b
		Linear=1,//l
		Flash=2,//f
		OneShot=3,//1
		Delay=4,//d
		Cumltv=5,//c
		Divide=6,//÷
		Multiply=7,// x
		Invert=8,// 1
		Set=9,
		Counter=10,//ctr
		Maxer=11,//finds the maximum input
		LinFlash=12,
		Average=13,
		None
	};
protected:
	string netname;
    virtual void calcState(bool update);
    double currentVal;
    double thresh;
    double miscMod;
    double state;
    double lastState;
    bool checked,visited;
    vector<Node*> conns;
    vector<double> connWeights;
    string name;
	Job job;
	NodeType type;
    Node* cond;
	bool condIsNot;
	int id;

	static int curNodeID;
};

#endif	/* NODE_H */

