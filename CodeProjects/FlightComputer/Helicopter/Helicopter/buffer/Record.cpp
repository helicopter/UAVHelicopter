/*
 * Record.cpp
 *
 * Created: 2/25/2013 9:02:25 PM
 *  Author: HP User
 */ 
#include "Record.h"
#include "string.h"

using namespace helicopter::buffer;

Record::Record()
{
	sizeOfRecord = 0;
	previousRecord = NULL;
	nextRecord = NULL;
	recordData = NULL;
}

Record::~Record()
{
	delete recordData;
}

Record::Record(void* recordData, int sizeOfItem)
{
	this->recordData = new byte[sizeOfItem];
	
	memcpy(this->recordData, recordData, sizeOfItem);
	
	sizeOfRecord = sizeOfItem;
	
	previousRecord = NULL;
	nextRecord = NULL;
}

void Record::setRecordData(void* recordData, int sizeOfItem)
{
	this->recordData = new byte[sizeOfItem];
	
	memcpy(this->recordData, recordData, sizeOfItem);
	
	sizeOfRecord = sizeOfItem;
}

Record * Record::Copy()
{
	Record *record = new Record(this->getRecordData(), this->getSizeOfRecord());
	
	//not copying the next and last pointers. 
	
	return record;
}