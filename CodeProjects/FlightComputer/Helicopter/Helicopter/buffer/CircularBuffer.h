/*
 * CircularBuffer.h
 *
 * Created: 9/1/2014 10:09:44 PM
 *  Author: HP User
 */ 


#ifndef CIRCULARBUFFER_H_
#define CIRCULARBUFFER_H_

#include "CommonHeader.h"

namespace helicopter
{
	namespace buffer
	{
		/**
		 * This class represents a linked list of records.
		 */
		class CircularBuffer
		{
			private:
				

				
				//linked list of records. 
				byte *buffer;
				
				
				//Represents the max number of records in this buffer
				int bufferSize;
				int headIndex;
				int tailIndex;
				int bytesInQueue;
			
			public:
				CircularBuffer(int bufferSize) :  buffer(NULL), bufferSize(bufferSize), headIndex(0), tailIndex(0), bytesInQueue(0)
				{
					buffer = new byte[bufferSize];
				}
				~CircularBuffer()
				{
					delete [] buffer;
				}
				
				int getBytesInQueue()
				{
					return bytesInQueue;
				}
				
				/**
				 * Pushes a byte onto the end of the queue. 
				 * @return true if the byte was successfully appended, false if the queue is full. The byte won't be appended. 
				 */
				bool enqueue(byte val);
				
				/**
				 * Pop the first byte off the queue.
				 * @return true if there was a byte poped off, False if there wasn't a byte to pop off. (the byte value returned will be 0);
				 */
				bool dequeue(byte &val);
		};
	}
}




#endif /* CIRCULARBUFFER_H_ */