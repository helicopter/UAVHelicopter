/*
 * GPSSensorTests.h
 *
 * Created: 2/23/2014 9:47:34 PM
 *  Author: HP User
 */ 


#ifndef GPSSENSORTESTS_H_
#define GPSSENSORTESTS_H_


#include "TestCase.h"

using namespace helicoptertestscommon::util::testframework;


int readSensor_test(TestCase *test);
int readRealSensor_test(TestCase *test);
int nedGps_test(TestCase *test);
int partialTransmit_test(TestCase *test);
int gpsPartialReceive_test(TestCase *test);
int baudchange_test(TestCase *test);
int fwdGpsData_test(TestCase *test);


//
//int readSensor_test(TestCase *test);
//int readgps_test(TestCase *test);
//int parsegpsmsg_test(TestCase *test);
//int calcXY_test(TestCase *test);
//
//



#endif /* GPSSENSORTESTS_H_ */