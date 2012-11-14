using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Helicopter1
{
    class Program
    {
        //static void Main(string[] args)
        //{


        //    Receiver receiver = new Receiver();
        //    receiver.Start();





        //    Measurement measurement = new Measurement();

        //    Position positionInertialFrame = new Position();
        //    positionInertialFrame.XLatitudePosition = 32.0;
        //    positionInertialFrame.YLongitudePosition = 44.0;
        //    positionInertialFrame.ZAltitudePosition = 22.0;
        //    measurement.Position = positionInertialFrame;

        //    Orientation orientation = new Orientation();
        //    orientation.Phi_rollDegrees = 10.0;
        //    orientation.Sai_yawDegrees = 10.0;
        //    orientation.Theta_pitchDegrees = 10.0;
        //    measurement.Orientation = orientation;



        //    Measurement referenceMeasurement = new Measurement();
        //    positionInertialFrame = new Position();
        //    positionInertialFrame.XLatitudePosition  = 42.0;
        //    positionInertialFrame.YLongitudePosition = 54.0;
        //    positionInertialFrame.ZAltitudePosition = 32.0;
        //    referenceMeasurement.Position = positionInertialFrame;

        //    Orientation referenceOrientation = new Orientation();
        //    referenceOrientation.Phi_rollDegrees = 20.0;
        //    referenceOrientation.Sai_yawDegrees = 25.0;
        //    referenceOrientation.Theta_pitchDegrees = 20.0;
        //    referenceMeasurement.Orientation = referenceOrientation;

        //    Velocity referenceVelocity = new Velocity();
        //    referenceVelocity.XVelocityBodyFrame = 4.4;
        //    referenceVelocity.YVelocityBodyFrame = 5.5;
        //    referenceVelocity.ZVelocityBodyFrame = 1.2;

        //    referenceMeasurement.Velocity = referenceVelocity;

        //    referenceMeasurement.ReferenceYaw = 0.0;


        //    ControlLaw controllerLaw = new ControlLaw();

        //    controllerLaw.setReferenceMeasurement(referenceMeasurement);
        //    controllerLaw.initializationMeasurment(measurement);





        //    measurement = new Measurement();

        //    positionInertialFrame = new Position();
        //    positionInertialFrame.XLatitudePosition = 34.0;
        //    positionInertialFrame.YLongitudePosition = 46.0;
        //    positionInertialFrame.ZAltitudePosition = 24.0;
        //    measurement.Position = positionInertialFrame;

        //    orientation = new Orientation();
        //    orientation.Phi_rollDegrees = 13.0;
        //    orientation.Sai_yawDegrees = 15.0;
        //    orientation.Theta_pitchDegrees = 13.0;
        //    measurement.Orientation = orientation;

            

        //    double ulonold = 0.0;
        //    double ulatold = 0.0;
        //    double ucolold = 0.0;
        //    double upedold = 0.0;

        //    for (int i = 0; i < 4; i++)
        //    {
        //        controllerLaw.processMeasurement(measurement);
        //        double ulon = controllerLaw.Getulon_MainRotorLongitudinalCyclicControlCommand_Pitch();
        //        double ulat = controllerLaw.Getulat_MainRotorLateralCyclicControlCommand_Roll();
        //        double ucol = controllerLaw.Getucol_MainRotorCollectiveControlCommand();
        //        double uped = controllerLaw.Getuped_TailRotorPedalControlCommand();

        //        ulonold = ulon;
        //        ulatold = ulat;
        //        ucolold = ucol;
        //        upedold = uped;
        //    }

        //    Measurement measurementcloser = new Measurement();
        //    Position closerPosition = new Position();
        //    Orientation closerOrientation = new Orientation();
        //    closerPosition.XLatitudePosition = 34.0;
        //    closerPosition.YLongitudePosition = 46.0;
        //    closerPosition.ZAltitudePosition = 24.0;
        //    measurementcloser.Position = positionInertialFrame;

        //    /******************************* im setting the orientation the same so i just change 1 thing at a time*/
        //                closerOrientation = new Orientation();
        //                closerOrientation.Phi_rollDegrees = 13.0;
        //                closerOrientation.Sai_yawDegrees = 15.0;
        //                closerOrientation.Theta_pitchDegrees = 13.0;
             
        //    //closerOrientation = referenceOrientation.Copy();
        //    measurementcloser.Orientation = orientation;
            

            


        //    //List<Tuple<double, double, double, double>> diffs = new List<Tuple<double, double, double, double>>();
        //    List<double[]> diffs = new List<double[]>();

        //    bool NotDone = true;
        //    while (NotDone)
        //    {
        //        /*closerPosition = new Position();
        //        measurementcloser = new Measurement();
        //        closerOrientation = new Orientation();*/

        //        NotDone = false;
        //        if (closerOrientation.Phi_rollDegrees < referenceOrientation.Phi_rollDegrees)
        //        {
        //            closerOrientation.Phi_rollDegrees += .2;
        //            NotDone = true;
        //        }
        //        if (closerOrientation.Sai_yawDegrees < referenceOrientation.Sai_yawDegrees)
        //        {
        //            closerOrientation.Sai_yawDegrees += .1;
        //            NotDone = true;
        //        }
        //        if (closerOrientation.Theta_pitchDegrees < referenceOrientation.Theta_pitchDegrees)
        //        {
        //            closerOrientation.Theta_pitchDegrees += .3;
        //            NotDone = true;
        //        }

        //        if (closerPosition.XLatitudePosition < referenceMeasurement.Position.XLatitudePosition)
        //        {
        //            closerPosition.XLatitudePosition += .1;
        //            NotDone = true;
        //        }
        //        if (closerPosition.YLongitudePosition < referenceMeasurement.Position.YLongitudePosition)
        //        {
        //            closerPosition.YLongitudePosition += .1;
        //            NotDone = true;
        //        }
        //        if (closerPosition.ZAltitudePosition < referenceMeasurement.Position.ZAltitudePosition)
        //        {
        //            closerPosition.ZAltitudePosition += .1;
        //            NotDone = true;
        //        }

        //        measurementcloser.Orientation = closerOrientation;
        //        measurementcloser.Position = closerPosition;
        //        controllerLaw.processMeasurement(measurementcloser);


        //        double ulon = controllerLaw.Getulon_MainRotorLongitudinalCyclicControlCommand_Pitch();
        //        double ulat = controllerLaw.Getulat_MainRotorLateralCyclicControlCommand_Roll();
        //        double ucol = controllerLaw.Getucol_MainRotorCollectiveControlCommand();
        //        double uped = controllerLaw.Getuped_TailRotorPedalControlCommand();

        //        double diff1 = ulon-ulonold;
        //        double diff2 = ulat-ulatold;
        //        double diff3 = ucol-ucolold;
        //        double diff4 = uped-upedold;
        //        diffs.Add(new double[]{diff1, diff2, diff3, diff4});

        //        ulonold = ulon;
        //        ulatold = ulat;
        //        ucolold = ucol;
        //        upedold = uped;

        //    }

        //    Console.Out.WriteLine("Done");
            
        //    //if I process the same measurements, i expect the new values to be greater because it isn't moving.




        //    //if i process measurements that are closer to the reference, i expect values to be less since it's closer.





        //}


        static void Main(string[] args)
        {


            Receiver2 receiver = new Receiver2();
            receiver.Start();





            Measurement measurement = new Measurement();

            Position positionInertialFrame = new Position();
            positionInertialFrame.XLatitudePosition = 32.0;
            positionInertialFrame.YLongitudePosition = 44.0;
            positionInertialFrame.ZAltitudePosition = 22.0;
            measurement.Position = positionInertialFrame;

            Orientation orientation = new Orientation();
            orientation.Phi_rollDegrees = 10.0;
            orientation.Sai_yawDegrees = 10.0;
            orientation.Theta_pitchDegrees = 10.0;
            measurement.Orientation = orientation;



            Measurement referenceMeasurement = new Measurement();
            positionInertialFrame = new Position();
            positionInertialFrame.XLatitudePosition = 42.0;
            positionInertialFrame.YLongitudePosition = 54.0;
            positionInertialFrame.ZAltitudePosition = 32.0;
            referenceMeasurement.Position = positionInertialFrame;

            Orientation referenceOrientation = new Orientation();
            referenceOrientation.Phi_rollDegrees = 20.0;
            referenceOrientation.Sai_yawDegrees = 25.0;
            referenceOrientation.Theta_pitchDegrees = 20.0;
            referenceMeasurement.Orientation = referenceOrientation;

            Velocity referenceVelocity = new Velocity();
            referenceVelocity.XVelocityBodyFrame = 4.4;
            referenceVelocity.YVelocityBodyFrame = 5.5;
            referenceVelocity.ZVelocityBodyFrame = 1.2;

            referenceMeasurement.Velocity = referenceVelocity;

            referenceMeasurement.ReferenceYaw = 0.0;


            ControlLaw controllerLaw = new ControlLaw();

            controllerLaw.setReferenceMeasurement(referenceMeasurement);
            





            measurement = new Measurement();

            positionInertialFrame = new Position();
            positionInertialFrame.XLatitudePosition = 34.0;
            positionInertialFrame.YLongitudePosition = 46.0;
            positionInertialFrame.ZAltitudePosition = 24.0;
            measurement.Position = positionInertialFrame;

            orientation = new Orientation();
            orientation.Phi_rollDegrees = 13.0;
            orientation.Sai_yawDegrees = 15.0;
            orientation.Theta_pitchDegrees = 13.0;
            measurement.Orientation = orientation;



            double ulonold = 0.0;
            double ulatold = 0.0;
            double ucolold = 0.0;
            double upedold = 0.0;

            for (int i = 0; i < 4; i++)
            {
                //controllerLaw.processMeasurement(measurement, file);
                double ulon = controllerLaw.Getulon_MainRotorLongitudinalCyclicControlCommand_Pitch();
                double ulat = controllerLaw.Getulat_MainRotorLateralCyclicControlCommand_Roll();
                double ucol = controllerLaw.Getucol_MainRotorCollectiveControlCommand();
                double uped = controllerLaw.Getuped_TailRotorPedalControlCommand();

                ulonold = ulon;
                ulatold = ulat;
                ucolold = ucol;
                upedold = uped;
            }

            Measurement measurementcloser = new Measurement();
            Position closerPosition = new Position();
            Orientation closerOrientation = new Orientation();
            closerPosition.XLatitudePosition = 34.0;
            closerPosition.YLongitudePosition = 46.0;
            closerPosition.ZAltitudePosition = 24.0;
            measurementcloser.Position = positionInertialFrame;

            /******************************* im setting the orientation the same so i just change 1 thing at a time*/
            closerOrientation = new Orientation();
            closerOrientation.Phi_rollDegrees = 13.0;
            closerOrientation.Sai_yawDegrees = 15.0;
            closerOrientation.Theta_pitchDegrees = 13.0;

            //closerOrientation = referenceOrientation.Copy();
            measurementcloser.Orientation = orientation;





            //List<Tuple<double, double, double, double>> diffs = new List<Tuple<double, double, double, double>>();
            List<double[]> diffs = new List<double[]>();

            bool NotDone = true;
            while (NotDone)
            {
                /*closerPosition = new Position();
                measurementcloser = new Measurement();
                closerOrientation = new Orientation();*/

                NotDone = false;
                if (closerOrientation.Phi_rollDegrees < referenceOrientation.Phi_rollDegrees)
                {
                    closerOrientation.Phi_rollDegrees += .2;
                    NotDone = true;
                }
                if (closerOrientation.Sai_yawDegrees < referenceOrientation.Sai_yawDegrees)
                {
                    closerOrientation.Sai_yawDegrees += .1;
                    NotDone = true;
                }
                if (closerOrientation.Theta_pitchDegrees < referenceOrientation.Theta_pitchDegrees)
                {
                    closerOrientation.Theta_pitchDegrees += .3;
                    NotDone = true;
                }

                if (closerPosition.XLatitudePosition < referenceMeasurement.Position.XLatitudePosition)
                {
                    closerPosition.XLatitudePosition += .1;
                    NotDone = true;
                }
                if (closerPosition.YLongitudePosition < referenceMeasurement.Position.YLongitudePosition)
                {
                    closerPosition.YLongitudePosition += .1;
                    NotDone = true;
                }
                if (closerPosition.ZAltitudePosition < referenceMeasurement.Position.ZAltitudePosition)
                {
                    closerPosition.ZAltitudePosition += .1;
                    NotDone = true;
                }

                measurementcloser.Orientation = closerOrientation;
                measurementcloser.Position = closerPosition;
            //    controllerLaw.processMeasurement(measurementcloser);


                double ulon = controllerLaw.Getulon_MainRotorLongitudinalCyclicControlCommand_Pitch();
                double ulat = controllerLaw.Getulat_MainRotorLateralCyclicControlCommand_Roll();
                double ucol = controllerLaw.Getucol_MainRotorCollectiveControlCommand();
                double uped = controllerLaw.Getuped_TailRotorPedalControlCommand();

                double diff1 = ulon - ulonold;
                double diff2 = ulat - ulatold;
                double diff3 = ucol - ucolold;
                double diff4 = uped - upedold;
                diffs.Add(new double[] { diff1, diff2, diff3, diff4 });

                ulonold = ulon;
                ulatold = ulat;
                ucolold = ucol;
                upedold = uped;

            }

            Console.Out.WriteLine("Done");

            //if I process the same measurements, i expect the new values to be greater because it isn't moving.




            //if i process measurements that are closer to the reference, i expect values to be less since it's closer.





        }
    }
}
