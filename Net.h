/* 
 * File:   Net.h
 * Author: Isaac
 *
 * Created on April 8, 2012, 9:40 PM
 */

#ifndef NET_H
#define	NET_H

#include <vector>
#include <string>
#include "Util.h"
#include "Node.h"
#include "Goal.h"

using namespace std;

class Net {
public:
    Net();
	Net(int type);
	void createNodes(int amt);
	void addNode(Node*);
	//void passIn(Economy* ec,Army* rm) {
		//econ=ec;
		//army=rm;                   	  }
	void resetNodes();
	void unreset();
	void recall(char[]);
    void initialize();
    void learn();
    void write();
	Net* copy(Net* toCopy);
	void excommunicate(Node*);
	unsigned int nodeAmt() {return node.size(); }
	Node* getNode(unsigned int id);
	Node* getNode(std::string);
	void replace(int nodeId,Node* replaceWith);
	unsigned int inputAmt() {return input.size(); }
	Node* getInput(unsigned int);
	unsigned int outputAmt() {return output.size();}
	Node* getOutput(unsigned int);
	int getType() {return type;}
	static bool canConnect(Node*,Node*);
	void giveNewID();
	int getID()	{return id;	}
	void setName(string set) {	name=set;}
	string getName() {	return name;}
    Net(const Net& orig);
	enum NetType {
		Econ,Army,Area,CmndGroup,UnitType,Base,Goal,None
	};
	static NetType stringToNetType(string);
    virtual ~Net();
private:
	//Army *army;
	//Economy* econ;
    vector<Node*> node;
    vector<Node*> input,output;
	int id;
	NetType type;
	string name;//the name is the filename without the "Nets/" and the ".txt"
	bool isTrim;
	static int curNetID;
};

#endif	/* NET_H */