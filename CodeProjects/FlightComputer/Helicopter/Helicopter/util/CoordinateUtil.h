/*
 * MathUtil.h
 *
 * Created: 12/15/2013 5:18:12 PM
 *  Author: HP User
 */ 


#ifndef COORDINATEUTIL_H_
#define COORDINATEUTIL_H_

#include <math.h>

namespace helicopter
{
	namespace util
	{
		/**
		 * In order for the algorithms to work, they need to know the helicopters position in
		 * Cartesian coordinates (X,Y,Z). However, the sensors gives the helicopters position
		 * in Curvilinear coordinates (lat/long). This class is used to transform the
		 * curvilinear coords into Cartesian coordinates.
		 *
		 * https://github.com/helicopter/UAVHelicopter/wiki/Coordinate-Systems
		 */
		class CoordinateUtil
		{	
			private:
			
				static const float PIOVER180;
				
				//The first eccentricity, used to convert lat/long to ecef
				static const float E;
				
				//The semi-major axis: Radius of the earth in Meters.
				static const float REA;
				
				/**
				 * Calculates the Ne Variable used to convert crom lat/long to ecef.
				 * This is the Prime vertical radius of curvature of the earth
				 * @param E The first eccentricity
				 * @param the latitude of the helicopter in Radians
				 */
				static float calculateNe(float E, float latitudeRads);
			
			public: 
			
				/**
				 * Converts degrees to radians
				 */
				inline static float DegreesToRad(float valueDegrees)
				{
					return valueDegrees * PIOVER180;
				}

			
				/**
				 * This method creates a rotation matrix that is used to convert from Earth-Center-Earth-Fixed to Local North-East-Down
				 * An example of the math behind this algorithm can be found on page 32 in 'Unmanned Roto Craft Systems', cai, chen, lee
				 * @param latitude The latitude of the aircraft in degrees
				 * @param longitude The longitude of the aircraft in degrees
				 * @param ecefToLocalNEDRotationMatrix OutputParam: a 3x3 matrix which will be populated with the rotation matrix used to convert
				 * earth centered earth fixed coordinates to Local North East Down coordinates.
				 */
				static void CalculateECEFToLocalNEDRotationMatrix(float latitudeDegrees, float longitudeDegrees, float  (&ecefToLocalNEDRotationMatrix)[3][3]);
			
				/**
				 * This method converts a set of curvilinear geodetic coordinates (lat long) to Cartesian coordinates (xyz) in the
				 * earth-centered-earth-fixed frame of reference.
				 * @param latitude the lat of the aircraft in degrees
				 * @param longitude the long of the aircraft in degrees
				 * @param altitudeFeetAgl the altitude of the aircraft in feet above ground level (as opposed to altitude above sea level)
				 * @param ecefX OutputParam: The X coordinate in earth-centered-earth-fixed
				 * @param ecefY OutputParam: The Y coordinate in earth-centered-earth-fixed
				 * @param ecefZ OutputParam: The Z coordinate in earth-centered-earth-fixed
				 */
				static void ConvertFromGeodeticToECEF(float latitudeDegrees, float longitudeDegrees, float altitudeFeetAgl, float &ecefX, float &ecefY, float &ecefZ);
			
			
				/**
				 * This method converts variables from Earth centered earth fixed frame, to local North East Down frame.
				 * @param ecefReferenceX The reference X position in ecef format. I.e. the helicopters take-off point X variable in ecef
				 * @param ecefReferenceY The reference Y position in ecef format. I.e. the helicopters take-off point Y variable in ecef
				 * @param ecefReferenceZ The reference Z position in ecef format. I.e. the helicopters take-off point Z variable in ecef
				 * @param ecefX the current helicopters X position in ecef format
				 * @param ecefY the current helicopters Y position in ecef format.
				 * @param ecefZ the current helicopters Z position in ecef format.
				 * @param localNEDX OutputParam: The X coordinate of the helicopter in the local north east down frame.
				 * @param localNEDY OutputParam: The Y coordinate of the helicopter in the local north east down frame.
				 * @param localNEDZ OutputParam: The Z coordinate of the helicopter in the local north east down frame.
				 */
				static void ConvertFromECEFToLocalNED(float ecefReferenceX, float ecefReferenceY, float ecefReferenceZ, 
					float ecefX, float ecefY, float ecefZ, float ecefToLocalNEDRotationMatrix[][3],
					float &localNEDX, float &localNEDY, float &localNEDZ);
					
					
				/**
				 * Same as the other method, but it uses integers instead. 
				 */
				static void ConvertFromECEFToLocalNED(long ecefReferenceX, long ecefReferenceY, long ecefReferenceZ,
					long ecefX, long ecefY, long ecefZ, float ecefToLocalNEDRotationMatrix[][3],
					float &localNEDX, float &localNEDY, float &localNEDZ);					
					
				static void ConvertFromGeodedicToLocalNED(
					float geodedicLatitude, float geodedicLongitude, float altitudeFeetAgl, 
					float ecefReferenceX, float ecefReferenceY, float ecefReferenceZ, float ecefToLocalNEDRotationMatrix[][3],
					float &localNEDX,float &localNEDY,float &localNEDZ);
		};
	}
	
}



#endif /* COORDINATEUTIL_H_ */