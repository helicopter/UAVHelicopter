/*
 * SerialDriverTests.h
 *
 * Created: 9/1/2013 7:33:55 PM
 *  Author: HP User
 */ 


#ifndef SERIALDRIVERINTEGRATIONTESTS_H_
#define SERIALDRIVERINTEGRATIONTESTS_H_


#include "TestCase.h"

using namespace helicoptertestscommon::util::testframework;

/**
 * Integration test to ensure that receiving and transmitting from the
 * serial driver is working properly.
 * This sends data via the serial driver, and verifies that it gets echoed
 * back in the same form. This test assumes there is something on the
 * other end which is echoing the data back.
 */
int serialreceivetransmit_test(TestCase *test);


#endif /* SERIALDRIVERINTEGRATIONTESTS_H_ */