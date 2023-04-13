#include <iostream>
#include "textind.h"
#include <fstream>
#include <sstream>
#include <string>
#include <cstring>



using namespace std;


TextIndex::TextIndex(int maxKeys, int unique) : NumKeys(0), Keys(0), RecAddrs(0)
{
    Init(maxKeys, unique);
}

TextIndex::~TextIndex()
{
    delete Keys;
    delete RecAddrs;
}

int TextIndex::Insert(const char * key, int recAddr)
{
    int i;
    int index = Find(key);
    if(Unique && index >=0) return 0;
    if(NumKeys == MaxKeys) return 0;

    for(i = NumKeys-1; i >= 0; i--)
    {
        if(strcmp(key, Keys[i]) > 0) break;
        Keys[i+1] = Keys[i];
        RecAddrs[i+1] = RecAddrs[i];
    }

    Keys[i+1] = strdup(key);
    RecAddrs[i+1] = recAddr;
    NumKeys++;
    return 1;
}


int  TextIndex::Remove(const char *key)
{
    int index = Find(key);
    if(index < 0) return 0;
    for (int i = index; i < NumKeys; i++)
    {
        Keys[i] = Keys[i+1];
        RecAddrs[i] = RecAddrs[i+1];
    }
    NumKeys--;
    return 1;

}



int  TextIndex::Search(const char * key) const
{
    int index = Find(key);
    if(index < 0 ) return index;
    return RecAddrs[index];
}


void  TextIndex::Print(ostream & stream) const
{
    stream << "max keys:" << MaxKeys << endl;
    stream << "num keys:" << NumKeys << endl;
}


int TextIndex::Find(const char * key) const
{
    for(int i = 0; i < NumKeys; i++)
    {
        if (strcmp(Keys[i], key) == 0) return i;
        else if (strcmp(Keys[i], key) > 0) return -1;
    }
    return -1;
}

int TextIndex::Init(int maxKeys, int unique)
{
    Unique = unique !=0;
    if(maxKeys <= 0)
    {
        maxKeys = 0;
        return 0;
    }
    MaxKeys = maxKeys;
    Keys = new char *[maxKeys];
    RecAddrs = new int [maxKeys];
    return 1;
}

