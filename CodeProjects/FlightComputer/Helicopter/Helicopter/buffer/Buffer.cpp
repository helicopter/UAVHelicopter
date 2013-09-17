/*
 * CircularByteBuffer.cpp
 *
 * Created: 2/24/2013 3:32:26 PM
 *  Author: HP User
 */ 
#include "Buffer.h"
#include "string.h"
#include "commonheader.h"

using namespace helicopter::buffer;

Buffer::Buffer()
{
	bufferSize = 0;
	records = NULL;
	firstRecord = NULL;
	lastRecord = NULL;
}

Buffer::~Buffer()
{
	//TODO should this be delete[]?
	delete this->records;
}

void Buffer::enqueue(Record *record)
{
	if (record != NULL)
	{
		//Copy the record
		//TODO danger not deep copy. 
		Record *newRecord = record->Copy();
		
		//Add the record to the linked list.
		if (this->records == NULL)
		{
			records = newRecord;
			firstRecord = newRecord;
			lastRecord = newRecord;
		}else
		{
			lastRecord->setNextRecord(newRecord);
			newRecord->setPreviousRecord(lastRecord);
			lastRecord = newRecord;
		}
		
		bufferSize++;
	}
}

//Client has to free the memory.


Record* Buffer::dequeue()
{
	Record *returnRecord = NULL;
	
	if (bufferSize != 0)
	{
		returnRecord = lastRecord;
		lastRecord = returnRecord->getPreviousRecord();
		bufferSize--;
	}
	
	return returnRecord;
}

