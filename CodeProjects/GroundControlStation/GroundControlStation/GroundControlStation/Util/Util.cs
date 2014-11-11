using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GroundControlStation.Util
{
    public class Util
    {

        public const float GRAVITY = 9.80665f;

        public const float INTERVAL_BETWEEN_SIM_DATA = .05f;
        //public const float INTERVAL_BETWEEN_SIM_DATA = .02f;


        public static double ConvertToDegPerSec(float radsPerSecValue)
        {
            return radsPerSecValue * (180.0 / Math.PI);
        }

        public static float ConvertToRads(float degreesValue)
        {
            return (float)(degreesValue * (Math.PI / 180));
        }

        public static float[,] CreateRotationMatrix(float phiRotationAboutXRads, float thetaRotationAboutYRads, float saiRotationAboutZRads)
        {
            float[,] rotationMatrix = new float[3, 3];
            rotationMatrix[0, 0] = (float)Math.Cos(thetaRotationAboutYRads) * (float)Math.Cos(saiRotationAboutZRads);
            rotationMatrix[0, 1] = (float)Math.Sin(phiRotationAboutXRads) * (float)Math.Sin(thetaRotationAboutYRads) * (float)Math.Cos(saiRotationAboutZRads) - (float) Math.Cos(phiRotationAboutXRads) * (float) Math.Sin(saiRotationAboutZRads);
            rotationMatrix[0,2] = (float) Math.Cos(phiRotationAboutXRads) * (float) Math.Sin(thetaRotationAboutYRads) * (float) Math.Cos(saiRotationAboutZRads) + (float) Math.Sin(phiRotationAboutXRads) * (float) Math.Sin(saiRotationAboutZRads);
            rotationMatrix[1,0] = (float) Math.Cos(thetaRotationAboutYRads) * (float) Math.Sin(saiRotationAboutZRads);
            rotationMatrix[1,1] = (float) Math.Sin(phiRotationAboutXRads) * (float) Math.Sin(thetaRotationAboutYRads) * (float) Math.Sin(saiRotationAboutZRads) + (float) Math.Cos(phiRotationAboutXRads) * (float) Math.Cos(saiRotationAboutZRads);
            rotationMatrix[1,2] = (float) Math.Cos(phiRotationAboutXRads) * (float) Math.Sin(thetaRotationAboutYRads) * (float) Math.Sin(saiRotationAboutZRads) - (float) Math.Sin(phiRotationAboutXRads) * (float) Math.Cos(saiRotationAboutZRads);
            rotationMatrix[2,0] = -(float) Math.Sin(thetaRotationAboutYRads);
            rotationMatrix[2,1] = (float) Math.Sin(phiRotationAboutXRads) * (float) Math.Cos(thetaRotationAboutYRads);
            rotationMatrix[2,2] = (float) Math.Cos(phiRotationAboutXRads) * (float) Math.Cos(thetaRotationAboutYRads);
            return rotationMatrix;

        }

        public static float[,] CreateRotationMatrixTransposed(float phiRotationAboutXRads, float thetaRotationAboutYRads, float saiRotationAboutZRads)
        {
            float[,] rotationMatrix = new float[3, 3];

            rotationMatrix[0, 0] = (float)Math.Cos(thetaRotationAboutYRads) * (float)Math.Cos(saiRotationAboutZRads);
            rotationMatrix[0, 1] = (float)Math.Cos(thetaRotationAboutYRads) * (float)Math.Sin(saiRotationAboutZRads);
            rotationMatrix[0, 2] = (float)-Math.Sin(thetaRotationAboutYRads);
            rotationMatrix[1, 0] = (float)Math.Sin(phiRotationAboutXRads) * (float)Math.Sin(thetaRotationAboutYRads) * (float)Math.Cos(saiRotationAboutZRads) - (float)Math.Cos(phiRotationAboutXRads) * (float)Math.Sin(saiRotationAboutZRads);
            rotationMatrix[1, 1] = (float)Math.Sin(phiRotationAboutXRads) * (float)Math.Sin(thetaRotationAboutYRads) * (float)Math.Sin(saiRotationAboutZRads) + (float)Math.Cos(phiRotationAboutXRads) * (float)Math.Cos(saiRotationAboutZRads);
            rotationMatrix[1, 2] = (float)Math.Sin(phiRotationAboutXRads) * (float)Math.Cos(thetaRotationAboutYRads);
            rotationMatrix[2, 0] = (float)Math.Cos(phiRotationAboutXRads) * (float)Math.Sin(thetaRotationAboutYRads) * (float)Math.Cos(saiRotationAboutZRads) + (float)Math.Sin(phiRotationAboutXRads) * (float)Math.Sin(saiRotationAboutZRads);
            rotationMatrix[2, 1] = (float)Math.Cos(phiRotationAboutXRads) * (float)Math.Sin(thetaRotationAboutYRads) * (float)Math.Sin(saiRotationAboutZRads) - (float)Math.Sin(phiRotationAboutXRads) * (float)Math.Cos(saiRotationAboutZRads);
            rotationMatrix[2, 2] = (float)Math.Cos(phiRotationAboutXRads) * (float)Math.Cos(thetaRotationAboutYRads);

            return rotationMatrix;
        }


        internal static float[,] CalculateECEFToLocalNEDRotationMatrix(float latitudeDegrees, float longitudeDegrees)
        {
            float[,] rotationMatrix = new float[3, 3];

            float radLat = ConvertToRads(latitudeDegrees);
            float radLong = ConvertToRads(longitudeDegrees);

            //Create a rotation matrix for rotating earth-centered-earth-fixed coordinates
            //into Local North-East-Down coordinates.
            rotationMatrix[0, 0] = -1 * (float)Math.Sin(radLat) * (float)Math.Cos(radLong);
            rotationMatrix[0, 1] = -1 * (float)Math.Sin(radLat) * (float)Math.Sin(radLong);
            rotationMatrix[0, 2] = (float)Math.Cos(radLat);
            rotationMatrix[1, 0] = -1 * (float)Math.Sin(radLong);
            rotationMatrix[1, 1] = (float)Math.Cos(radLong);
            rotationMatrix[1, 2] = 0;
            rotationMatrix[2, 0] = -1 * (float)Math.Cos(radLat) * (float)Math.Cos(radLong);
            rotationMatrix[2, 1] = -1 * (float)Math.Cos(radLat) * (float)Math.Sin(radLong);
            rotationMatrix[2, 2] = -1 * (float)Math.Sin(radLat);

            return rotationMatrix;
        }

        public static float[,] CalculateECEFToLocalNEDRotationMatrixTransposed(float latitudeDegrees, float longitudeDegrees)
        {
            float[,] rotationMatrix = new float[3, 3];

            float radLat = ConvertToRads(latitudeDegrees);
            float radLong = ConvertToRads(longitudeDegrees);

	        //Create a rotation matrix for rotating earth-centered-earth-fixed coordinates
	        //into Local North-East-Down coordinates.
            rotationMatrix[0, 0] = -1 * (float)Math.Sin(radLat) * (float)Math.Cos(radLong);
            rotationMatrix[0, 1] = -1 * (float)Math.Sin(radLong);
            rotationMatrix[0, 2] = -1 * (float)Math.Cos(radLat) * (float)Math.Cos(radLong);
            rotationMatrix[1, 0] = -1 * (float)Math.Sin(radLat) * (float)Math.Sin(radLong);
            rotationMatrix[1, 1] = (float)Math.Cos(radLong);
            rotationMatrix[1, 2] = -1 * (float)Math.Cos(radLat) * (float)Math.Sin(radLong);
            rotationMatrix[2, 0] = (float)Math.Cos(radLat);
            rotationMatrix[2, 1] = 0;
            rotationMatrix[2, 2] = -1 * (float)Math.Sin(radLat);

            return rotationMatrix;
        }



        public static float[] RotateMatrix2(float[][] rotationMatrix, float[] valuesToRotate)
        {
            float[] rotatedValues = new float[3];

            //iterate through the rows of the rotation matrix.
            for (int row = 0; row < 3; row++)
            {
                //iterate through the columns of the rotation matrix
                for (int column = 0; column < 3; column++)
                {
                    //technically the rotatedMatrix[row] is writing to a 'column' in the matrix, and velocitiesNEDCms is actually the 'row'.
                    rotatedValues[row] += rotationMatrix[row][column] * valuesToRotate[column];
                }
            }

            return rotatedValues;
        }



        public static float[] RotateMatrix(float[,] rotationMatrix, float[] valuesToRotate)
        {
	        float[] rotatedValues = new float[3];

	        //iterate through the rows of the rotation matrix.
	        for (int row = 0; row < 3; row++)
	        {
		        //iterate through the columns of the rotation matrix
		        for (int column = 0; column < 3; column++)
		        {
			        //technically the rotatedMatrix[row] is writing to a 'column' in the matrix, and velocitiesNEDCms is actually the 'row'.
			        rotatedValues[row] += rotationMatrix[row,column] * valuesToRotate[column];
		        }
	        }

            return rotatedValues;
        }


        static float E = 0.08181919f;
        static float REA = 6378137.0f;

        public static float calculateNe(float E, float latitudeRads)
        {
            return REA / (float)Math.Sqrt(1 - (float)Math.Pow(E, 2) * (float)Math.Pow((float)Math.Sin(latitudeRads), 2));
        }

        public static void ConvertFromGeodeticToECEF(float latitudeDegrees, float longitudeDegrees, float altitudeFeetAgl, out float  ecefX, out float ecefY, out float ecefZ)
        {
            float radLat = ConvertToRads(latitudeDegrees);
            float radLong = ConvertToRads(longitudeDegrees);
	

	        float Ne = calculateNe(E, radLat);

            ecefX = (Ne + altitudeFeetAgl) * (float)Math.Cos(radLat) * (float)Math.Cos(radLong);
            ecefY = (Ne + altitudeFeetAgl) * (float) Math.Cos(radLat) * (float)Math.Sin(radLong);
            ecefZ = ((Ne * (1 - (float)Math.Pow(E, 2))) + altitudeFeetAgl) * (float)Math.Sin(radLat);
        }

    }
}
