/*
 * CircularByteBuffer.h
 *
 * Created: 2/24/2013 3:06:51 PM
 *  Author: HP User
 */ 


#ifndef BUFFER_H_
#define BUFFER_H_

#include "Record.h"

namespace helicopter
{
	namespace buffer
	{
		/**
		 * This class represents a linked list of records.
		 */
		class Buffer
		{
			private:
				
				//Represents the current number of records in this buffer
				int bufferSize;
				
				//linked list of records. 
				Record *records;
				Record *firstRecord;
				Record *lastRecord;
			
			public:
				Buffer();
				~Buffer();
				
				int getBufferSize()
				{
					return bufferSize;
				}
				
				void enqueue(Record *record);
				
				Record* dequeue();
		};
	}
}


#endif /* BUFFER_H_ */