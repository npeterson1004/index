//testind.cc
#include "textind.h"
#include "delim.h"
#include "buffile.h"
#include "recording.h"
#include "indfile.h"
#include <iostream>

// make an indexed file from a recording file: myfile
int IndexRecordingFile (char * myfile, 
	TextIndexedFile<Recording> & indexFile)
{
	Recording rec; int recaddr, result;
	DelimFieldBuffer Buffer; // create a buffer
	BufferFile RecFile(Buffer); 
	result = RecFile . Open (myfile,ios::in);//to read from this file
	if (!result)
	{
		cout << "Unable to open file "<<myfile<<endl;
		return 0;
	}
	while (1) // loop until the read fails
	{
		recaddr = RecFile . Read (); // read next record
		if (recaddr < 0) break;
		rec. Unpack (Buffer);
		indexFile . Append(rec); 
		//both data file and associated index file are upadted. 
		//These two files are created by the method Create in
		//the class TextIndexedFile
	}
	Recording rec1;
	char ky[] = "LON2312";
	result = indexFile.Read (ky, rec1); //read a record with this key
	cout <<"Found record: "<<rec1;

	return 1;
}


int main ()
{// first argument is the file name for the data file
	int result;
	TextIndex RecIndex (10);
	DelimFieldBuffer Buffer; // create a buffer
	TextIndexedFile<Recording> IndFile (Buffer, 12, 10);

	char dfile[18] = "Data.csv";




	char name[] = "indfile";
	result = IndFile.Create(name);
			//A data file (named as indfile.dat) and its
			//index file (named as indfile.ind) are created 
		
	
	IndexRecordingFile (dfile, IndFile); 
	  //data from input file argc is inserted to indfile.dat and their index
	  //are inserted to indfile.ind.
	  
	IndFile.Close();
}

