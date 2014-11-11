using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GroundControlStation.Util
{
    public class AHRS
    {

        public static float yawRads;
        public static float pitchRads;
        public static float rollRads;

        //public static float[][] dcm = new float[3][] { new float[]{ 1, 0, 0 }, new float[]{ 0, 1, 0 }, new float[]{ 0, 0, -1 } };
        public static float[][] dcm = new float[3][] { new float[] { 1, 0, 0 }, new float[] { 0, 1, 0 }, new float[] { 0, 0, 1 } };



        public static void reset()
        {
            //dcm = new float[3][] { new float[] { 1, 0, 0 }, new float[] { 0, 1, 0 }, new float[] { 0, 0, -1 } };
            dcm = new float[3][] { new float[] { 1, 0, 0 }, new float[] { 0, 1, 0 }, new float[] { 0, 0, 1 } };
        }



        static float[] scaleAndAdjust(float[] vectorToAdjust, float[] vectorToScale, float scalerValue, float[] outputVector)
        {
	        outputVector[0] = vectorToAdjust[0] - scalerValue * vectorToScale[0];
	        outputVector[1] = vectorToAdjust[1] - scalerValue * vectorToScale[1];
	        outputVector[2] = vectorToAdjust[2] - scalerValue * vectorToScale[2];
	
            return outputVector;
        }

        static float DotProduct(float[] vector1, float[] vector2)
        {
	        return vector1[0]*vector2[0] + vector1[1]*vector2[1] + vector1[2]*vector2[2];
        }


        static void orthonormalizeDcm() 
        {
	        /**
	         * The dot product of two orthonormal vectors should be 0 (because cos(90) = 0). So this
	         * gives us an error of how far from orthonormal the vectors are. It's a measure
	         * of how much X and Y are rotated towards each other. 
	         */
	        float error = DotProduct(dcm[0], dcm[1]);
	
	        /**
	         * Scale and adjust the two vectors involved in calculating the error to bring them 
	         * 'closer' to each other (more orthonormal)
	         */
	        dcm[0] = scaleAndAdjust(dcm[0], dcm[1], (error/2), dcm[0]);
	        dcm[1] = scaleAndAdjust(dcm[1], dcm[0], (error/2), dcm[1]);
	
	        //The remaining vector will become the cross product of the other two vectors
            dcm[2] = CrossProduct(dcm[0], dcm[1], dcm[2]);

	        //take crossproduct of dcm[1] first because dcm[2] is positive pointing down.
            //dcm[2] = CrossProduct(dcm[1], dcm[0], dcm[2]);
	
	        //Normalize the matrix to ensure that the vector's magnitudes = 1
	        dcm[0]=  Normalize(dcm[0]);
	        dcm[1] = Normalize(dcm[1]);
            dcm[2] = Normalize(dcm[2]);

        }



        static float[] Normalize(float[] vector)
        {
	        float magnitude = 0;
	
	        //Square each vector value and add it to the magnitude
	        for (int i = 0; i < 3; i++)
	        {
		        magnitude += vector[i] * vector[i];
	        }
	
	        magnitude = (float) Math.Sqrt(magnitude);
	
	        for (int i = 0; i < 3; i++)
	        {
		        vector[i] = vector[i] / magnitude;
	        }

            return vector;
        }


        public static float[] CrossProduct(float[] vector1, float[] vector2, float[] crossProduct)
        {
	        crossProduct[0] = vector1[1] * vector2[2] - vector1[2]*vector2[1];
	        crossProduct[1] = -1 * (vector1[0] * vector2[2] - vector1[2]*vector2[0]);
	        crossProduct[2] = vector1[0] * vector2[1] - vector1[1] * vector2[0];

            return crossProduct;    
        }




        static float gyroTimeInterval = 1 / 20.0f;
        
        public static void ahrs(float frdAccXMss, float frdAccYMss, float frdAccZMss,
            float frdGyroXRs, float frdGyroYRs, float frdGyroZRs,
            float frdMagX, float frdMagY, float frdMagZ, float accelWeight, float magWeight)
        {


            //float[] accelerometerVector = new float[3] {frdAccXMss, frdAccYMss, frdAccZMss};
            float[] accelerometerVector = new float[3] { frdAccXMss, frdAccYMss, frdAccZMss };

	        float[] magnetometerVector = new float[3] {frdMagX, frdMagY, frdMagZ};
		
	        float[] accelerometerAngularDisplacement = new float[3];
            float[] magnetometerAngularDisplacement = new float[3];

            float[]  gyroscopeAngularDisplacement = new float[3] { frdGyroXRs * gyroTimeInterval, 
											frdGyroYRs * gyroTimeInterval, 
											frdGyroZRs * gyroTimeInterval};

            float[] angularDisplacementWeightedAverage = new float[3];	
		
	        float[] changeInVector  = new float[3];



            accelerometerVector = Normalize(accelerometerVector);
            magnetometerVector = Normalize(magnetometerVector);


            float[] westFacingVector = new float[3];
            float[] correctedMagnetometerVector = new float[3];


            //aligning the mag vector to the dcm estimate rather than the accelerometer sensor gives me much better yaw prediction performance.
            //it hurts pitch performance a bit though.
            westFacingVector = CrossProduct(dcm[2], magnetometerVector, westFacingVector);
            correctedMagnetometerVector = CrossProduct(westFacingVector, dcm[2], correctedMagnetometerVector);
             

            //take cross product of sensor data and not of dcm because if we did dcm, then the initial angular displacements would be near 0 because the cross product would result in a vector of 1,0,0 which is basically the initial vector. 
            //so without this the angular displacements between the mag and accel would be different. and they shouldn't be. they should be near the same. 
            //westFacingVector = CrossProduct(accelerometerVector, magnetometerVector, westFacingVector);
            //correctedMagnetometerVector = CrossProduct(westFacingVector, accelerometerVector, correctedMagnetometerVector);
            

            correctedMagnetometerVector = Normalize(correctedMagnetometerVector);


            /*
	        accelerometerAngularDisplacement = CrossProduct(accelerometerVector, dcm[2], accelerometerAngularDisplacement);
	        magnetometerAngularDisplacement = CrossProduct(correctedMagnetometerVector, dcm[0], magnetometerAngularDisplacement);
	        */

            //added subtraction from previous estimate because that is what's in the white paper, although it's not in the example code. 
            //float[] acctemp = {accelerometerVector[0]-dcm[2][0], accelerometerVector[1]-dcm[2][1], accelerometerVector[2]-dcm[2][2]};
            //float[] magtemp = { correctedMagnetometerVector[0] - dcm[0][0], correctedMagnetometerVector[1] - dcm[0][1], correctedMagnetometerVector[2] - dcm[0][2] };
            
            /*float[] acctemp = { accelerometerVector[0] - dcm[2][0], accelerometerVector[1] - dcm[2][1], accelerometerVector[2] - dcm[2][2] };
            float[] magtemp = { correctedMagnetometerVector[0] - dcm[0][0], correctedMagnetometerVector[1] - dcm[0][1], correctedMagnetometerVector[2] - dcm[0][2] };


            accelerometerAngularDisplacement = CrossProduct(dcm[2], acctemp, accelerometerAngularDisplacement);
            magnetometerAngularDisplacement = CrossProduct(dcm[0], magtemp, magnetometerAngularDisplacement);
	        */
            //i take cross product of dcmxaccel because this is the same as the dcmxerrorofdcmandest that the paper discusses. The code does this but the paper doesn't discuss this option.
            accelerometerAngularDisplacement = CrossProduct(dcm[2], accelerometerVector, accelerometerAngularDisplacement);
            magnetometerAngularDisplacement = CrossProduct(dcm[0], correctedMagnetometerVector, magnetometerAngularDisplacement);


	        for (int i = 0; i < 3; i++)
	        {

		        /*angularDisplacementWeightedAverage[i] = (-gyroscopeAngularDisplacement[i] + 
												        accelerometerAngularDisplacement[i] * accelWeight + 
												        magnetometerAngularDisplacement[i] * magWeight) /
												        (1.0f + accelWeight + magWeight);
                 */
                angularDisplacementWeightedAverage[i] = (-gyroscopeAngularDisplacement[i] +
                                                        accelerometerAngularDisplacement[i] * accelWeight +
                                                        magnetometerAngularDisplacement[i] * magWeight) /
                                                        (1.0f + accelWeight + magWeight);

	        }

	        /**
	         * Adjust the dcm matrix by the calculated angular displacement. We adjust the DCM matrix by the averaged
	         * angle displacement of the three sensors because this reduces noise caused by any one sensor, and 
	         * reduces the effect of non-gravitational acceleration (linear acceleration) from the accelerometer.
	         */
	        for (int i = 0; i < 3; i++)
	        {
                changeInVector = CrossProduct(angularDisplacementWeightedAverage, dcm[i], changeInVector);
		
		        dcm[i][0] += changeInVector[0];
		        dcm[i][1] += changeInVector[1];
		        dcm[i][2] += changeInVector[2];
	        }


            orthonormalizeDcm();

            /*
            yawRads = (float) Math.PI + (float) Math.Atan2(dcm[1][0], dcm[0][0]);
            pitchRads = (float) -Math.Asin(dcm[2][0]);
            rollRads = (float)Math.Atan2(dcm[2][1], dcm[2][2]);
             */

            //originally from file:///C:/Users/HP%20User/Downloads/EulerAngles%20(1).pdf
            
            
            /*
            //Negated the others to align with axies more properly. I don't know why.
            yawRads = (float)-Math.Atan2(dcm[1][0], dcm[0][0]);

            if (yawRads < 0)
            {
                yawRads = (float) 2f * (float)Math.PI + yawRads;
            }

            pitchRads = (float)Math.Asin(dcm[2][0]);
            rollRads = (float)-Math.Atan2(dcm[2][1], dcm[2][2]);
             * 
             */

            yawRads = (float)Math.Atan2(dcm[1][0], dcm[0][0]);
            //convert yaw to 0-2pi. 
            if (yawRads < 0)
            {
                yawRads = (float)2f * (float)Math.PI + yawRads;
            }

            pitchRads = (float)-Math.Asin(dcm[2][0]);
            rollRads = (float)Math.Atan2(dcm[2][1], dcm[2][2]);
        }
    }
}
