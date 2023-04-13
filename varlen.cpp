// varlen.cc

#include "varlen.h"
#include <string>

//class VariableLengthBuffer

// public members

VariableLengthBuffer :: VariableLengthBuffer (int maxBytes)
 // construct with a maximum of maxFields
: IOBuffer (maxBytes)
{
	Init ();
}

void VariableLengthBuffer :: Clear ()
// clear fields from buffer
{
	IOBuffer::Clear();
}

int VariableLengthBuffer :: Read (istream & stream)
// write the number of bytes in the buffer from the stream
// the record length is represented by an unsigned short value
{
	if (stream.eof()) return -1;
	int recaddr = stream . tellg ();//file point position for get
	Clear ();
	unsigned short bufferSize;
	stream . read ((char *)&bufferSize, sizeof(bufferSize));
	if (! stream . good ()){stream.clear(); return -1;}
	BufferSize = bufferSize;
	if (BufferSize > MaxBytes) return -1; // buffer overflow
	stream . read (Buffer, BufferSize);
	if (! stream . good ()){stream.clear(); return -1;}
	return recaddr; //file position for get?? why?
}


int VariableLengthBuffer :: Write (ostream & stream) const
// write the length and buffer into the stream
{
	int recaddr = stream . tellp ();//put pointer position  
	unsigned short bufferSize;
	bufferSize = BufferSize;
	stream . write ((char *)&bufferSize, sizeof(bufferSize));
	if (!stream) return -1;
	stream . write (Buffer, BufferSize);
	if (! stream . good ()) return -1;
	return recaddr; //the address (position) of the record in the file
}

const char * headerStrV = "Variable";
const int headerSizeV = 9;

int VariableLengthBuffer :: ReadHeader (istream & stream)
// read the header and check for consistency
{
	char str[headerSizeV+1];
	int result;
	// read the IOBuffer header
	result = IOBuffer::ReadHeader (stream);
	if (!result) return FALSE;
	// read the header string
	stream . read (str, headerSizeV);
	if (!stream.good()) return FALSE;
	if (strncmp (str, headerStrV, headerSizeV) != 0) return FALSE;
	// read and check the record description
	return stream . tellg ();
}

int VariableLengthBuffer :: WriteHeader (ostream & stream) const
// write a buffer header to the beginning of the stream
// A header consists of the 
//	IOBUFFER header	
//	header string
//	Variable sized record of length fields that describes the file records
{
	int result;
	// write the parent (IOBuffer) header
	result = IOBuffer::WriteHeader (stream); //"IOBuffer" 
	if (!result) return FALSE;
	// write the header string
	stream . write (headerStrV, headerSizeV); //"Variable"
	if (!stream . good ()) return FALSE;
	// write the record description
	return stream . tellp();
}

void VariableLengthBuffer :: Print (ostream & stream) const
{
	IOBuffer::Print (stream);
}

int VariableLengthBuffer :: Init ()
 // construct with a maximum of maxFields
{
	Clear();
	return TRUE;
}

