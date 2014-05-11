/*
 * RadioDriverTests.h
 *
 * Created: 8/25/2013 3:37:44 PM
 *  Author: HP User
 */ 


#ifndef RADIOINTERFACEINTEGRATIONTESTS_H_
#define RADIOINTERFACEINTEGRATIONTESTS_H_

#include "TestCase.h"

using namespace helicoptertestscommon::util::testframework;

/**
 * Tests sending and receiving data over the radio by
 * sending data over the radio, and then receiving data
 * that is an echo of the sent data. It's assumed that
 * software is running on the other end to echo the
 * sent data back.
 */
//int radiotransmitandreceive_test(TestCase *test);

int radiocontrollerservooutput_test(TestCase *test);

int systemtelemetrytransmitandreceive_test(TestCase *test);

int newmsgformat_test(TestCase *test);
int newmsgformat2_test(TestCase *test);


//int reliablyreceive_test(TestCase *test);


#endif /* RADIOINTERFACEINTEGRATIONTESTS_H_ */