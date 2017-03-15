#include <iostream>
#include <fstream>

#include "NetHandler.h"

using namespace std;

vector<Net*> NetHandler::net=vector<Net*>();

NetHandler::NetHandler()
{
}
Net* NetHandler::initNet(string file,Net::NetType type)
{
	file=(string)("Nets/")+file;
	
	char *address=new char[file.size()];
	address=(char*)file.c_str();

	ifstream mem;
	mem.open(address);

	if (mem.fail())
	{
		//then mem.fail(). but screw your file to the sticking-place, and mem'll not fail.
		Util::fprint("ERROR! nHand initNet(): File failed: "+file);
		mem.close();
		return new Net();
	}
	long begin=mem.tellg();//get the size of the file
	mem.seekg(0,ios::end);
	long end=mem.tellg();
	int size=(int)(end-begin);// /sizeof(char); (sizeof(char) = 1)
    mem.seekg(0,ios::beg);
	char *t= new char[size+1];
	mem.getline(t,size+1);
	Net* brain=new Net(type);
	brain->setName(file.substr(5,file.size()-9));
	
	if (t[0]==*"#")
	{
		string val="";
		int n=1;
		while (t[n]!=*"{" && n<size)
        {		
			val.push_back(t[n]);
			n++;
		}
		brain->createNodes(Util::stringToInt(val));
	}
	else 
	{
		Util::fprint("ERROR!: net "+file+" does not have a # as the first symbol");
	}
	//delete t;
	net.push_back(brain);
	return brain;
}
void NetHandler::connectAll()
{
	for (unsigned int n=0;n<net.size();n++)
	{
		//Util::debug("connecting "+net[n]->getName());
		connectNet(net[n]);
		net[n]->initialize();
	}
}
void NetHandler::addIn(Net* newt)// new+net=newt. get it?
{//this function assumes that the net has already been initted and connected. it just needs to be given 
	//outputs to other nets, which has to be done by going through their files and looking for connections to this net
	for (unsigned int n=0;n<net.size();n++)
	{
		if (net[n]->getID()!=newt->getID())
		{
			string file=net[n]->getName();
			file=(string)("Nets/")+file+(string)".txt";
			char *address=new char[file.size()];
			address=(char*)file.c_str();
			ifstream mem;
			mem.open(address);
			if (mem.fail())
			{
				//then mem.fail(). but screw your file to the sticking place and mem'll not fail.
				Util::fprint("ERROR! nHand addIn(): File failed: "+file);
				return;
			}
			long begin=mem.tellg();//get the size of the file {
			mem.seekg(0,ios::end);
			long end=mem.tellg();
			int size=(int)(end-begin)/sizeof(char);//}
			mem.seekg(0,ios::beg);
			char *t=new char[size+1];
			mem.getline(t,size+1);
			int curNode=-1;
			for (int l=0;l<size;l++)
			{
				if (t[l]==*"/"||t[l]==*">")
				{
					curNode++;
				}
				if (t[l]==*"'")
				{
					l++;
					string name="";
					int apostIx=0;
					for (l;l<size;l++)
					{
						if (t[l]!=*"'")
						{
							name.push_back(t[l]);
						}
						else 
						{
							apostIx=l+1;
							l++;
							break;
						}
					}
			//		Util::debug("nethand addin() name found:"+name);
			//		Util::debug("	 -t[apostIx]="+t[apostIx]);
					l++;
			//		Util::debug("   -t[l]="+t[l]);
					string connVal="";
					for (l;l<size;l++)
					{
						if (t[l]==*":")
						{
							l++;
							break;
						}
						else connVal.push_back(t[l]);
					}
					int connTo=Util::stringToInt(connVal);
					string weightVal="";
					if (name==newt->getName())
					{
						for (l;l<size;l++)
						{
							if (t[l]==*")"|| t[l]==*"<")
							{
								l++;
								break;
							}
							else weightVal.push_back(t[l]);
						}
						double weight=Util::stringToNum(weightVal);//the connTo number is everything after the func symbol
						if (weightVal=="cond"||weightVal=="notcond")
						{
							Util::fprint("ERROR! The weightval from "+net[n]->getName()+" to "+newt->getName()+" is "+weightVal+". The reader cant handle that.");
						}
						else if (t[apostIx]==*"&")//if the func symbol is 'add'
						{//meaning all conns should be at full, adding them
							net[n]->getNode(curNode)->addConn(newt->getNode(connTo),weight);
						}
						else if (t[apostIx]==*"%")//the func symbol for avg
						{
							net[n]->getNode(net[n]->getNode(connTo)->getName()+"_avgr")->addConn(newt->getNode(curNode),weight);
						}
						else if (t[apostIx]==*"^")//the func symbol for max
						{
							net[n]->getNode(net[n]->getNode(connTo)->getName()+"_maxer")->addConn(newt->getNode(curNode),weight);	
						}//to if (max)
						else
						{
							Util::fprint("WARNING! unexpected combine symbol: t[apostIx]=",t[apostIx]);
						}
					}//to if (name=newt->name
				}//to if t[l]=='
			}//to for (l<size)
		}//to if net[n]!=newt
	}//to for(nets)
}//to addIn(Net* newt)
void NetHandler::connectNet(Net* curNet) 
{
	string file=curNet->getName();
	//Util::debug("    connecting "+file);
	file=(string)("Nets/")+file+(string)".txt";
    //get the text from the file
    char *address=new char[file.size()];
	address=(char*)file.c_str();

	ifstream mem;
	mem.open(address);

	if (mem.fail())
	{
		//then mem.fail(). but screw your file to the sticking place and mem'll not fail.
		Util::fprint("nHand connectNet(): File failed: "+file);
		return;
	}
	//get files size
	long begin=mem.tellg();
    mem.seekg(0,ios::end);
    long end=mem.tellg();
    int size=(int)(end-begin);///sizeof(char); (sizeof(char)= 1 )
    mem.seekg(0,ios::beg);
	char *t=new char[size+1];
	mem.getline(t,size+1);
	
    unsigned int curNode=-1;
	if (curNet==NULL)
	{
		Util::fprint("ERROR! nHand: curNet ("+file+") doesnt exist");
		return;
	}
	if (curNet->nodeAmt()==0)
	{
		Util::fprint("WARNING! curNet "+file+" is empty");
		return;
	}
	enum GoalStatus {
		Not=0,Drive=1,Meet=2,Strive=3,Over=4
	};
	GoalStatus status=Not;
	for (int n=0;n<size;n++)
	{
/*		if (t[n]!=*" ")
		{
		Util::debug("n=",n,false);
		Util::debug("       t[n]= ",(t[n]));
		}
*/		if (t[n]==*"[")
		{//how to comment things out in the textfile
			n++;
	//		Util::debug("Comment: ",false);
			while (t[n]!=*"]" && n<size)
			{
	//			Util::debug(t[n],false);
				n++;
			}
	//		Util::debug();
		}
		else if (t[n]==*"/")
        {//the ID setter
            curNode++;
			if (curNode>=curNet->nodeAmt())
			{
				Util::fprint("WARNING! not enough nodes in "+file+". need ",curNode+1);
				curNet->addNode(new Node());
			}
			string val="";
			n++;
			while (Util::charToInt(t[n])>=0)// !=( -2 or -1)
			{
				val.push_back(t[n]);
				n++;
			}
			n--;
			if (Util::stringToInt(val)!=curNode)
				Util::fprint("WARNING! the ID after the / ("+val+") does not equal the actual ID :",curNode);
		//	Util::debug("curnode =",curNode);
		}
		else if (t[n]==*">")
		{
			curNode++;
			if (curNode>=curNet->nodeAmt())
			{
				Util::fprint("WARNING! not enough nodes in "+file+". need ",curNode+1);
				curNet->addNode(new Goal());
			}
			else
			{
				curNet->replace(curNode,new Goal());
				curNet->getNode(curNode)->setNetname(curNet->getName());
			}
		//	Util::debug("curnode =",curNode);
			status=Drive;
		}
		else if (t[n]==*"\\")
		{
			status=(GoalStatus)((int)status+1);
			if (status==Over)
				status=Not;
		}
		else if (t[n]==*"*")
		{
			if (status==Strive|| status==Meet)
			{
				n++;
				string val="";
				while (t[n]!=*"="&& t[n]!=*","&& t[n]!=*"\""&& t[n]!=*"|" && t[n]!=*"(" && t[n]!=*"~")
				{
					val.push_back(t[n]);
					n++;
				}
				if (status==Strive)
					((Goal*)curNet->getNode(curNode))->setStriveWeight(Util::stringToNum(val));
				else
					((Goal*)curNet->getNode(curNode))->setMeetWeight(Util::stringToNum(val));
				n--;
			}
		}
		else if (t[n]==*"=")
		{//job
			n++;
			string val="";
			while (Util::charToInt(t[n])!=-1)
            {
				val.push_back(t[n]);
                n++;
            }
			getNode(curNet,curNode,status)->setJob(Util::stringToInt(val));
			n--;
		}
		else if (t[n]==*",")
		{
			n++;
			string val="";
			while (Util::charToInt(t[n])!=-1)
			{
				val.push_back(t[n]);
				n++;
			}
			getNode(curNet,curNode,status)->setType(Util::stringToInt(val));
			n--;
		}
        else if (t[n]==*"\"")
        {
            n++;//skips over first "
            string name="";
            while (t[n]!=*"\"" && n<size)
            {
				if (t[n]==*"/"|| t[n]==*"(" || t[n]==*"<")
					Util::fprint("ERROR! in netHand::connectNet("+curNet->getName()+" name of ",(int)curNode," messing up   n=",n);
                name.push_back(t[n]);
                n++;
            }
            getNode(curNet,curNode,status)->setName(name);
		}
        else if (t[n]==*"|")
        {
            n++;
            string val="";
			while (Util::charToInt(t[n])!=-1)
            {
                val.push_back(t[n]);
                n++;
            }
            double thresh=Util::stringToNum(val);
            getNode(curNet,curNode,status)->setThresh(thresh);
			n--;
		}
        else if (t[n]==*"~")
        {
            n++;
            string val="";
            while (t[n]!=*"(")
            {
                val.push_back(t[n]);
                n++;
            }
            double miscMod=Util::stringToNum(val);
		//	Util::debug("node "+getNode(curNet,curNode,status)->getName()+" miscMod= "+val);
			getNode(curNet,curNode,status)->setMiscMod(miscMod);
			n--;
		}
        else if (t[n]==*"(")
		{
            n++;
            while (t[n]!=*")"&& n<size)
			{
				if (t[n]==*" ")
					n++;
                if (t[n]==*"<")
				{
					n++;
                    string val="";
					while (t[n]!=*":"&& t[n]!=*"<" )
                    {
                        val.push_back(t[n]);
                        n++;
                    }
					n++;
					//get the connection weight first, so that connecting can be dynamic
                    string weightVal="";
                    while (t[n]!=*"<"&&t[n]!=*")" && t[n]!=*"[")
                    {
                        weightVal.push_back(t[n]);
                        n++;
                    }
					if (t[n]==*"[")
					{
					//	Util::debug("Comment: ",false);
						while (t[n]!=*"]" && n<size)
						{
					//		Util::debug(t[n],false);
							n++;
						}
						n++;
					//	Util::debug();
					}
					int connTo=0;
					if (val.find("'")!=string::npos)//if the conn needs to go across nets
					{
						double weight=0;
						if (weightVal!="cond"&&weightVal!="notcond")
							weight=Util::stringToNum(weightVal);
						string name="";
						int apostIx=0;
						for (unsigned int m=1;m<val.size();m++)
						{
							if (val[m]!=*"'")
							{
								name.push_back(val[m]);
							}
							else 
							{
								apostIx=m;
								m=val.size();
							}
						}
						vector<Net*> toConn=getAllNets(Net::stringToNetType(name));
						apostIx++;//the combine symbol is whats after the apostrophe

						if (val[apostIx]==*"@")
						{
							string netName="";
							while (val[apostIx]!=*"^" && val[apostIx]!=*"%" && val[apostIx]!=*"&")
							{
								netName.push_back(val[apostIx]);
								apostIx++;
							}
							for (unsigned int n=0;n<toConn.size();n++)
							{
								if (toConn[n]->getName()!=netName)
								{
									toConn.erase(toConn.begin()+n);
									n--;//otherwise it will skip over the one after n
								}
							}
						}
						//the char after the apostrophe is the combine symbol
						//the combine symbol is how to handle connections to multiple networks
						connTo=Util::stringToInt(val.substr(apostIx+1));//the connTo number is everything after the combine symbol
						
						Node* combine=NULL;
						if (val[apostIx]==*"^")
						{
							combine=new Node();//a new node to pass on the maximum value
							combine->setType(11);//maxer=type
							combine->setJob(8);//calc=job
							combine->setName(getNode(curNet,curNode,status)->getName()+"_maxer");
						}
						else if (val[apostIx]==*"%")
						{
							combine=new Node();
							combine->setType(13);//type=average
							combine->setJob(8);//job=calc
							combine->setName(getNode(curNet,curNode,status)->getName()+"_avgr");
						}
						if ((weightVal=="cond" || weightVal=="notcond"))
						{
							if (toConn.size()==1)
							{
								getNode(curNet,curNode,status)->setCondConn(toConn[0]->getNode(connTo));
								getNode(curNet,curNode,status)->setCondConnIsNot(weightVal=="notcond");
							}
							else 
							{
								Util::fprint((string)"ERROR!: tried to connect a cond to ",toConn.size()," nodes.  curNet="+curNet->getName()+" curNode=",curNode);
								getNode(curNet,curNode,status)->setCondConn(toConn[0]->getNode(connTo));
								getNode(curNet,curNode,status)->setCondConnIsNot(weightVal=="notcond");
							}
						}
						for (unsigned int g=0;g<toConn.size();g++)
						{
							if (val[apostIx]==*"&")//if the combine symbol is 'add'
							{//meaning all conns should be at full, adding them
								getNode(curNet,curNode,status)->addConn(toConn[g]->getNode(connTo),weight);
							}
							else if (val[apostIx]==*"%")//the combine symbol for avg
							{
								combine->addConn(toConn[g]->getNode(connTo),weight);
							}
							else if (val[apostIx]==*"^")//the combine symbol for max
							{
								combine->addConn(toConn[g]->getNode(connTo),weight);
							}
						}
						if (val[apostIx]==*"^")
						{//if combine symbol is max
							curNet->addNode(combine);
							getNode(curNet,curNode,status)->addConn(combine,1.0);
						}
						else if (val[apostIx]==*"%")
						{
							curNet->addNode(combine);
							getNode(curNet,curNode,status)->addConn(combine,1.0);
						}
					}//to if connVal has ' in it
					else
					{
						if (weightVal=="cond")
						{
							getNode(curNet,curNode,status)->setCondConn(curNet->getNode(connTo));
						}
						else if (weightVal=="notcond")
						{
							getNode(curNet,curNode,status)->setCondConn(curNet->getNode(connTo));
							getNode(curNet,curNode,status)->setCondConnIsNot(true);
						}
						else
						{
							connTo=Util::stringToInt(val);
							getNode(curNet,curNode,status)->addConn(curNet->getNode(connTo),Util::stringToNum(weightVal)); 
						}
					} //to if (connToVal does not have ' in it) 
				}//to if (t[n]='<')
                else
				{
					Util::fprint("skip ",n,false);
					Util::fprint(" ",t[n]);
					n++;
				}
			}//to while (t[n]!=')'
		}//to if (t[n]='(')
        else if (t[n]==*"}")
        {
			if (n!=size-1)
			{
				Util::fprint("WARNING! File } caused nethand to ignore:  ");
				for (n;n<size;n++)
					Util::fprint(t[n],false);
				Util::fprint();
			}
			n=size+1;
        }
	}//to for(n<t.size)
	return;
}
Node* NetHandler::getNode(Net *net, int node, int goalStatus)
{
	if (goalStatus==0)//no goals involved
		return net->getNode(node);
	if (goalStatus==1)//drive
		return net->getNode(node);
	else if (goalStatus==2)//meet
		return ((Goal*)net->getNode(node))->getMeet();
	else if (goalStatus==3)//strive
		return ((Goal*)net->getNode(node))->getStrive();
	else return net->getNode(node);
}
void NetHandler::resetNodes()
{
	for (unsigned int n=0;n<net.size();n++)
	{
		net[n]->resetNodes();
	}
}
void NetHandler::learn()//must be done after updating
{
	/*
    vector<Node*> justFired;
	for (unsigned int m=0;m<net.size();m++)
	{
    for (int n=0;n<net[m]->nodeAmt();n++)
    {
        if (net[m]->getNode(n)->justFired())
        {//if node[n] just fired
			justFired.push_back(net[m]->getNode(n));
			if (net[m]->getNode(n)->isEditable())
                net[m]->getNode(n)->learnThresh(THRESH_USE);
			for (int c=0;c<net[m]->nodeAmt();c++)
            {
				if (Net::canConnect(net[m]->getNode(n),net[m]->getNode(c)))
                {//^ a chart of what types should connect to what
                    if (net[m]->getNode(c)->getState())
                    {
                        net[m]->getNode(c)->learnWeight(net[m]->getNode(n),CONN_AND* net[m]->getNode(n)->getState());
                    }
                    else
                    {
                        net[m]->getNode(c)->learnWeight(net[m]->getNode(n),CONN_NOT* net[m]->getNode(n)->getState());
                    }
                }
            }
        }
    }
	}
    for (unsigned int f=0;f<justFired.size();f++)
    {
        for (unsigned int c=0;c<justFired.size();c++)
        {
			if (Net::canConnect(justFired[c],justFired[f]))
            {
                justFired[c]->learnWeight(justFired[f],CONN_SYN*justFired[f]->getState());
            }
        }
    }
	//change this later
    for (int n=0;n<net[0]->nodeAmt()/*change later*//*;n++)
   {
		if (net[0]->getNode(n)->justTurnedOff())
        {//if node[n] just turned off
            for (unsigned int f=0;f<justFired.size();f++)
            {//all node that just fired decrease connection to this one
				if (Net::canConnect(net[0]->getNode(f),net[0]->getNode(n)))
	                net[0]->getNode(f)->learnWeight(net[0]->getNode(n),CONN_OPP);
            }
        }
    }
*/
}
void NetHandler::excommunicate(Net* blasphemers)
{//deletes a net
	int whichAmongUs=-1;
	for (unsigned int n=0;n<net.size();n++)
	{
		if (net[n]->getID()!=blasphemers->getID())
		{
			for (unsigned int m=0;m<blasphemers->nodeAmt();m++)
			{
				net[n]->excommunicate(blasphemers->getNode(m));
			}
		}
		else whichAmongUs=n;
	}
	delete blasphemers;
	net.erase(net.begin()+whichAmongUs);
}
void NetHandler::excommunicate(Node* blasphemer)
{
	for (unsigned int n=0;n<net.size();n++)
	{
		net[n]->excommunicate(blasphemer);
	}
}
bool NetHandler::fileExists(string file)
{
	file=(string)("Nets/")+file;
    char *address=new char[file.size()];
	address=(char*)file.c_str();
	ifstream mem;
	mem.open(address);
	return !mem.fail();
}
Net* NetHandler::getNet(unsigned int n)
{
	if (n<net.size())
		return net[n];
	else 
	{
		Util::fprint("ERROR! netHand: asked for net out of bounds:",n);
		return new Net();
	}
}
Net* NetHandler::getNet(Net::NetType type)
{
	for (unsigned int n=0;n<net.size();n++)
	{
		if (net[n]->getType()==type)
		{
			return net[n];
		}
	}
	Util::fprint("WARNING! nHand: No net found of type"+type);
	return new Net();
}
Net* NetHandler::getNet(string name)
{
	for (unsigned int n=0;n<net.size();n++)
	{
		if (net[n]->getName()==name)
		{
			return net[n];
		}
	}
	Util::fprint("WARNING! nHand: No net found by the name of "+name);
	return new Net();
}
vector<Net*> NetHandler::getAllNets(Net::NetType type)
{
	vector<Net*> rightType=vector<Net*>();
	for (unsigned int n=0;n<net.size();n++)
	{
		if (net[n]->getType()==type)
		{
			rightType.push_back(net[n]);
		}
	}
	if (rightType.size()==0)
	{
		Util::fprint("WARNING!: nhand getAllNets(NetType type=",type,false);
		Util::fprint(") none found");
	}
	return rightType;
}
vector<Net*> NetHandler::getAllNets(string name)
{
	vector<Net*> haveName=vector<Net*>();
	for (unsigned int n=0;n<net.size();n++)
	{
		if (net[n]->getName()==name)
		{
			haveName.push_back(net[n]);
		}
	}
	if (haveName.size()==0)
		Util::fprint("WARNING! nhand getAllNets("+name+"): none found");
	return haveName;
}
