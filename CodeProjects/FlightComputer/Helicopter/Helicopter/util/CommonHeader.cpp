/*
 * CommonHeader.cpp
 *
 * Created: 2/28/2013 8:48:54 PM
 *  Author: HP User
 */ 
#include <CommonHeader.h>


void * operator new(size_t size)
{
	return malloc(size);
}

void operator delete(void * ptr)
{
	if (ptr != NULL)
	{
		free(ptr);
	}
}

void * operator new[](size_t size)
{
	return malloc(size);
}

void operator delete[](void * ptr)
{
	if(ptr != NULL)
	{
		free(ptr);
	}
	
}