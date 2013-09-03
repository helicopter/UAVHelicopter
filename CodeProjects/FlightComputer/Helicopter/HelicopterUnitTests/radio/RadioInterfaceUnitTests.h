/*
 * RadioInterfaceTests.h
 *
 * Created: 8/26/2013 11:13:06 AM
 *  Author: HP User
 */ 


#ifndef RADIOINTERFACEUNITTESTS_H_
#define RADIOINTERFACEUNITTESTS_H_


#include "TestCase.h"

using namespace helicoptertestscommon::util::testframework;

/**
 * Tests sending and receiving data over the radio by
 * sending data over the radio, and then receiving data
 * that is an echo of the sent data. It's assumed that
 * software is running on the other end to echo the
 * sent data back.
 */
int radiotransandrec_test(TestCase *test);


#endif /* RADIOINTERFACEUNITTESTS_H_ */