using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Net.Sockets;
using System.Net;

namespace Helicopter1
{

    
    public class Receiver2
    {
        public float olduped = 0.0f;
        static UdpClient listener = new UdpClient(Util.LISTENERPORT);
        static IPEndPoint groupEP = new IPEndPoint(IPAddress.Any, Util.LISTENERPORT);

        static Socket s = new Socket(AddressFamily.InterNetwork, SocketType.Dgram, ProtocolType.Udp);
        //static IPAddress broadcast = IPAddress.Parse("192.168.2.255");
        static IPAddress broadcast = IPAddress.Parse("127.0.0.255");

        static IPEndPoint ep = new IPEndPoint(broadcast, 49000);

        bool firstValue = false;

        internal void Start()
        {
            using (System.IO.StreamWriter file = new System.IO.StreamWriter(@"C:\Heli\logs\log1.csv"))
            {
            Measurement measurement = new Measurement();

            Position positionInertialFrame = new Position();
            positionInertialFrame.XLatitudePosition = 0;
            positionInertialFrame.YLongitudePosition = 0;
            positionInertialFrame.ZAltitudePosition = 0;
             
            
            measurement.Position = positionInertialFrame;

            Orientation orientation = new Orientation();
            orientation.Phi_rollDegrees = 0.0;
            orientation.Sai_yawDegrees = 0.0;
            orientation.Theta_pitchDegrees = 0.0;
            measurement.Orientation = orientation;


            positionInertialFrame = new Position();
            Measurement referenceMeasurement = new Measurement();
            /*positionInertialFrame.XLatitudePosition = 47.500312805175781;
            positionInertialFrame.YLongitudePosition = -122.21683502197266;*/

            positionInertialFrame.XLatitudePosition = 47.500312805175781;
            positionInertialFrame.YLongitudePosition = -122.21683502197266;


            positionInertialFrame.ZAltitudePosition = 110.0; 
            referenceMeasurement.Position = positionInertialFrame;

            Orientation referenceOrientation = new Orientation();
            referenceOrientation.Phi_rollDegrees = 0.0;
            referenceOrientation.Sai_yawDegrees = 0.0;
            referenceOrientation.Theta_pitchDegrees = 0.0;
            referenceMeasurement.Orientation = referenceOrientation;

            Velocity referenceVelocity = new Velocity();
            referenceVelocity.XVelocityBodyFrame = 0.0;
            referenceVelocity.YVelocityBodyFrame = 0.0;
            referenceVelocity.ZVelocityBodyFrame = 0.0;
            referenceVelocity.YawVelocity = 0.0;
            referenceMeasurement.Velocity = referenceVelocity;

            referenceMeasurement.ReferenceYaw = 0.0;


            ControlLaw controllerLaw = new ControlLaw();

            

            bool done = false;

            Position intialStartingReferencePosition = null;

            try
            {
                while (!done)
                {
                    byte[] bytes = listener.Receive(ref groupEP);


                    measurement = new Measurement();

                    positionInertialFrame = new Position();
                    Velocity newVelocity = new Velocity();
                    newVelocity.YawVelocity = System.BitConverter.ToSingle(bytes.Skip(53).Take(4).ToArray(), 0) * 57.2957795;
                    positionInertialFrame.XLatitudePosition = System.BitConverter.ToSingle(bytes.Skip(117).Take(4).ToArray(), 0);
                    positionInertialFrame.YLongitudePosition = System.BitConverter.ToSingle(bytes.Skip(121).Take(4).ToArray(), 0); 
                    positionInertialFrame.ZAltitudePosition = System.BitConverter.ToSingle(bytes.Skip(129).Take(4).ToArray(), 0);
                    measurement.Position = positionInertialFrame;
                    measurement.Velocity = newVelocity;



                    if (!firstValue)
                    {
                        intialStartingReferencePosition = positionInertialFrame;
                        firstValue = true;
                        controllerLaw.SetInitialReferenceStartingPosition(intialStartingReferencePosition);
                        controllerLaw.setReferenceMeasurement(referenceMeasurement);
                    }


                    orientation = new Orientation();
                    orientation.Phi_rollDegrees = System.BitConverter.ToSingle(bytes.Skip(85).Take(4).ToArray(), 0);
                    orientation.Sai_yawDegrees = System.BitConverter.ToSingle(bytes.Skip(89).Take(4).ToArray(), 0);
                    orientation.Theta_pitchDegrees = System.BitConverter.ToSingle(bytes.Skip(81).Take(4).ToArray(), 0);
                    measurement.Orientation = orientation;
                    controllerLaw.processMeasurement(measurement, file);


                    double ulon = controllerLaw.Getulon_MainRotorLongitudinalCyclicControlCommand_Pitch();
                    double ulat = controllerLaw.Getulat_MainRotorLateralCyclicControlCommand_Roll();
                    double ucol = controllerLaw.Getucol_MainRotorCollectiveControlCommand();
                    double uped = controllerLaw.Getuped_TailRotorPedalControlCommand();

                   // Console.Write(ulon + ", " + ulat + ", " + ucol +", " + uped+ "\r");


                    ulon = ulon / 100 ;                    
                    byte[] negativeonebytes = System.BitConverter.GetBytes((float)ulon);
                    Array.Copy(negativeonebytes, 0, bytes, 9, negativeonebytes.Length);


                    ulat = ulat / 100 ;
                    negativeonebytes = System.BitConverter.GetBytes((float)ulat);
                    Array.Copy(negativeonebytes, 0, bytes, 13, negativeonebytes.Length);
                
             uped = -1 * uped  / 100.0;


             if (uped > 9.6)
             {
                 negativeonebytes = System.BitConverter.GetBytes((float)9.6);
                 olduped = 9.6f;
             }
             else if (uped < -9.6)
             {
                 negativeonebytes = System.BitConverter.GetBytes((float)-9.6);
                 olduped = -9.6f;
             }
             else
             {
                 negativeonebytes = System.BitConverter.GetBytes((float)uped);
             }
             Array.Copy(negativeonebytes, 0, bytes, 193, negativeonebytes.Length); 
              
    //                ucol = ucol /10.0;
             ucol = -1 * ucol / 10.0;
                    if (ucol > 9.6)
                    {
                        negativeonebytes = System.BitConverter.GetBytes((float)9.6);
                        Array.Copy(negativeonebytes, 0, bytes, 189, negativeonebytes.Length);
                    }
                    else if (ucol < -9.6)
                    {
                        negativeonebytes = System.BitConverter.GetBytes((float)-9.6);
                        Array.Copy(negativeonebytes, 0, bytes, 189, negativeonebytes.Length);
                    }
                    else
                    {
                        negativeonebytes = System.BitConverter.GetBytes((float)ucol);
                        Array.Copy(negativeonebytes, 0, bytes, 189, negativeonebytes.Length);
                    }

                    IPEndPoint ep = new IPEndPoint(broadcast, 49000);
                   s.SendTo(bytes, ep);

                }

            }
            catch (Exception e)
            {
                Console.WriteLine(e.ToString());
            }
            finally
            {
                listener.Close();
            }

             }

        }
    }
}
