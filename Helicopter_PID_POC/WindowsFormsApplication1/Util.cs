using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Helicopter1
{
    class Util
    {
        /*public static double UCOLMAXCONTROLVALUE = 5.0;
        public static double UCOLMINCONTROLVALUE = 0;*/

        public static double UCOLMAXCONTROLVALUE = 5.0;
        public static double UCOLMINCONTROLVALUE = 0;

        public static double UPEDMAXCONTROLVALUE = 9.6;
        public static double UPEDMINCONTROLVALUE = -9.6;

        public static int LISTENERPORT = 8089;


        public static double TIMEBETWEENINTERVALS = .05; //20HZ


        public static double velocityThresholdLimit = 4.0; //5m/s

        
        //public static double K_Y = .3252;
        //public static double K_Y = .03252;
     //   public static double K_Y = .46;
        //public static double K_Y = 0.2018;
     //   public static double K_Y = 2.75;
        public static double K_Y = 1.437;


        //public static double K_V = .2493;
        //public static double K_V = K_Y * (0.22);
        //public static double K_V =.782;
      //  public static double K_V = .866;
        public static double K_V = 2.8363;





        //public static double K_THETA = .7566;
        //public static double K_THETA = .107; //.021 worked well.
        //public static double K_THETA = .049; //.021 worked well.
        //public static double K_THETA = .021; //.021 worked well.
        public static double K_THETA = .031; //.021 worked well.

        //public static double K_X = .3256;
        //public static double K_X = 0.2359;
        public static double K_X = .289;


        //public static double K_U = 0.5;
        public static double K_U = 1.859;


        //public static double K_PitchVelocity = .0625;
        public static double K_PitchVelocity = .03;
        //public static double K_RollVelocity = .0225;
       // public static double K_RollVelocity = 0.07;
        public static double K_RollVelocity = 0.048;

        //public static double K_SAI = 3;
        //public static double K_SAI = 3.7;
 //**       public static double K_SAI = 8.2;
        //public static double K_SAI = 6;
   //    public static double K_SAI = 3.2;
        //public static double K_SAI = 3.6882;
        public static double K_SAI = 1.8471;


        
        //public static double K_PHI = .4569;
        //public static double K_PHI = .016;
        //public static double K_PHI = .296;
        //public static double K_PHI = .076;
        public static double K_PHI = .092;
        
        //public static double K_R = 17.6;
       // public static double K_R = .35;
        //public static double K_R = 12.2;
        //public static double K_R = 2.2;
   //public static double K_R = 4.7;
        //public static double K_R = K_SAI * 8;
        //public static double K_R = K_SAI * (0.09);
        public static double K_R = K_SAI * (0.09);
        


        //public static double K_Z = 1.6018;
        //public static double K_Z = .8706;//.4706;
        //public static double K_Z = 5.80;//.4706; // having a huge error actually helped a lot with overshoot. but thats whats increasing the derivative is for.
    //*****4.8
        //public static double K_Z = .2941;
        //public static double K_Z = .5;
        //public static double K_Z = .607;
        public static double K_Z = 1.901;

        //public static double K_W = 0.6060;
        //public static double K_W = 5.5;
        //public static double K_W = .4375;
        //public static double K_W = K_Z * (0.22);
        //public static double K_W = 2.472;
        public static double K_W = 1.141;
        //*****2.5


        public static double K_ETA_X = 0;

        //public static double K_ETA_SAI = 0.0;
    //    public static double K_ETA_SAI = 0.032;
       
       // public static double K_ETA_SAI = .0042;//.0032;
     //   public static double K_ETA_SAI = .0040;//.0032; -- good one
        //public static double K_ETA_SAI = 2.7778;
        public static double K_ETA_SAI = .5;
      
        public static double K_ETA_Y = 0;
        
        //public static double K_ETA_Z = 0.008;
//        public static double K_ETA_Z = 0.1;
        //public static double K_ETA_Z = 0.073;//.033
        //public static double K_ETA_Z = 0.088;
        public static double K_ETA_Z = .02;
        //*****


        //public static double Ti_Z = Math.Sqrt((1 / K_Z) * (1 / K_W));
        //public static double Ti_Z = Math.Sqrt(K_Z * K_W);
        public static double Ti_Z = .1;


        public static double Ti_SAI = .1;

        /*
        public static double K_THETA = .7566;
        public static double K_Y = .3252;
        public static double K_V = .2493;
        public static double K_X = .3256;
        
        public static double K_U = 0.1628;
        public static double K_SAI = 3;
        //public static double K_SAI = .0015; // from symposium book
        public static double K_PHI = .4569;
  //      public static double K_R = .35; 
        public static double K_R = 3.95; 
        //public static double K_R = .0001; // from symposium book
        
        public static double K_W = 0.6060;
        public static double K_Z = 1.6018;


        public static double K_ETA_X = 0;
   //     public static double K_ETA_X = 0.004;//mine
        public static double K_ETA_SAI = 0.0;
   //     public static double K_ETA_SAI = .008; // from symposium book
        //public static double K_ETA_SAI = 0.008;
        public static double K_ETA_Y = 0;
  //      public static double K_ETA_Y = .004;//mine
        public static double K_ETA_Z = 0;
 //       public static double K_ETA_Z = .004; // from symposium book

        /*public static double K_ETA_X = .02;
        public static double K_ETA_SAI = .02;
        public static double K_ETA_Y = .02;
        public static double K_ETA_Z = .02;*/

        public static double Sin(double angleDegrees)
        {
            //return Math.Sin((Math.PI * angleDegrees) / 180.0);
            return Math.Sin(angleDegrees * (Math.PI / 180.0));
        }

        public static double Cos(double angleDegrees)
        {
            return Math.Cos(angleDegrees * (Math.PI / 180.0));
        }

        //public static double[] ConvertFromECEFToLocalNED(double[] ecefReferencePosition, double[] ecefCurrentPosition)
        //{

           
        //    double[,] rotationMatrix = GetRotationMatrixFromInertialToBodyFrame(orientation);
        //    double[,] positionMatrix = currentPosition.GetPositionMatrix();

        //    double[,] rotatedPosition = new double[,] { { 0 }, { 0 }, { 0 } };

        //    /*rotatedPosition[0,0] = rotationMatrix[0,0]*positionMatrix[0,0] + rotationMatrix[0,1]*positionMatrix[1,0] + rotationMatrix[0,2]*positionMatrix[2,0];
        //    rotatedPosition[1,0] = rotationMatrix[1,0]*positionMatrix[0,0] + rotationMatrix[1,1]*positionMatrix[1,0] + rotationMatrix[1,2]*positionMatrix[2,0];
        //    rotatedPosition[2,0] = rotationMatrix[2,0]*positionMatrix[0,0] + rotationMatrix[2,1]*positionMatrix[1,0] + rotationMatrix[2,2]*positionMatrix[2,0];
        //    */

        //    //for each column in rotation matrix through columns
        //    for (int i = 0; i < rotationMatrix.GetLength(1); i++)
        //    {

        //        for (int j = 0; j < positionMatrix.GetLength(0); j++)
        //        {
        //            rotatedPosition[i, 0] += rotationMatrix[i, j] * positionMatrix[j, 0];
        //        }
        //    }

        //    newPosition.XLatitudePosition = rotatedPosition[0, 0];
        //    newPosition.YLongitudePosition = rotatedPosition[1, 0];
        //    newPosition.ZAltitudePosition = rotatedPosition[2, 0];

        //    return newPosition;
        //}

        public static Position RotatePositionFromGeodedicFrameToBodyFrame(Orientation orientation, Position currentPosition, Position originalReferencePosition)
        {

            //Rotate from Geodedic to earth centered earth fixed
     /*       Position currentEcefPosition = new Position();
            double[] ecef = ConvertFromGeodeticToECEF(currentPosition.YLongitudePosition, currentPosition.XLatitudePosition, currentPosition.ZAltitudePosition);
            currentEcefPosition.XLatitudePosition = ecef[0];
            currentEcefPosition.YLongitudePosition = ecef[1];
            currentEcefPosition.ZAltitudePosition = ecef[2];*/
/*
            Position referenceEcefPosition = new Position();
            ecef = ConvertFromGeodeticToECEF(originalReferencePosition.YLongitudePosition, originalReferencePosition.XLatitudePosition, originalReferencePosition.ZAltitudePosition);
            referenceEcefPosition.XLatitudePosition = ecef[0];
            referenceEcefPosition.YLongitudePosition = ecef[1];
            referenceEcefPosition.ZAltitudePosition = ecef[2];

            */


            //Rotate from earth centered earth fixed to local NED (which is the same as vehicle carried NED)
            double[,] ecefToNEDRotationMatrix = calculateEcefToNEDMatrix(originalReferencePosition);


            Position currentMinusRef = new Position();
            currentMinusRef.XLatitudePosition = currentPosition.XLatitudePosition - originalReferencePosition.XLatitudePosition;
            currentMinusRef.YLongitudePosition = currentPosition.YLongitudePosition - originalReferencePosition.YLongitudePosition;
            currentMinusRef.ZAltitudePosition = currentPosition.ZAltitudePosition - originalReferencePosition.ZAltitudePosition;

            double[,] positionMatrix = currentMinusRef.GetPositionMatrix();
            double[,] rotatedPosition = new double[,] { { 0 }, { 0 }, { 0 } };


            Position currentPositionLocalNED = new Position();

            //for each column in rotation matrix through columns
            for (int i = 0; i < ecefToNEDRotationMatrix.GetLength(1); i++)
            {

                for (int j = 0; j < positionMatrix.GetLength(0); j++)
                {
                    rotatedPosition[i, 0] += ecefToNEDRotationMatrix[i, j] * positionMatrix[j, 0];
                }
            }

            currentPositionLocalNED.XLatitudePosition = rotatedPosition[0, 0];
            currentPositionLocalNED.YLongitudePosition = rotatedPosition[1, 0];
            currentPositionLocalNED.ZAltitudePosition = rotatedPosition[2, 0];


            

            //Rotate from vehicle carried NED to body NED
            double[,] NEDToBodyRotationMatrix = calculateNEDToBodyMatrix(orientation);
            positionMatrix = currentPositionLocalNED.GetPositionMatrix();
            rotatedPosition = new double[,] { { 0 }, { 0 }, { 0 } };


            Position currentPositionBody = new Position();

            //for each column in rotation matrix through columns
            for (int i = 0; i < NEDToBodyRotationMatrix.GetLength(1); i++)
            {

                for (int j = 0; j < positionMatrix.GetLength(0); j++)
                {
                    rotatedPosition[i, 0] += NEDToBodyRotationMatrix[i, j] * positionMatrix[j, 0];
                }
            }

            currentPositionBody.XLatitudePosition = rotatedPosition[0, 0];
            currentPositionBody.YLongitudePosition = rotatedPosition[1, 0];
            currentPositionBody.ZAltitudePosition = rotatedPosition[2, 0];

            return currentPositionBody;
        }

        public static double[,] calculateNEDToBodyMatrix(Orientation currentOrientation)
        {
            double[,] rotationMatrix = new double[,] { 
            { Cos(currentOrientation.Theta_pitchDegrees)*Cos(currentOrientation.Sai_yawDegrees), Cos(currentOrientation.Theta_pitchDegrees)*Sin(currentOrientation.Sai_yawDegrees), -1 * Sin(currentOrientation.Theta_pitchDegrees)  }, 
            { Sin(currentOrientation.Phi_rollDegrees)*Sin(currentOrientation.Theta_pitchDegrees)*Cos(currentOrientation.Sai_yawDegrees) - Cos(currentOrientation.Phi_rollDegrees)*Sin(currentOrientation.Sai_yawDegrees), Sin(currentOrientation.Phi_rollDegrees)*Sin(currentOrientation.Theta_pitchDegrees)*Sin(currentOrientation.Sai_yawDegrees)+Cos(currentOrientation.Phi_rollDegrees)*Cos(currentOrientation.Sai_yawDegrees), Sin(currentOrientation.Phi_rollDegrees) * Cos(currentOrientation.Theta_pitchDegrees)}, 
            { Cos(currentOrientation.Phi_rollDegrees)*Sin(currentOrientation.Theta_pitchDegrees)*Cos(currentOrientation.Sai_yawDegrees)+Sin(currentOrientation.Phi_rollDegrees)*Sin(currentOrientation.Sai_yawDegrees), Cos(currentOrientation.Phi_rollDegrees)*Sin(currentOrientation.Theta_pitchDegrees)*Sin(currentOrientation.Sai_yawDegrees) - Sin(currentOrientation.Phi_rollDegrees)*Cos(currentOrientation.Sai_yawDegrees), Cos(currentOrientation.Phi_rollDegrees)*Cos(currentOrientation.Theta_pitchDegrees)} };


            return rotationMatrix;
        }

        public  static double[,] calculateEcefToNEDMatrix(Position referenceEcefPositionGeodedic)
        {
            //lambda = long, phi = lat
            double[,] rotationMatrix = new double[,] { 
            { -1 * Sin(referenceEcefPositionGeodedic.XLatitudePosition) * Cos(referenceEcefPositionGeodedic.YLongitudePosition), -1 * Sin(referenceEcefPositionGeodedic.XLatitudePosition) * Sin(referenceEcefPositionGeodedic.YLongitudePosition), Cos(referenceEcefPositionGeodedic.XLatitudePosition)  }, 
            { -1 * Sin (referenceEcefPositionGeodedic.YLongitudePosition), Cos(referenceEcefPositionGeodedic.YLongitudePosition), 0}, 
            { -1 * Cos(referenceEcefPositionGeodedic.XLatitudePosition) * Cos(referenceEcefPositionGeodedic.YLongitudePosition), -1 * Cos(referenceEcefPositionGeodedic.XLatitudePosition)*Sin(referenceEcefPositionGeodedic.YLongitudePosition), -1 * Sin(referenceEcefPositionGeodedic.XLatitudePosition)} };


            return rotationMatrix;
        }




        public static double[] ConvertFromGeodeticToECEF(double longitude_lambda, double latitude_phi, double altitude_h)
        {
            double long_rad = longitude_lambda * (Math.PI / 180.0);
            double lat_rad = latitude_phi * (Math.PI / 180.0);

            
            double E = 0.08181919;
            double Ne = calculateNe(E, lat_rad);

            //WARNING, RAD'S VS. DEGREES?
            double Xe = (Ne + altitude_h) * Math.Cos(lat_rad) * Math.Cos(long_rad);
            double Ye = (Ne + altitude_h) * Math.Cos(lat_rad) * Math.Sin(long_rad);
            double Ze = ((Ne * (1 - Math.Pow(E, 2))) + altitude_h) * Math.Sin(lat_rad);

            return new double[]  { Xe, Ye, Ze };
        }

        static double Rea = 6378137.0;
        private static double calculateNe(double E, double latitude_phi)
        {
            double Ne = Rea / Math.Sqrt(1 - Math.Pow(E, 2) * Math.Pow(Math.Sin(latitude_phi), 2));

            return Ne;
        }

        public static double[,] GetRotationMatrixFromBodyToInertialFrame(Orientation orientation)
        {
            //TODO Handle for the case where you hit a singularity at +-Pi/2
            double[,] rotationMatrixBodyToInertial = new double[,] { 
            { Sin(orientation.Theta_pitchDegrees) * Cos(orientation.Sai_yawDegrees), Sin(orientation.Phi_rollDegrees) * Sin(orientation.Theta_pitchDegrees) * Cos(orientation.Sai_yawDegrees) - Cos(orientation.Phi_rollDegrees) * Sin(orientation.Sai_yawDegrees), Cos(orientation.Phi_rollDegrees) * Sin(orientation.Theta_pitchDegrees) * Cos(orientation.Sai_yawDegrees) + Sin(orientation.Phi_rollDegrees) * Sin(orientation.Sai_yawDegrees) }, 
            { Cos(orientation.Theta_pitchDegrees) * Sin(orientation.Sai_yawDegrees), Sin(orientation.Phi_rollDegrees) * Sin(orientation.Theta_pitchDegrees) * Sin(orientation.Sai_yawDegrees) + Cos(orientation.Phi_rollDegrees) * Cos(orientation.Sai_yawDegrees), Cos(orientation.Phi_rollDegrees) * Sin(orientation.Theta_pitchDegrees) * Sin(orientation.Sai_yawDegrees) - Sin(orientation.Phi_rollDegrees) * Cos(orientation.Sai_yawDegrees) }, 
            { Sin(orientation.Theta_pitchDegrees) * -1, Sin(orientation.Phi_rollDegrees) * Cos(orientation.Theta_pitchDegrees), Cos(orientation.Phi_rollDegrees) * Cos(orientation.Theta_pitchDegrees) } };

            return rotationMatrixBodyToInertial;
        }

        public static double[,] GetRotationMatrixFromInertialToBodyFrame(Orientation orientation)
        {
            //TODO Handle for the case where you hit a singularity at +-Pi/2
            double[,] rotationMatrixBodyToInertial = GetRotationMatrixFromBodyToInertialFrame(orientation);
            double[,] rotationMatrixInertialToBody = new double[,]{{0,0,0},{0,0,0},{0,0,0}};

            for (int i = 0; i < rotationMatrixBodyToInertial.GetLength(0); i++)
            {
                for (int j = 0; j < rotationMatrixBodyToInertial.GetLength(0); j++)
                {
                    rotationMatrixInertialToBody[j, i] = rotationMatrixBodyToInertial[i, j];
                }
            }

            return rotationMatrixInertialToBody;
        }

        public static Position RotatePositionFromInertialFrameToBodyFrame(Orientation orientation, Position currentPosition)
        {
            Position newPosition = new Position();

            double[,] rotationMatrix = GetRotationMatrixFromInertialToBodyFrame(orientation);
            double[,] positionMatrix = currentPosition.GetPositionMatrix();

            double[,] rotatedPosition = new double[,] { { 0 }, { 0 }, { 0 } };

            /*rotatedPosition[0,0] = rotationMatrix[0,0]*positionMatrix[0,0] + rotationMatrix[0,1]*positionMatrix[1,0] + rotationMatrix[0,2]*positionMatrix[2,0];
            rotatedPosition[1,0] = rotationMatrix[1,0]*positionMatrix[0,0] + rotationMatrix[1,1]*positionMatrix[1,0] + rotationMatrix[1,2]*positionMatrix[2,0];
            rotatedPosition[2,0] = rotationMatrix[2,0]*positionMatrix[0,0] + rotationMatrix[2,1]*positionMatrix[1,0] + rotationMatrix[2,2]*positionMatrix[2,0];
            */

            //for each column in rotation matrix through columns
            for (int i = 0; i < rotationMatrix.GetLength(1); i++)
            {

                for (int j = 0; j < positionMatrix.GetLength(0); j++)
                {
                    rotatedPosition[i, 0] += rotationMatrix[i, j] * positionMatrix[j, 0];
                }
            }

            newPosition.XLatitudePosition = rotatedPosition[0, 0];
            newPosition.YLongitudePosition = rotatedPosition[1, 0];
            newPosition.ZAltitudePosition = rotatedPosition[2, 0];

            return newPosition;
        }
    }
}
