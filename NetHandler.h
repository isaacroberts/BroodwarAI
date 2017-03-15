#ifndef NETHANDLER_H
#define NETHANDLER_H

#include <string>
#include <vector>
#include <BWAPI.h>
#include "Util.h"
#include "Net.h"

using namespace std;

class NetHandler
{
public:
	NetHandler();
	static Net* initNet(string file){ return initNet(file,Net::None);}
	static Net* initNet(string file,Net::NetType type);
	static void addIn(Net*);
	static void connectAll();
	static void connectNet(Net*);
	static Node* getNode(Net*,int node,int goalStatus);
	static void resetNodes();
	static Net* getNet(unsigned int n);
	static Net* getNet(string name);
	static Net* getNet(Net::NetType);
	static vector<Net*> getAllNets(string name);
	static vector<Net*> getAllNets(Net::NetType type);
	static void learn();
	static void excommunicate(Net*);
	static void excommunicate(Node*);
	static bool fileExists(string);
	static double THRESH_USE;//=.07;//when a node fires, its threshhold decreases   THRESH_USE=CON_SYN* amtOfUsefulConnectionsPerNode * -1;
    static double CONN_SYN;//=.01;//when a node fires, all other nodes firing gain a connection to it
    static double CONN_AND;//=-.002;//when a node fires, all nodes On gain a small connection to it
    static double CONN_NOT;//=-.02;//when a node fires, all nodes Off lose a very small connection to it   CONN_NOT=CONN_AND* avgFiresPerUpdate*-1;
    static double CONN_OPP;//=-.001;//when a node fires, all nodes turning off lose a small connection to it
	
private:
	static vector<Net*> net;
	
};

#endif