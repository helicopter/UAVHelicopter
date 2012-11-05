using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using WindowsFormsApplication1;
using System.Windows.Forms;

namespace Helicopter1
{
    public class ControlLaw
    {

        bool isGoingClockwise = true;

        private static int MAXNUMBEROFHISTORICALERRORS = 1000;

        Position initialReferenceStartingPositionEFEC = null;
        Position initialReferenceStartingPositionGeodedic = null;

        Measurement currentMeasurementNED = null;
        Measurement previousMeasurementNED = null;
        public static Measurement referenceMeasurementNED = new Measurement();
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
         //   referenceMeasurementNED = new Measurement();
        }


        double integralOfZBodyAltitude = 0.0;
        double integralOfYaw = 0.0;
        double integralOfXBodyLatitude = 0.0;
        double integralOfYBodyLongitude = 0.0;

        bool firstMeasurement = true;

        Queue<double> yawErrors = new Queue<double>();
        Queue<double> derivativeYaw = new Queue<double>();
        Queue<double> integralYaw = new Queue<double>();
        Queue<double> Ped = new Queue<double>();

        double previousCollectiveActuatorError = 0.0;

        double previousYVelocityBodyFrame = 0.0;
        double previousXVelocityBodyFrame = 0.0;

        internal void processMeasurement(Measurement newmeasurementgeodedic, System.IO.StreamWriter file, System.IO.StreamWriter file2)
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



           /* Measurement destinationBody = referenceMeasurementNED.Copy();
            destinationBody.Orientation = newMeasurementLocalNED.Orientation;
            destinationBody = destinationBody.ConvertFromLocalNEDToBodyFrame();

            Measurement newMeasurementBody = newMeasurementLocalNED.Copy();
            newMeasurementBody = newMeasurementBody.ConvertFromLocalNEDToBodyFrame();
            */

          //  Position errorPositionBodyFrame = destinationBody.Position.SubtractPosition(newMeasurementBody.Position);
            
            
            Position errorPositionNED = referenceMeasurementNED.Position.SubtractPosition(newMeasurementLocalNED.Position);
            Measurement temp = newMeasurementLocalNED.Copy();
            temp.Position = errorPositionNED;
            Position errorPositionBodyFrame = temp.ConvertFromLocalNEDToBodyFrame().Position;
        errorPositionBodyFrame.ZAltitudePosition = errorPositionNED.ZAltitudePosition;
            

            //*********
            //Calculate integrals
            //**********
            integralOfXBodyLatitude = integralOfXBodyLatitude + errorPositionBodyFrame.XLatitudePosition * Util.TIMEBETWEENINTERVALS;
            //double integralOfYBodyLongitude = calculateIntegral(1);
            integralOfYBodyLongitude = integralOfYBodyLongitude + errorPositionBodyFrame.YLongitudePosition * Util.TIMEBETWEENINTERVALS;
       //     integralOfZBodyAltitude = integralOfZBodyAltitude + errorPositionBodyFrame.ZAltitudePosition;



            Measurement currentmeasurementBodyFrame = currentMeasurementNED.ConvertFromLocalNEDToBodyFrame();
            Measurement previousmeasurementBodyFrame = previousMeasurementNED.ConvertFromLocalNEDToBodyFrame();

            //**********
            //Calculate Velocity
            //**********
           // double xVelocityBodyFrame = (currentmeasurementBodyFrame.Position.XLatitudePosition - previousmeasurementBodyFrame.Position.XLatitudePosition) / Util.TIMEBETWEENINTERVALS;
            //double yVelocityBodyFrame = (currentmeasurementBodyFrame.Position.YLongitudePosition - previousmeasurementBodyFrame.Position.YLongitudePosition) / Util.TIMEBETWEENINTERVALS;

            double xVelocityBodyFrame = currentmeasurementBodyFrame.Velocity.XVelocityBodyFrame;
            double yVelocityBodyFrame = currentmeasurementBodyFrame.Velocity.YVelocityBodyFrame;

            

            //Note: i'm not using body frame z velocity because the error isn't in body frame. I'm rationalizing this because for z we are not
            //producing an angle.
            //double zVelocityBodyFrame = (currentmeasurementBodyFrame.Position.ZAltitudePosition - previousmeasurementBodyFrame.Position.ZAltitudePosition) / Util.TIMEBETWEENINTERVALS;
          //  double zVelocityBodyFrame = (currentMeasurementNED.Position.ZAltitudePosition - previousMeasurementNED.Position.ZAltitudePosition) / Util.TIMEBETWEENINTERVALS;
            double zVelocityBodyFrame = currentmeasurementBodyFrame.Velocity.ZVelocityBodyFrame;

            
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



        
        //    double weightedIntegralOfZBodyAltitude = integralOfZBodyAltitude * Util.K_ETA_Z;

            //Calculate weighted proportional values
            double weightedProportionalErrorX = errorPositionBodyFrame.XLatitudePosition * Util.K_X;
            double weightedProportionalErrorY = errorPositionBodyFrame.YLongitudePosition * Util.K_Y;
            double weightedProportionalErrorZ = errorPositionBodyFrame.ZAltitudePosition * Util.K_Z;

            double weightedDerivativeVelocityX = velocityErrorX * Util.K_U;
            double weightedDerivativeVelocityY = velocityErrorY * Util.K_V;
            double weightedDerivativeVelocityZ = velocityErrorZ * Util.K_W;

            /*
            weightedDerivativeVelocityX = 0;
            weightedDerivativeVelocityY = 0;
            weightedDerivativeVelocityZ = 0;
            */


            /* file2.WriteLine("weightedIntegralOfXBodyLatitude," + weightedIntegralOfXBodyLatitude + ",weightedIntegralOfYBodyLongitude," + weightedIntegralOfYBodyLongitude +
                 ",weightedIntegralOfZBodyAltitude," + weightedIntegralOfZBodyAltitude + ",weightedProportionalErrorX," +weightedProportionalErrorX+
                 ",weightedProportionalErrorY," + weightedProportionalErrorY + ",weightedProportionalErrorZ," + weightedProportionalErrorZ+
                 ",weightedDerivativeVelocityX," + weightedDerivativeVelocityX + ",weightedDerivativeVelocityY,"+weightedDerivativeVelocityY+",weightedDerivativeVelocityZ," +
                 weightedDerivativeVelocityZ
                  + ", phi_roll: " + newMeasurementLocalNED.Orientation.Phi_rollDegrees + ", Sai_yaw: " + newMeasurementLocalNED.Orientation.Sai_yawDegrees + ", theta_pitch" + newMeasurementLocalNED.Orientation.Theta_pitchDegrees

                  ); */



            //HACK: CRAP! X CONTROLS THETA WHICH CONTROLS LONGITUDE, NOT LATITUDE.
            Console.Write(weightedIntegralOfXBodyLatitude + ", " + weightedProportionalErrorX + ", " + weightedDerivativeVelocityX + "\r");
            //Calculate Proportional, Integral, Derivative.
            double pidX_thetaDes = weightedIntegralOfXBodyLatitude + weightedProportionalErrorX + weightedDerivativeVelocityX;
            double pidY_phiDes = weightedIntegralOfYBodyLongitude + weightedProportionalErrorY + weightedDerivativeVelocityY;






            double integralOfZBodyAltitudeTemp = integralOfZBodyAltitude + errorPositionBodyFrame.ZAltitudePosition * Util.TIMEBETWEENINTERVALS;
            double weightedIntegralOfZBodyAltitudeTemp = integralOfZBodyAltitudeTemp * Util.K_ETA_Z;
            double pidZTemp = -1 * weightedIntegralOfZBodyAltitudeTemp - weightedProportionalErrorZ - weightedDerivativeVelocityZ;
            //pidZTemp = -1 * pidZTemp / 10.0;
            pidZTemp = -1 * pidZTemp;
            double pidZ = 0.0;
            double weightedIntegralOfZBodyAltitude = 0.0;

            if (pidZTemp > Util.UCOLMAXCONTROLVALUE || pidZTemp < Util.UCOLMINCONTROLVALUE)
            {
                //integralOfZBodyAltitude = 0;

                if (pidZTemp > Util.UCOLMAXCONTROLVALUE)
                {
                    if (integralOfZBodyAltitude > 0)
                    {
                        integralOfZBodyAltitude = integralOfZBodyAltitude - Util.Ti_Z * (pidZTemp - Util.UCOLMAXCONTROLVALUE);
                    }
                    
                }
                if (pidZTemp < Util.UCOLMINCONTROLVALUE)
                {
                    if (integralOfZBodyAltitude < 0)
                    {
                        integralOfZBodyAltitude = integralOfZBodyAltitude - Util.Ti_Z * (pidZTemp - Util.UCOLMINCONTROLVALUE);
                    }
                    
                }


                //integrator windup preventor
                //integralOfZBodyAltitude = integralOfZBodyAltitude + errorPositionBodyFrame.ZAltitudePosition;
                weightedIntegralOfZBodyAltitude = integralOfZBodyAltitude * Util.K_ETA_Z;
                
                pidZ = -1 * weightedIntegralOfZBodyAltitude - weightedProportionalErrorZ - weightedDerivativeVelocityZ;
                //pidZ = -1 * pidZ / 10.0;
                pidZ = -1 * pidZ;
            }
            else
            {
                //IMPORTANT NOTE: i'M NOT multiplying by time interval because the performance is much better without doing that.
                //integralOfZBodyAltitude = integralOfZBodyAltitude + errorPositionBodyFrame.ZAltitudePosition * Util.TIMEBETWEENINTERVALS;
                integralOfZBodyAltitude = integralOfZBodyAltitude + errorPositionBodyFrame.ZAltitudePosition;


                weightedIntegralOfZBodyAltitude = integralOfZBodyAltitude * Util.K_ETA_Z;
                pidZ = -1 * weightedIntegralOfZBodyAltitude - weightedProportionalErrorZ - weightedDerivativeVelocityZ;
                //pidZ = -1 * pidZ / 10.0;
                pidZ = -1 * pidZ;
            }






            /*if (pidX_thetaDes > 16) pidX_thetaDes = 16;
            if (pidX_thetaDes < -16) pidX_thetaDes = -16;*/
            if (pidX_thetaDes > 13) pidX_thetaDes = 13;
            if (pidX_thetaDes < -13) pidX_thetaDes = -13;
            /*if (pidX_thetaDes > 6) pidX_thetaDes = 6;
            if (pidX_thetaDes < -2) pidX_thetaDes = -2;*
            /*if (pidY_phiDes > 7) pidY_phiDes = 7;
            if (pidY_phiDes < -7) pidY_phiDes = -7;*/
            if (pidY_phiDes > 10) pidY_phiDes = 10;
            if (pidY_phiDes < -10) pidY_phiDes = -10;
          /*  if (pidY_phiDes > 3) pidY_phiDes = 3;
            if (pidY_phiDes < -3) pidY_phiDes = -3;*/







            ulon_MainRotorLongitudinalCyclicControlCommand_Pitch = -1 * ((Util.K_THETA * (newMeasurementLocalNED.Orientation.Theta_pitchDegrees - pidX_thetaDes)) + newMeasurementLocalNED.Velocity.PitchVelocityDeg * Util.K_PitchVelocity);
            ulat_MainRotorLateralCyclicControlCommand_Roll = -1 * ((Util.K_PHI * (newMeasurementLocalNED.Orientation.Phi_rollDegrees + pidY_phiDes)) + (newMeasurementLocalNED.Velocity.RollVelocityDeg * Util.K_RollVelocity)); 
            ucol_MainRotorCollective_BladeAngle = pidZ;


            

            file.WriteLine("xlat: ," + newMeasurementLocalNED.Position.XLatitudePosition + ", ylong: ," + newMeasurementLocalNED.Position.YLongitudePosition + ", zalt: ," +
newMeasurementLocalNED.Position.ZAltitudePosition + ", geolat: ," + newmeasurementgeodedic.Position.XLatitudePosition + ", geolong: ," + newmeasurementgeodedic.Position.YLongitudePosition + ", geoalt: ," + newmeasurementgeodedic.Position.ZAltitudePosition
+ ", Errorx: ," + errorPositionNED.XLatitudePosition + ", Errory: ," + errorPositionNED.YLongitudePosition + ", Errorz ," + errorPositionNED.ZAltitudePosition + ", errorbodx: ," + errorPositionBodyFrame.XLatitudePosition
+ ", errorbody: ," + errorPositionBodyFrame.YLongitudePosition
 + ", errorbodz: ," + errorPositionBodyFrame.ZAltitudePosition
 + ", pidxthetaDes: ," + pidX_thetaDes
 + ", pidY_phiDes: ," + pidY_phiDes
 + ", pixZ: ," + pidZ

 + ", phi_roll: ," + newMeasurementLocalNED.Orientation.Phi_rollDegrees + ", Sai_yaw: ," + newMeasurementLocalNED.Orientation.Sai_yawDegrees + ", theta_pitch," + newMeasurementLocalNED.Orientation.Theta_pitchDegrees
 + ", ulon_pitch: ," + ulon_MainRotorLongitudinalCyclicControlCommand_Pitch
 + ", ulat_roll: ," + ulat_MainRotorLateralCyclicControlCommand_Roll
 + ", collective: ," + ucol_MainRotorCollective_BladeAngle
                 + ", errorbodyx, " + errorPositionBodyFrame.XLatitudePosition
                 + ", errorbodyy," + errorPositionBodyFrame.YLongitudePosition
                 + ", errorbodyz," + errorPositionBodyFrame.ZAltitudePosition
                 + "weightedIntegralOfXBodyLatitude," + weightedIntegralOfXBodyLatitude + ",weightedIntegralOfYBodyLongitude," + weightedIntegralOfYBodyLongitude +
                ",weightedIntegralOfZBodyAltitude," + weightedIntegralOfZBodyAltitude + ",weightedProportionalErrorX," + weightedProportionalErrorX +
                ",weightedProportionalErrorY," + weightedProportionalErrorY + ",weightedProportionalErrorZ," + weightedProportionalErrorZ +
                ",weightedDerivativeVelocityX," + weightedDerivativeVelocityX + ",weightedDerivativeVelocityY," + weightedDerivativeVelocityY + ",weightedDerivativeVelocityZ," +
                weightedDerivativeVelocityZ
                 + ", phi_roll: " + newMeasurementLocalNED.Orientation.Phi_rollDegrees + ", Sai_yaw: " + newMeasurementLocalNED.Orientation.Sai_yawDegrees + ", theta_pitch" + newMeasurementLocalNED.Orientation.Theta_pitchDegrees
 
 );



            //yawErrors.Enqueue(weightedProportionalErrorX);
           // yawErrors.Enqueue(weightedProportionalErrorZ);
           // yawErrors.Enqueue(errorPositionNED.YLongitudePosition);
            yawErrors.Enqueue(errorPositionNED.XLatitudePosition);
        //    derivativeYaw.Enqueue(weightedDerivativeVelocityY);
         //   derivativeYaw.Enqueue(weightedDerivativeVelocityX);
         //   derivativeYaw.Enqueue(weightedDerivativeVelocityZ);
            derivativeYaw.Enqueue(errorPositionNED.YLongitudePosition);
            
        //    integralYaw.Enqueue(weightedIntegralOfYBodyLongitude);
         //   integralYaw.Enqueue(weightedIntegralOfYBodyLongitude);
         //   integralYaw.Enqueue(weightedIntegralOfXBodyLatitude);
            //integralYaw.Enqueue(errorPositionNED.ZAltitudePosition);
            integralYaw.Enqueue(errorPositionNED.ZAltitudePosition);
            
            
          //  Ped.Enqueue(ulat_MainRotorLateralCyclicControlCommand_Roll);
            Ped.Enqueue(ucol_MainRotorCollective_BladeAngle);

            if (yawErrors.Count > 400)
            {
                yawErrors.Dequeue();
                derivativeYaw.Dequeue();
                integralYaw.Dequeue();
                Ped.Dequeue();
            }
            List<double> myList = new List<double>();
            myList.AddRange(yawErrors);
            Form1.plotLineGraphChart1(myList);
            myList = new List<double>();
            myList.AddRange(derivativeYaw);
            Form1.plotLineGraph(myList, Form1.chart2);
            myList = new List<double>();
            myList.AddRange(integralYaw);
            Form1.plotLineGraph(myList, Form1.chart3);
            myList = new List<double>();
            myList.AddRange(Ped);
            Form1.plotLineGraph(myList, Form1.chart4);
            

            /**
             * yaw processing
             */
            double yawError = newMeasurementLocalNED.Orientation.Sai_yawDegrees - referenceMeasurementNED.Orientation.Sai_yawDegrees;
            if (yawError > 180)
            {
                yawError = yawError - 360;
            }
            else if (yawError < -180)
            {
                yawError = yawError + 360;
            }

            /*
            if (newMeasurementLocalNED.Orientation.Sai_yawDegrees < referenceMeasurementNED.Orientation.Sai_yawDegrees &&
                previousMeasurementNED.Orientation.Sai_yawDegrees > referenceMeasurementNED.Orientation.Sai_yawDegrees)
            {
                //Boolean going clockwise = false
                isGoingClockwise = false;
            }
            else if (newMeasurementLocalNED.Orientation.Sai_yawDegrees > referenceMeasurementNED.Orientation.Sai_yawDegrees &&
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
            */
            

            /*if (yawErrors.Count > 40)
            {
                yawErrors.Dequeue();
            }*/




            /**
             * HORRIBLE WAY OF DOING INTEGRAL ANTI-WINDUP
             */

            integralOfYaw = integralOfYaw + yawError * Util.TIMEBETWEENINTERVALS;

            double weightedIntegralOfYawError = integralOfYaw * Util.K_ETA_SAI;
            double weightedProportionalOfYawError = yawError * Util.K_SAI;



            //double derivativeOfYaw = (newMeasurementLocalNED.Orientation.Sai_yawDegrees - previousMeasurementNED.Orientation.Sai_yawDegrees) / Util.TIMEBETWEENINTERVALS;
            double derivativeOfYaw = newMeasurementLocalNED.Velocity.YawVelocity;
            

            //double yawDerivativeError = referenceMeasurementNED.Velocity.YawVelocity - derivativeOfYaw;
            double yawDerivativeError = derivativeOfYaw - referenceMeasurementNED.Velocity.YawVelocity; //it goes out of control the other way.

            //double weightedYawDerivativeError = yawDerivativeError * Util.K_R;
            //double weightedYawDerivativeError = referenceMeasurementNED.Velocity.YawVelocity * Util.K_R;
            //double weightedYawDerivativeError = currentMeasurementNED.Velocity.YawVelocity * Util.K_R;
            double weightedYawDerivativeError = yawDerivativeError * Util.K_R;

            //control law from: http://books.google.com/books?id=Xqzv_wXqC5EC&pg=PA516&lpg=PA516&dq=pid+controller+tail+rotor&source=bl&ots=PxlTw8tb_Y&sig=u0iBG5m6hV4VeY1yM9ApTPHtO2Q&hl=en#v=onepage&q=pid%20controller%20tail%20rotor&f=false


//******** note added a * -1 here.
            uped_TailRotorPedal_BladeAngleOnTail = (-1 * weightedIntegralOfYawError - weightedProportionalOfYawError - weightedYawDerivativeError) * -1;


            if (uped_TailRotorPedal_BladeAngleOnTail > Util.UPEDMAXCONTROLVALUE ||
                uped_TailRotorPedal_BladeAngleOnTail < Util.UPEDMINCONTROLVALUE)
            {
                if (uped_TailRotorPedal_BladeAngleOnTail > Util.UPEDMAXCONTROLVALUE)
                {
                    if (integralOfYaw > 0)
                    {
                        integralOfYaw = integralOfYaw - Util.Ti_SAI * (uped_TailRotorPedal_BladeAngleOnTail - Util.UPEDMAXCONTROLVALUE);
                    }

                }
                if (integralOfYaw < Util.UPEDMINCONTROLVALUE)
                {
                    if (integralOfYaw < 0)
                    {
                        integralOfYaw = integralOfYaw - Util.Ti_SAI * (uped_TailRotorPedal_BladeAngleOnTail - Util.UPEDMINCONTROLVALUE);
                    }

                }






     //           integralOfYaw = integralOfYaw - yawError * Util.TIMEBETWEENINTERVALS;
            //    integralOfYaw = 0;
                weightedIntegralOfYawError = integralOfYaw * Util.K_ETA_SAI;
              
                uped_TailRotorPedal_BladeAngleOnTail = (-1 * weightedIntegralOfYawError - weightedProportionalOfYawError - weightedYawDerivativeError) * -1;
            }


            /**
             * END HORRIBLE WAY OF INTIGRAL ANTIWINDUP
             */



















            
//            yawErrors.Enqueue(weightedProportionalOfYawError);
           // yawErrors.Enqueue(errorPositionBodyFrame.ZAltitudePosition);
          /*  yawErrors.Enqueue(yawError);
            derivativeYaw.Enqueue(weightedYawDerivativeError);
            integralYaw.Enqueue(weightedIntegralOfYawError);
            Ped.Enqueue(uped_TailRotorPedal_BladeAngleOnTail);

            if (yawErrors.Count > 200)
            {
                yawErrors.Dequeue();
                derivativeYaw.Dequeue();
                integralYaw.Dequeue();
                Ped.Dequeue();
            }
            List<double> myList = new List<double>();
            myList.AddRange(yawErrors);
            Form1.plotLineGraphChart1(myList);
            myList = new List<double>();
            myList.AddRange(derivativeYaw);
            Form1.plotLineGraph(myList, Form1.chart2);
            myList = new List<double>();
            myList.AddRange(integralYaw);
            Form1.plotLineGraph(myList, Form1.chart3);
            myList = new List<double>();
            myList.AddRange(Ped);
            Form1.plotLineGraph(myList, Form1.chart4);
            */


            /*
            yawErrors.Enqueue(yawError);
            derivativeYaw.Enqueue(derivativeOfYaw);
            integralYaw.Enqueue(integralOfYaw);
            Ped.Enqueue(uped_TailRotorPedal_BladeAngleOnTail);

            if (yawErrors.Count > 400)
            {
                yawErrors.Dequeue();
                derivativeYaw.Dequeue();
                integralYaw.Dequeue();
                Ped.Dequeue();
            }



            
            List<double> myList = new List<double>();
            myList.AddRange(yawErrors);
            Form1.plotLineGraphChart1(myList);
            myList = new List<double>();
            myList.AddRange(derivativeYaw);
            Form1.plotLineGraph(myList, Form1.chart2);
            myList = new List<double>();
            myList.AddRange(integralYaw);
            Form1.plotLineGraph(myList, Form1.chart3);
            myList = new List<double>();
            myList.AddRange(Ped);
            Form1.plotLineGraph(myList, Form1.chart4);
             */

            if (refreshcounter == 5)
            {
                Application.DoEvents();
                refreshcounter = 0;
            }
            else
            {
                refreshcounter++;
            }


       /*     file.WriteLine(newmeasurement.Orientation.Sai_yawDegrees + ", " + referenceMeasurementNED.Orientation.Sai_yawDegrees + ", " +
           yawError + ", " + weightedProportionalOfYawError);*/
            
            
       //     Console.Write(weightedIntegralOfZBodyAltitude + ", " + weightedProportionalErrorZ + ", " + weightedDerivativeVelocityZ + "\r");

           
        }

        int refreshcounter = 0;

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
            referenceMeasurementNED = referenceMeasurement.ConvertFromGeodedicToLocalNED(initialReferenceStartingPositionEFEC, initialReferenceStartingPositionGeodedic);
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
