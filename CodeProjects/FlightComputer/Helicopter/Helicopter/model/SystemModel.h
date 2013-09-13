/*
 * SystemModel.h
 *
 * Created: 9/8/2013 4:57:07 PM
 *  Author: HP User
 */ 


#ifndef SYSTEMMODEL_H_
#define SYSTEMMODEL_H_

#include "SystemTelemetryMessage.h"

using namespace helicopter::messages;

namespace helicopter
{
	namespace model
	{
		
	
		class SystemModel
		{
			private:
			int rawMagX;
			
			int rawMagY;
			
			int rawMagZ;
			
			public:
			
			SystemModel():
			rawMagX(0),
			rawMagY(0),
			rawMagZ(0)
			{
				
			}
			
			int RawMagX() const { return rawMagX; }
			void RawMagX(int val) { rawMagX = val; }
			
			int RawMagY() const { return rawMagY; }
			void RawMagY(int val) { rawMagY = val; }
			
			int RawMagZ() const { return rawMagZ; }
			void RawMagZ(int val) { rawMagZ = val; }
				
			SystemTelemetryMessage *CreateTelemetryMessage();
		};
	}
	
}



#endif /* SYSTEMMODEL_H_ */