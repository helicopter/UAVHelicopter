/*
 * Record.h
 *
 * Created: 2/25/2013 8:34:39 PM
 *  Author: HP User
 */ 


#ifndef RECORD_H_
#define RECORD_H_

#include "commonheader.h"

using namespace helicopter::util::common;

namespace helicopter
{
	namespace buffer
	{
		class Record
		{
			
			static const int MAXSIZEOFRECORD=256;
			
			private:
				byte *recordData;
				
				int sizeOfRecord;
				
				Record *previousRecord;
				Record *nextRecord;
			
			public:
				Record();
				Record(void* data, int sizeOfItem);
				~Record();
					
				Record *Copy ();
					
				Record *getPreviousRecord()
				{
					return previousRecord;	
				}					
				
				void setPreviousRecord(Record *record)
				{
					previousRecord = record;
				}
				
				Record *getNextRecord()
				{
					return nextRecord;
				}
				
				void setNextRecord(Record *record)
				{
					nextRecord = record;
				}
				
				void setRecordData(void *recordData, int sizeOfItem);
				
				
				byte* getRecordData()
				{
					//Should this really return record data, or copy of it?
					return recordData;
				}
				
				int getSizeOfRecord()
				{
					return sizeOfRecord;
				}
				
				void setSizeOfRecord(int size)
				{
					sizeOfRecord = size;
				}
			
		};
	}
}



#endif /* RECORD_H_ */