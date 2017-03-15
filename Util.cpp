#include "Util.h"
#include "Ref.h"
#include <stdlib.h>

#include <time.h>

ofstream Util::out;
bool Util::debugging=true;
int Util::fileNum=0;
Util::Util()
{
}
void Util::setup()
{
	fileNum=0;
	while (remove((char*)("Output/"+preappend(numToString(fileNum),3)+".txt").c_str())==0)
		fileNum++; //deletes all the output files until theres nothing left to delete
	//				wont delete the files if one is deleted: if there are 0,1,2,4,5,6 it wont delete anything after 2
	fileNum=0;
	out.open("Output/000.txt");
	out.clear();
	if (debugging)
		Util::fprint("-------DEBUG-------");
	else
		Util::fprint("-------OUTPUT------");
}
int Util::roughSquareRoot(int x)
{
	if (x<0)
		x*=-1;
	int lastDif=x+50;
	int inc=1;
	if (x>1000)
	{
		inc=10;
		if (x>100000)
		{
			inc=100;
			if (x>1000000)
			{
				inc=1000;
			}
		}
	}
	int halfX=x/2;
	for (int z=0;z<=halfX;z+=inc)
	{
		int mult=z*z;
		int dif=abs(x-mult);
		if (dif>=lastDif)//if the last answer was closer to square than this one, then that must be the closest
		{
			//return the last answer (z- increment)
			return z-inc;
		}
		lastDif=dif;
	}
	//last resort
	fprint("warning! Util::roughSquareRoot- x=",x,false);
	fprint(" increment=",inc,false);
	fprint(". return x/4",false);
	
	return x/4;
}
int Util::stringToInt(std::string let)
{
	if (let.size()>9)
		Util::fprint("possible mistake!: Util::stringToInt let="+let);
	
	int num=0;
    bool neg=false;
    for (unsigned int n=0;n<let.size();n++)
    {
        num*=10;
        if (charToInt(let[n])>=0)
		{
	        num+=charToInt(let[n]);
		}
        else if (let[n]==*"-")
        {
            if (num==0)
            {
                neg=true;
            }
			else
			{
			    return num;
			}
        }
        else
        {
			fprint("possible mistake! Stray character in stringToInt ",false);
			fprint(let);
			fprint();
		    return num;
        }
	}
    num*= (neg ? -1 : 1);
    return num;
}
double Util::stringToNum(std::string let)
{
	//if (let.size()>9)
	//	Util::debug("Util::stringToInt let="+let);
    double num=0;
    int digits=0;
    int dec=0;
    bool neg=false;
    for (unsigned int n=0;n<let.size();n++)
    {
        num*=10;
        digits++;
		if (charToInt(let[n])>=0)
		{
	        num+=charToInt(let[n]);
		}
        else if (let[n]==*".")
        {
            dec=let.size()-digits;
            num/=10;
        }
        else if (let[n]==*"-")
        {
            if (num==0)
            {
                digits=0;
                neg=true;
            }
			else
			{
				if (dec!=0)
		           num/=pow(10.0,dec);
				num*= (neg ? -1 : 1);
			    return num;
			}
        }
        else
        {
			fprint("possible mistake! Stray character in stringToNum ",false);
			fprint(let[n]);
			fprint();
			if (dec!=0)
	            num/=pow(10.0,dec);
			num*= (neg ? -1 : 1);
            return num;
        }
    }
    num/=pow(10.0,dec);
    num*= (neg ? -1 : 1);
    return num;
}
int Util::charToInt(char c)
{
		 if (c==*"1")
	    return 1;
    else if (c==*"2")
		return 2;
    else if (c==*"3")
		return 3;
	else if (c==*"4")
		return 4;
	else if (c==*"5")
		return 5;
	else if (c==*"6")
		return 6;
	else if (c==*"7")
		return 7;
	else if (c==*"8")
		return 8;
	else if (c==*"9")
		return 9;
	else if (c==*"0")
		return 0;
	else if (c==*"-" || c==*".")//both used in numbers
		return -2;              //sortof error value
	else return -1;//error value
}
string Util::numToString(int num)
{
	if (num==0)
		return "0";
	int digit=0;
	string str="";
	bool neg=false;
	if (num<0)
	{
		num*=-1;
		neg=true;
	}
	while (num>0)
	{
		digit=num%10;
		switch (digit)
		{
		case 0:
			str="0"+str;
			break;
		case 1:
			str="1"+str;
			break;
		case 2:
			str="2"+str;
			break;
		case 3:
			str="3"+str;
			break;
		case 4:
			str="4"+str;
			break;
		case 5:
			str="5"+str;
			break;
		case 6:
			str="6"+str;
			break;
		case 7:
			str="7"+str;
			break;
		case 8:
			str="8"+str;
			break;
		case 9:
			str="9"+str;
			break;
		default:
			Util::fprint("ERROR! numToString(int) digit=",digit,false);
			Util::fprint(" Str= "+str+" num=",num);
			digit*=-1;
			break;
		}
		num-= digit;
		num/=10;
	}
	if (neg)
		str="-"+str;
	return str;
}
string Util::numToString(unsigned int num)
{
	if (num==0)
		return "0";
	int digit=0;
	string str="";
	while (num>0)
	{
		digit=num%10;
		switch (digit)
		{
		case 0:
			str="0"+str;
			break;
		case 1:
			str="1"+str;
			break;
		case 2:
			str="2"+str;
			break;
		case 3:
			str="3"+str;
			break;
		case 4:
			str="4"+str;
			break;
		case 5:
			str="5"+str;
			break;
		case 6:
			str="6"+str;
			break;
		case 7:
			str="7"+str;
			break;
		case 8:
			str="8"+str;
			break;
		case 9:
			str="9"+str;
			break;
		default:
			Util::fprint("ERROR! numToString(unsigned int) digit=",digit,false);
			Util::fprint(" Str= "+str+" num=",num);
			digit*=-1;
			break;
		}
		num-= digit;
		num/=10;
	}
	return str;
}
string Util::numToString(double num)
{
	string str="";
	if (num>=0)
	{
		str=numToString((int)num);
	}
	else if (num<0)
	{
		num*=-1;
		str="-"+numToString((int)num);
	}
	else return "0";

	num-=floor(num);
	if (num==0)
		return str;
	str.push_back(*".");
	int digit=0;

	while (num>0)
	{
		num*=10;
		digit=(int)(num);
		if (num-digit>=.999)
			digit++; //to remove some floating point errors
		else if (num-digit<=.001)
			num=digit;
		switch (digit)
		{
		case 0:
			str.push_back(*"0");
			break;
		case 1:
			str.push_back(*"1");
			break;
		case 2:
			str.push_back(*"2");
			break;
		case 3:
			str.push_back(*"3");
			break;
		case 4:
			str.push_back(*"4");
			break;
		case 5:
			str.push_back(*"5");
			break;
		case 6:
			str.push_back(*"6");
			break;
		case 7:
			str.push_back(*"7");
			break;
		case 8:
			str.push_back(*"8");
			break;
		case 9:
			str.push_back(*"9");
			break;
		default:
			Util::fprint("ERROR! numToString(double) digit=",digit,false);
			Util::fprint(" Str= "+str+" num=",num);
			break;
		}
		num-=digit;
	}
	return str;
}
string Util::preappend(string str,unsigned int length)
{
	string ret=str;
	while (ret.length()<length)
	{
		ret="0"+ret;
	}
	return ret;
}
string Util::truncate(string str,unsigned int length)
{
	if (str.size()<=length)
		return str;
	unsigned int dec=str.find(".");
	if (dec!=string::npos)
	{
		if (str.size()-dec>=length)
		{
			return str.substr(0,length);
		}
		else if (dec<length)
		{
			return str.substr(0,length);
		}
		else
		{
			str=str.substr(0,dec-1);
		}
	}
	//shouldnt have any decimal by now
	double pow=str.size();
	str.insert(1,".");
	unsigned int powLength;
	if (pow>1)
		powLength=(unsigned int)ceil(log10(pow));
	else powLength=1;
	if (length<=powLength+2)//if the whole thing would be taken up by the .E1
		return str.substr(0,length);
	if (pow<=3)//if the power is <3, ie could you fit the last 3 digits in the place of the .E3
		return str.substr(0,length);

	str=str.substr(0,length-(1+powLength));
	str=str+"E";//scientific notation
	str=str+numToString((int)pow);
	return str;
}
string Util::makeLength(string str,unsigned int length)
{
	if (str.size()>length)
		return truncate(str,length);
	else if (str.size()<length)
		return preappend(str,length);
	else return str;
}
BWAPI::TilePosition Util::posToTile(BWAPI::Position pos)
{
	BWAPI::TilePosition tile=BWAPI::TilePosition(pos.x()/32,pos.y()/32);
	return tile;
}
int Util::getWeaponRange(BWAPI::UnitType* type)
{
	int mostRange=0;
	mostRange=type->airWeapon().maxRange();
	if (mostRange<type->groundWeapon().maxRange())
		mostRange=type->groundWeapon().maxRange();

	if (type->isSpellcaster())
	{
		std::set<BWAPI::TechType> magic=type->abilities();
		for (std::set<BWAPI::TechType>::iterator spell=magic.begin();spell!=magic.end();spell++)
		{
			if (spell->getWeapon()!=BWAPI::WeaponTypes::None)
			{
				if (mostRange<spell->getWeapon().maxRange())
					mostRange=spell->getWeapon().maxRange();
			}
		}
	}
	return mostRange;
}
int Util::roughDist(BWAPI::Position pos,BWAPI::Position pawz) {
	return roughDist(pos.x(),pos.y(),pawz.x(),pawz.y());
}
int Util::roughDist(BWAPI::Position pos,int x,int y) {
	return roughDist(pos.x(),pos.y(),x,y);
}
int Util::roughDist(int x,int y,int eks,int wi) {
	return (x-eks + y-wi);
}
double Util::distance(int x,int y,int eks,int wi) {
	return sqrt(pow(double(x-eks),2)+pow(double(y-wi),2));
}
double Util::sqrdDistance(int x,int y,int eks,int wi) {
	return pow(double(x-eks),2)+pow(double(y-wi),2);
}
void Util::checkFileLength()
{
	ifstream readout;
	fprint("address=",false);
	readout.open((char*)("Output/"+preappend(numToString(fileNum),3)+".txt").c_str());
	long begin=readout.tellg();//get the size of the file {
    readout.seekg(0,ios::end);
    long end=readout.tellg();
    int size=(int)(end-begin);//}
	Util::fprint("Size = ",size);
	if (size>=100000000)//100,000,000(one billion)=~100 mb
	{
		fprint("To be continued in #",fileNum,"...");
		out.close();
		fileNum++;
		out.open((char*)("Output/"+preappend(numToString(fileNum),3)+".txt").c_str());
		out.clear();
		if (debugging)
			out<<"------------DEBUG #"<<fileNum<<"------------"<<endl;
		else out<<"-----------OUTPUT #"<<fileNum<<"-------------"<<endl;
		out<<"At update "<<Ref::updates()<<endl;
		out<<"Game time = "<<BWAPI::Broodwar->elapsedTime()<<endl;

	}
}
//------Fprint--------------
void Util::fprint(std::string text) {
	out<<text<<endl;
}
void Util::fprint(char let[]) {
	out<<let<<endl;
}
void Util::fprint(char let[],bool noEnter) {
	out<<let;
}
void Util::fprint(char let[],int i) {
	out<<let<<i<<endl;
}
void Util::fprint(char let[],int x,int y) {
	out<<let<<x<<", "<<y<<endl;
}
void Util::fprint(char let[],int i,char word[],int x) {
	out<<let<<i<<word<<x<<endl;
}
void Util::fprint(char let[],char c){
	out<<let<<c<<endl;
}
void Util::fprint(char let[],char* c) {
	out<<let<<c<<endl;
}
void Util::fprint(char let[],unsigned int ui) {
	out<<let<<ui<<endl;
}
void Util::fprint(char let[],double d) {
	out<<let<<d<<endl;
}
void Util::fprint(char let[],void* ptr) {
	out<<let<<ptr<<endl;
}
void Util::fprint(char let[],const void * ptr) {
	out<<let<<ptr<<endl;
}
void Util::fprint(const char let[]) {
	out<<let<<endl;
}
void Util::fprint(string text, bool noEnter) {
	out<<text;
}
void Util::fprint(char let[],int i,bool noEnter) {
	out<<let<<i;
}
void Util::fprint(char let[],double d,bool noEnter) {
	out<<let<<d;
}
void Util::fprint(char let[],char c,bool noEnter) {
	out<<let<<c;
}
void Util::fprint(string let,int i,string word) {
	out<<let<<i<<word<<endl;
}
void Util::fprint(string word,int i,string text,int n) {
	out<<word<<i<<text<<n<<endl;
}
void Util::fprint(string text,int num) {
	out<<text<<num<<endl;
}
void Util::fprint(string text,unsigned int num) {
	out<<text<<num<<endl;
}
void Util::fprint(string text,double num) {
	out<<text<<num<<endl;
}
void Util::fprint(string text,char c) { 
	out<<text<<c<<endl;
}
void Util::fprint(string text,void* ptr){
	out<<text<<ptr<<endl;
}
void Util::fprint(string text,int num,bool noEnter) {
	out<<text<<num;
}
void Util::fprint(string text,double num,bool noEnter) {
	out<<text<<num;
}
void Util::fprint(char c,bool noEnter) {
	out<<c;
}
void Util::fprint(int write) {
	out<<write;
}
void Util::fprint(unsigned int write) {
	out<<write;
}
void Util::fprint(double d) {
	out<<d;
}
void Util::fprint(long l) {
	out<<l;
}
void Util::fprint(char c) {
	out<<c;
}
void Util::fprint(void* ptr) {
	out<<ptr;
}
void Util::fprint(bool b) {
	if (b) out<<"true";
	else out<<"false";
}
void Util::fprint() {
	out<<" "<<endl;
}
//-----------DEBUGGERS--------------

void Util::debug(std::string text) {
	if (debugging) out<<text<<endl;
}
void Util::debug(char let[]) {
	if (debugging) out<<let<<endl;
}
void Util::debug(char let[],bool noEnter) {
	if (debugging) out<<let;
}
void Util::debug(char let[],int i) {
	if (debugging) out<<let<<i<<endl;
}
void Util::debug(char let[],int x,int y) {
	if (debugging) out<<let<<x<<", "<<y<<endl;
}
void Util::debug(char let[],int i,char word[],int x) {
	if (debugging) out<<let<<i<<word<<x<<endl;
}
void Util::debug(char let[],char c){
	if (debugging) out<<let<<c<<endl;
}
void Util::debug(char let[],char* c) {
	if (debugging) out<<let<<c<<endl;
}
void Util::debug(char let[],unsigned int ui) {
	if (debugging) out<<let<<ui<<endl;
}
void Util::debug(char let[],double d) {
	if (debugging) out<<let<<d<<endl;
}
void Util::debug(char let[],void* ptr) {
	if (debugging) out<<let<<ptr<<endl;
}
void Util::debug(char let[],const void * ptr) {
	if (debugging) out<<let<<ptr<<endl;
}
void Util::debug(const char let[]) {
	if (debugging) out<<let<<endl;
}
void Util::debug(string text, bool noEnter) {
	if (debugging) out<<text;
}
void Util::debug(char let[],int i,bool noEnter) {
	if (debugging) out<<let<<i;
}
void Util::debug(char let[],double d,bool noEnter) {
	if (debugging) out<<let<<d;
}
void Util::debug(char let[],char c,bool noEnter) {
	if (debugging) out<<let<<c;
}
void Util::debug(string word,int i,string text,int n) {
	if (debugging) out<<word<<i<<text<<n<<endl;
}
void Util::debug(string text,int num) {
	if (debugging) out<<text<<num<<endl;
}
void Util::debug(string text,unsigned int num) {
	if (debugging) out<<text<<num<<endl;
}
void Util::debug(string text,double num) {
	if (debugging) out<<text<<num<<endl;
}
void Util::debug(string text,char c) { 
	if (debugging) out<<text<<c<<endl;
}
void Util::debug(string text,void* ptr){
	if (debugging) out<<text<<ptr<<endl;
}
void Util::debug(string text,int num,bool noEnter) {
	if (debugging) out<<text<<num;
}
void Util::debug(string text,double num,bool noEnter) {
	if (debugging) out<<text<<num;
}
void Util::debug(char c,bool noEnter) {
	if (debugging) out<<c;
}
void Util::debug(int write) {
	if (debugging) out<<write;
}
void Util::debug(unsigned int write) {
	if (debugging) out<<write;
}
void Util::debug(double d) {
	if (debugging) out<<d;
}
void Util::debug(long l) {
	if (debugging) out<<l;
}
void Util::debug(char c) {
	if (debugging) out<<c;
}
void Util::debug(void* ptr) {
	if (debugging) out<<ptr;
}
void Util::debug(bool b) {
	if (b) if (debugging) out<<"true";
	else if (debugging) out<<"false";
}
void Util::debug() {
	if (debugging) out<<" "<<endl;
}
Util::~Util(void)
{
}
