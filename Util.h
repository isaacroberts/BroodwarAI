#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include <math.h>
#include "BWAPI.h"

using namespace std;



class Util
{
public:
	Util();
	static void setup();
	static int roughSquareRoot(double x)
		{	return roughSquareRoot((int)x);  }
	static int roughSquareRoot(int);
	static BWAPI::TilePosition posToTile(BWAPI::Position);
	static int getWeaponRange(BWAPI::UnitType type) {return getWeaponRange(&type);}
	static int getWeaponRange(BWAPI::UnitType*);
	static int roughDist(BWAPI::Position,int,int);
	static int roughDist(BWAPI::Position,BWAPI::Position);
	static int roughDist(int,int,int,int);
	static double distance(BWAPI::Position pos,BWAPI::Position paws) {return distance(pos.x(),pos.y(),paws.x(),paws.y());}
	static double distance(int,int,int,int);
	static double sqrdDistance(BWAPI::Position p,BWAPI::Position b) {return sqrdDistance(p.x(),p.y(),b.x(),b.y());}
	static double sqrdDistance(int,int,int,int);
	static int stringToInt(std::string);
	static double stringToNum(std::string);
	static int charToInt(char);
	static string numToString(int);
	static string numToString(double num);
	static string numToString(unsigned int);
	static string preappend(string,unsigned int length);
	static string truncate(string,unsigned int length);
	static string makeLength(string,unsigned int length);
	static void checkFileLength();

	static void fprint(string text);
	static void fprint(char[]);
	static void fprint(char[],int);
	static void fprint(char[],int,int);
	static void fprint(char[],int,char[],int);
	static void fprint(char[],char);
	static void fprint(char[],char*);
	static void fprint(char[],unsigned int);
	static void fprint(char[],double);
	static void fprint(char[],void*);
	static void fprint(char[], const void *);
	static void fprint(char[],bool noEnter);
	static void fprint(char[],int,bool noEnter);
	static void fprint(char[],double,bool noEnter);
	static void fprint(char[],char,bool noEnter);
	static void fprint(const char[]);
	static void fprint(string,bool noEnter);
	static void fprint(string,int);
	static void fprint(string,unsigned int);
	static void fprint(string,double);
	static void fprint(string,void*);
	static void fprint(string,int,string,int);
	static void fprint(string,int,string);
	static void fprint(string,int,bool noEnter);
	static void fprint(string,double,bool noEnter);
	static void fprint(string,char);
	static void fprint(char,bool noEnter);
	static void fprint(int);
	static void fprint(bool);
	static void fprint(unsigned int);
	static void fprint(double);
	static void fprint(char);
	static void fprint(long);
	static void fprint(void*);
	static void fprint();

	static void debug(string text);
	static void debug(char[]);
	static void debug(char[],int);
	static void debug(char[],int,int);
	static void debug(char[],int,char[],int);
	static void debug(char[],char);
	static void debug(char[],char*);
	static void debug(char[],unsigned int);
	static void debug(char[],double);
	static void debug(char[],void*);
	static void debug(char[], const void *);
	static void debug(char[],bool noEnter);
	static void debug(char[],int,bool noEnter);
	static void debug(char[],double,bool noEnter);
	static void debug(char[],char,bool noEnter);
	static void debug(const char[]);
	static void debug(string,bool noEnter);
	static void debug(string,int);
	static void debug(string,unsigned int);
	static void debug(string,double);
	static void debug(string,void*);
	static void debug(string,int,string,int);
	static void debug(string,int,bool noEnter);
	static void debug(string,double,bool noEnter);
	static void debug(string,char);
	static void debug(char,bool noEnter);
	static void debug(int);
	static void debug(bool);
	static void debug(unsigned int);
	static void debug(double);
	static void debug(char);
	static void debug(long);
	static void debug(void*);
	static void debug();
	~Util(void);
	static bool debugging;
	static int fileNum;
private:
	static ofstream out;
};
