// indbuff.tc
#include "tindbuff.h"

TextIndexBuffer::TextIndexBuffer (int keySize, int maxKeys, 
								  int extraFields, int extraSpace)
: FixedFieldBuffer (1+2*maxKeys+extraFields, 
		sizeof(int)+maxKeys*keySize+maxKeys*sizeof(int) + extraSpace)
	// 1. buffer fields consist of the folowing fields: 
	//    1)numKeys, actual number of keys
	//    2)Keys [maxKeys] key fields, size = maxKeys * keySize
	//    3)RecAddrs [maxKeys] record address fields, size = maxKeys*sizeof(int)
	//    4)dammy 
	// 2. 1+2*maxKeys+extraFields gives the total of number fields -- Jie
{
	MaxKeys = maxKeys;
	KeySize = keySize;
	AddField (sizeof(int)); //add the field for storing numKeys
	for (int i = 0; i < maxKeys; i++)
	{
		AddField (KeySize); //add  the field for keySize
		AddField (sizeof(int)); //add the field for record address size
	}
	Dummy = new char[keySize+1]; // allocate the space for dummy in pack and unpack
	           // its length is same as the length of (key, recordAddress)
}

int TextIndexBuffer::Pack (const TextIndex & index)
{
	int result;
	Clear ();
	result = FixedFieldBuffer::Pack (&index.NumKeys);
	for (int i = 0; i < index.NumKeys; i++)
	{// note only pack the actual keys and recaddrs
		result = result && FixedFieldBuffer::Pack (index.Keys[i]);
		result = result && FixedFieldBuffer::Pack (&index.RecAddrs[i]);
	}
	for (int j = 0; j<index.MaxKeys-index.NumKeys; j++)
	{// pack dummy values for other fields
		result = result && FixedFieldBuffer::Pack (Dummy);
		result = result && FixedFieldBuffer::Pack (Dummy);
	}
	return result;
}

int TextIndexBuffer::Unpack(TextIndex & index) 
{
	int result;
	result = FixedFieldBuffer::Unpack (&index.NumKeys);
	for (int i = 0; i < index.NumKeys; i++)
	{// note only pack the actual keys and recaddrs
		index.Keys[i] = new char[KeySize]; // just to be safe
		result = result && FixedFieldBuffer::Unpack (index.Keys[i]);
		result = result && FixedFieldBuffer::Unpack (&index.RecAddrs[i]);
	}
	for (int j = 0; j<index.MaxKeys-index.NumKeys; j++)
	{// pack dummy values for other fields
		result = result && FixedFieldBuffer::Unpack (Dummy);
		result = result && FixedFieldBuffer::Unpack (Dummy);
	}
	return result;
}

void TextIndexBuffer:: Print (ostream & stream) const
{
	stream <<"TextIndexBuffer: KeySize "<<KeySize
		<<" MaxKeys "<<MaxKeys<<endl;
	FixedFieldBuffer :: Print (stream);
}

