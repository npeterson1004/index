//textind.h
#ifndef TEXTIND_H
#define TEXTIND_H
#include <iostream>


using namespace std;

class TextIndex
{
  public:
	TextIndex (int maxKeys = 100, int unique = 1);
	~TextIndex ();
	int Insert (const char * key, int recAddr);
	int Remove (const char * key);
	
	//return the address associated with the parameter key
	int Search (const char * key) const;
	
	//print the index objecy
	void Print (ostream &) const;
	
  protected:
	int MaxKeys;
	int NumKeys; 
	char * * Keys; //array to hold keys
	int * RecAddrs; //array to hold record addresses associated with keys
	
	//returns the index of the parameter key in the Keys[]
	int Find (const char * key) const;
	
	//Allocate two arrays of Keys[] and RecAddrs[]
	int Init (int maxKeys, int unique);
	
	// if true, each key value must be unique in the index
	int Unique; 
friend class TextIndexBuffer;
};

#include "textind.cpp"
#endif

