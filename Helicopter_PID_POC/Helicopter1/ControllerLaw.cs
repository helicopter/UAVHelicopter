using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;


namespace Helicopter1
{
    public class ControlLaw
    {


        private static int MAXNUMBEROFHISTORICALERRORS = 1000;

        Position initialReferenceStartingPositionEFEC = null;
        Position initialReferenceStartingPositionGeodedic = null;

        Measurement currentMeasurementNED = null;
        Measurement previousMeasurementNED = null;
        Measurement referenceMeasurementNED = null;
        //Position previousErrorPositionBodyFrame = new Position();
        //Position previousPreviousErrorPositionBodyFrame = new Position();

        double ulon_MainRotorLongitudinalCyclicControlCommand_Pitch = 0;
        double ulat_MainRotorLateralCyclicControlCommand_Roll = 0;
        double ucol_MainRotorCollective_BladeAngle = 0;
        double uped_TailRotorPedal_BladeAngleOnTail = 0;

        public ControlLaw()
        {
            currentMeasurementNED = new Measurement();
            previousMeasurementNED = new Measurement();
            referenceMeasurementNED = new Measurement();
        }


        double integralOfZBodyAltitude = 0.0;
        double integralOfYaw = 0.0;
        double integralOfXBodyLatitude = 0.0;
        double integralOfYBodyLongitude = 0.0;

        bool firstMeasurement = true;

        Queue<double> yawErrors = new Queue<double>();

        bool isGoingClockwise = true;

        internal void processMeasurement(Measurement newmeasurementgeodedic, System.IO.StreamWriter file)
        {

            //Need to convert new measurement into correct frame.
            //lets do this according to the doc. First we go from geodedic to EFEC, then to inertial, then we do some math
            //and convert to body. 
            //so basically get newmeasurement into NED.
            //I also have to ensure that the reference measurement gets properly transformed to local NED

            /**
             * 1. ensure that the new measurement gets transformed to local NED
             * 2. ensure that the first new measurement gets set to previous measurement
             * 3. ensure that the reference measurement gets transformed to local NED
             * 4. Ensure that the takeoff point is transformed to ECEF then used for REF for other conversions to NED
             * 5. make sure appropriate rotations are made for local NED to body frame.
             * 6. ensure body frame still applies because it said stuff about body only being used for velocity. 
             */
            Measurement newMeasurementLocalNED = newmeasurementgeodedic.ConvertFromGeodedicToLocalNED(initialReferenceStartingPositionEFEC,
                initialReferenceStartingPositionGeodedic);

            if (firstMeasurement)
            {
                firstMeasurement = false;
                previousMeasurementNED = newMeasurementLocalNED.Copy();
                currentMeasurementNED = newMeasurementLocalNED.Copy();


                return;
            }
            else
            {
                previousMeasurementNED = currentMeasurementNED.Copy();
                currentMeasurementNED = newMeasurementLocalNED.Copy();
            }


            Position errorPositionNED = referenceMeasurementNED.Position.SubtractPosition(newMeasurementLocalNED.Position);
            Measurement temp = newMeasurementLocalNED.Copy();
            temp.Position = errorPositionNED;
            Position errorPositionBodyFrame = temp.ConvertFromLocalNEDToBodyFrame().Position;
            

            //*********
            //Calculate integrals
            //**********
            integralOfXBodyLatitude = integralOfXBodyLatitude + errorPositionBodyFrame.XLatitudePosition;
            //double integralOfYBodyLongitude = calculateIntegral(1);
            integralOfYBodyLongitude = integralOfYBodyLongitude + errorPositionBodyFrame.YLongitudePosition;
            integralOfZBodyAltitude = integralOfZBodyAltitude + errorPositionBodyFrame.ZAltitudePosition;



            Measurement currentmeasurementBodyFrame = currentMeasurementNED.ConvertFromLocalNEDToBodyFrame();
            Measurement previousmeasurementBodyFrame = previousMeasurementNED.ConvertFromLocalNEDToBodyFrame();

            //**********
            //Calculate Velocity
            //**********
            double xVelocityBodyFrame = (currentmeasurementBodyFrame.Position.XLatitudePosition - previousmeasurementBodyFrame.Position.XLatitudePosition) / Util.TIMEBETWEENINTERVALS;
            double yVelocityBodyFrame = (currentmeasurementBodyFrame.Position.YLongitudePosition - previousmeasurementBodyFrame.Position.YLongitudePosition) / Util.TIMEBETWEENINTERVALS;
            double zVelocityBodyFrame = (currentmeasurementBodyFrame.Position.ZAltitudePosition - previousmeasurementBodyFrame.Position.ZAltitudePosition) / Util.TIMEBETWEENINTERVALS;


            
            //Body and ned comparison works here because the velocity for the reference was already in body frame. (m/s)
            double velocityErrorX = xVelocityBodyFrame - referenceMeasurementNED.Velocity.XVelocityBodyFrame;
            double velocityErrorY = yVelocityBodyFrame - referenceMeasurementNED.Velocity.YVelocityBodyFrame;
            double velocityErrorZ = zVelocityBodyFrame - referenceMeasurementNED.Velocity.ZVelocityBodyFrame;





            //***************
            //Calculate weighted values
            //**************

            //Calculate weighted integrals
            double weightedIntegralOfXBodyLatitude = integralOfXBodyLatitude * Util.K_ETA_X;
            double weightedIntegralOfYBodyLongitude = integralOfYBodyLongitude * Util.K_ETA_Y;
            double weightedIntegralOfZBodyAltitude = integralOfZBodyAltitude * Util.K_ETA_Z;

            //Calculate weighted proportional values
            double weightedProportionalErrorX = errorPositionBodyFrame.XLatitudePosition * Util.K_X;
            double weightedProportionalErrorY = errorPositionBodyFrame.YLongitudePosition * Util.K_Y;
            double weightedProportionalErrorZ = errorPositionBodyFrame.ZAltitudePosition * Util.K_Z;

            double weightedDerivativeVelocityX = velocityErrorX * Util.K_U;
            double weightedDerivativeVelocityY = velocityErrorY * Util.K_V;
            double weightedDerivativeVelocityZ = velocityErrorZ * Util.K_W;



            Console.Write(weightedIntegralOfXBodyLatitude + ", " + weightedProportionalErrorX + ", " + weightedDerivativeVelocityX + "\r");
            //Calculate Proportional, Integral, Derivative.
            double pidX_thetaDes = weightedIntegralOfXBodyLatitude + weightedProportionalErrorX + weightedDerivativeVelocityX;
            double pidY_phiDes = weightedIntegralOfYBodyLongitude + weightedProportionalErrorY + weightedDerivativeVelocityY;
            double pidZ = -1 * weightedIntegralOfZBodyAltitude - weightedProportionalErrorZ - weightedDerivativeVelocityZ;


            ulon_MainRotorLongitudinalCyclicControlCommand_Pitch = -1 * Util.K_THETA * (newMeasurementLocalNED.Orientation.Theta_pitchDegrees - pidX_thetaDes);
            ulat_MainRotorLateralCyclicControlCommand_Roll = -1 * Util.K_PHI * (newMeasurementLocalNED.Orientation.Phi_rollDegrees + pidY_phiDes); 
            ucol_MainRotorCollective_BladeAngle = pidZ;


            /**
             * yaw processing
             */
            /*double yawError = newMeasurementLocalNED.Orientation.Sai_yawDegrees - referenceMeasurementNED.Orientation.Sai_yawDegrees;
            if (yawError > 180)
            {
                yawError = yawError - 360;
            }
            else if (yawError < -180)
            {
                yawError = yawError + 360;
            }*/

            //double yawError = newMeasurementLocalNED.Orientation.Sai_yawDegrees - referenceMeasurementNED.Orientation.Sai_yawDegrees;
            //if the current measurement is < ref measure and previous measurement > ref measurement
            //   then you crossed the threshold going the counter clockwise direction
            //       the error should be 0-360 positive
            //else if the current measurement is > ref measurement, and previous measurement < ref measurement
            //  then you crossed the threshold going the clockwise direction.
            //        the error should be 0-360 negative.

            if (newMeasurementLocalNED.Orientation.Sai_yawDegrees < referenceMeasurementNED.Orientation.Sai_yawDegrees &&
                previousMeasurementNED.Orientation.Sai_yawDegrees > referenceMeasurementNED.Orientation.Sai_yawDegrees)
            {
                //Boolean going clockwise = false
                isGoingClockwise = false;
            }else if (newMeasurementLocalNED.Orientation.Sai_yawDegrees > referenceMeasurementNED.Orientation.Sai_yawDegrees &&
                previousMeasurementNED.Orientation.Sai_yawDegrees < referenceMeasurementNED.Orientation.Sai_yawDegrees)
            {
                isGoingClockwise = true;
            }

            double yawError = 0.0;
            if (isGoingClockwise)
            {
                yawError = newMeasurementLocalNED.Orientation.Sai_yawDegrees - referenceMeasurementNED.Orientation.Sai_yawDegrees;
            }
            else
            {
                yawError = newMeasurementLocalNED.Orientation.Sai_yawDegrees - referenceMeasurementNED.Orientation.Sai_yawDegrees;
            }

            yawErrors.Enqueue(yawError);

            if (yawErrors.Count > 40)
            {
                yawErrors.Dequeue();
            }

            


            integralOfYaw = integralOfYaw + yawError;

            double weightedIntegralOfYawError = integralOfYaw * Util.K_ETA_SAI;
            double weightedProportionalOfYawError = yawError * Util.K_SAI;



            //double derivativeOfYaw = (newMeasurementLocalNED.Orientation.Sai_yawDegrees - previousMeasurementNED.Orientation.Sai_yawDegrees) / Util.TIMEBETWEENINTERVALS;
            double derivativeOfYaw = newMeasurementLocalNED.Velocity.YawVelocity;
            

            double yawDerivativeError = referenceMeasurementNED.Velocity.YawVelocity - derivativeOfYaw;

            //double weightedYawDerivativeError = yawDerivativeError * Util.K_R;
            double weightedYawDerivativeError = referenceMeasurementNED.Velocity.YawVelocity * Util.K_R;


            //control law from: http://books.google.com/books?id=Xqzv_wXqC5EC&pg=PA516&lpg=PA516&dq=pid+controller+tail+rotor&source=bl&ots=PxlTw8tb_Y&sig=u0iBG5m6hV4VeY1yM9ApTPHtO2Q&hl=en#v=onepage&q=pid%20controller%20tail%20rotor&f=false

            uped_TailRotorPedal_BladeAngleOnTail = -1 * weightedIntegralOfYawError - weightedProportionalOfYawError - weightedYawDerivativeError;

       /*     file.WriteLine(newmeasurement.Orientation.Sai_yawDegrees + ", " + referenceMeasurementNED.Orientation.Sai_yawDegrees + ", " +
           yawError + ", " + weightedProportionalOfYawError);*/
            
            
       //     Console.Write(weightedIntegralOfZBodyAltitude + ", " + weightedProportionalErrorZ + ", " + weightedDerivativeVelocityZ + "\r");

           
        }

        float previousuped = (float) 0.0;


        /*private double calculateIntegral(double currentError, double previousError, double previousPreviousError)
        {
            return ((previousPreviousError + previousError) + ((previousError + currentError) / 2)) * Util.TIMEBETWEENINTERVALS;
        }*/

        /// <summary>
        /// Controls the pitch angle of the helicopter by adjusting the
        /// longitudinal motion of the blades (tilts the rotor forward or backward)
        /// </summary>
        internal double Getulon_MainRotorLongitudinalCyclicControlCommand_Pitch()
        {
            return ulon_MainRotorLongitudinalCyclicControlCommand_Pitch;
        }

        /// <summary>
        /// Controls the roll angle of the helicopter by adjusting the main rotor
        /// laterally, by tilting the main rotor to the left or right
        /// </summary>
        public double Getulat_MainRotorLateralCyclicControlCommand_Roll()
        {
            return ulat_MainRotorLateralCyclicControlCommand_Roll;
        }

        public double Getucol_MainRotorCollectiveControlCommand()
        {
            return ucol_MainRotorCollective_BladeAngle;
        }

        public double Getuped_TailRotorPedalControlCommand()
        {
            return uped_TailRotorPedal_BladeAngleOnTail;
        }


        Measurement referenceJUNK = null;
        internal void setReferenceMeasurement(Measurement referenceMeasurement)
        {
            referenceJUNK = referenceMeasurement;
            this.referenceMeasurementNED = referenceMeasurement.ConvertFromGeodedicToLocalNED(initialReferenceStartingPositionEFEC, initialReferenceStartingPositionGeodedic);
        }



        internal void SetInitialReferenceStartingPosition(Position intialStartingReferencePositionGeodedic)
        {
            initialReferenceStartingPositionEFEC = new Position();
            initialReferenceStartingPositionGeodedic = intialStartingReferencePositionGeodedic;


            double[] ecef = Util.ConvertFromGeodeticToECEF(intialStartingReferencePositionGeodedic.YLongitudePosition, intialStartingReferencePositionGeodedic.XLatitudePosition, intialStartingReferencePositionGeodedic.ZAltitudePosition);
            initialReferenceStartingPositionEFEC.XLatitudePosition = ecef[0];
            initialReferenceStartingPositionEFEC.YLongitudePosition = ecef[1];
            initialReferenceStartingPositionEFEC.ZAltitudePosition = ecef[2];
        }
    }
}
