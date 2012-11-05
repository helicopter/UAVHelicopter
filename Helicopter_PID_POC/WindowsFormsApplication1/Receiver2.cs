using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Net.Sockets;
using System.Net;

using WindowsFormsApplication1;
using System.Windows.Forms;


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

        public bool done = false;

        internal void Start()
        {

          //  Form1.trackBar1.Value =  (int) (Util.K_SAI * 10.0);
            Form1.trackBar1.Value = (int)(Util.K_Z * 1000.0);
            Form1.trackBar2.Value = (int)(Util.K_W * 1000.0);
            Form1.trackBar3.Value = (int)(Util.Ti_Z * 1000.0);
            Form1.trackBar4.Value = (int)(Util.K_ETA_Z * 1000.0);

            using (System.IO.StreamWriter file = new System.IO.StreamWriter(@"C:\Heli\logs\log1.csv"))
            {
                using (System.IO.StreamWriter file2 = new System.IO.StreamWriter(@"C:\Heli\logs\log1pid.csv"))
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
                    Orientation referenceOrientation = new Orientation();

                    /*
                     * Stand still
                     * 3 feet
                     * 0 deg
                     */
                    //positionInertialFrame.XLatitudePosition = 47.500312805175781;
                    //positionInertialFrame.YLongitudePosition = -122.21683502197266;
                    //referenceOrientation.Sai_yawDegrees = 0.0;
                    //positionInertialFrame.ZAltitudePosition = 3.0;


                    /*
                     * Stand still
                     * 10 feet
                     * 0 deg
                     */
                    //positionInertialFrame.XLatitudePosition = 47.500312805175781;
                    //positionInertialFrame.YLongitudePosition = -122.21683502197266;
                    //referenceOrientation.Sai_yawDegrees = 0.0;
                    //positionInertialFrame.ZAltitudePosition = 10.0;

                    /*
                     * Stand still
                     * 110 feet
                     * 0 deg
                     */
                    positionInertialFrame.XLatitudePosition = 47.500312805175781;
                    positionInertialFrame.YLongitudePosition = -122.21683502197266;
                    referenceOrientation.Sai_yawDegrees = 0.0;
                    positionInertialFrame.ZAltitudePosition = 110.0;

                    /*
                     * Stand still
                     * 3 feet
                     * 160 deg
                     */
                    //positionInertialFrame.XLatitudePosition = 47.500312805175781;
                    //positionInertialFrame.YLongitudePosition = -122.21683502197266;
                    //referenceOrientation.Sai_yawDegrees = 160.0;
                    //positionInertialFrame.ZAltitudePosition = 3.0;

                    /*
                     * Stand still
                     * 10 feet
                     * 160 deg
                     */
                    //positionInertialFrame.XLatitudePosition = 47.500312805175781;
                    //positionInertialFrame.YLongitudePosition = -122.21683502197266;
                    //referenceOrientation.Sai_yawDegrees = 160.0;
                    //positionInertialFrame.ZAltitudePosition = 10.0;

                    /*
                     * Stand still
                     * 110 feet
                     * 160 deg
                     */
                    //positionInertialFrame.XLatitudePosition = 47.500312805175781;
                    //positionInertialFrame.YLongitudePosition = -122.21683502197266;
                    //referenceOrientation.Sai_yawDegrees = 160.0;
                    //positionInertialFrame.ZAltitudePosition = 110.0;


                    /*
                     * Goto
                     * 3 feet
                     * 160 deg
                     */
                    //positionInertialFrame.XLatitudePosition = 47.48675900142983;
                    //positionInertialFrame.YLongitudePosition = -122.21457481384277;
                    //referenceOrientation.Sai_yawDegrees = 160.0;
                    //positionInertialFrame.ZAltitudePosition = 3.0;


                    /*
                     * Goto
                     * 10 feet
                     * 160 deg
                     */
                    //positionInertialFrame.XLatitudePosition = 47.48675900142983;
                    //positionInertialFrame.YLongitudePosition = -122.21457481384277;
                    //referenceOrientation.Sai_yawDegrees = 160.0;
                    //positionInertialFrame.ZAltitudePosition = 10.0;

                    /*
                     * Goto
                     * 110 feet
                     * 160 deg
                    // */
                    //positionInertialFrame.XLatitudePosition = 47.48675900142983;
                    //positionInertialFrame.YLongitudePosition = -122.21457481384277;
                    //referenceOrientation.Sai_yawDegrees = 160.0;
                    //positionInertialFrame.ZAltitudePosition = 110.0;



                    //stand still
                    /*positionInertialFrame.XLatitudePosition = 47.500312805175781;
                    positionInertialFrame.YLongitudePosition = -122.21683502197266;
                    referenceOrientation.Sai_yawDegrees = 0.0;
                    positionInertialFrame.ZAltitudePosition = 3.0;*/


                    /*positionInertialFrame.XLatitudePosition = 47.48675900142983;
                      positionInertialFrame.YLongitudePosition = -122.21457481384277;*/


                    //  positionInertialFrame.ZAltitudePosition = 50.0;
                  //  positionInertialFrame.ZAltitudePosition = 110.0;
                     // positionInertialFrame.ZAltitudePosition = 3.0;
                    
                    referenceMeasurement.Position = positionInertialFrame;

                    
                    referenceOrientation.Phi_rollDegrees = 0.0;
                  // referenceOrientation.Sai_yawDegrees = 160.0;
                    //referenceOrientation.Sai_yawDegrees = 90.0;
                   // referenceOrientation.Sai_yawDegrees = 0.0;
                   // referenceOrientation.Sai_yawDegrees = 180.0;
                    //referenceOrientation.Sai_yawDegrees = 170.0;


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





                    Position intialStartingReferencePosition = null;
                    DateTime starTime = DateTime.Now;
                    try
                    {
                        while (!done)
                        {
                            byte[] bytes = listener.Receive(ref groupEP);
                            starTime = DateTime.Now;

                            measurement = new Measurement();

                            positionInertialFrame = new Position();
                            Velocity newVelocity = new Velocity();
                            newVelocity.YawVelocity = System.BitConverter.ToSingle(bytes.Skip(53).Take(4).ToArray(), 0) * 57.2957795;
                            newVelocity.PitchVelocityDeg = System.BitConverter.ToSingle(bytes.Skip(45).Take(4).ToArray(), 0) * 57.2957795;
                            newVelocity.RollVelocityDeg = System.BitConverter.ToSingle(bytes.Skip(49).Take(4).ToArray(), 0) * 57.2957795;

                            newVelocity.XVelocityNEDFrame = System.BitConverter.ToSingle(bytes.Skip(173).Take(4).ToArray(), 0) * -1; // * -1 to convert to north
                            newVelocity.YVelocityNEDFrame = System.BitConverter.ToSingle(bytes.Skip(165).Take(4).ToArray(), 0);
                            newVelocity.ZVelocityNEDFrame = System.BitConverter.ToSingle(bytes.Skip(169).Take(4).ToArray(), 0) * -1; // * -1 to convert to down

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
                            controllerLaw.processMeasurement(measurement, file, file2);


                            double ulon = controllerLaw.Getulon_MainRotorLongitudinalCyclicControlCommand_Pitch();
                            double ulat = controllerLaw.Getulat_MainRotorLateralCyclicControlCommand_Roll();
                            double ucol = controllerLaw.Getucol_MainRotorCollectiveControlCommand();
                            double uped = controllerLaw.Getuped_TailRotorPedalControlCommand();

                            // Console.Write(ulon + ", " + ulat + ", " + ucol +", " + uped+ "\r");


                          //  ulon = ulon / 100;
                            if (ulon > .8) ulon = .8;
                            if (ulon < -.8) ulon = -.8;
                            byte[] negativeonebytes = System.BitConverter.GetBytes((float)ulon);
                            Array.Copy(negativeonebytes, 0, bytes, 9, negativeonebytes.Length);


                      //      ulat = ulat / 100;
                            if (ulat > .8) ulat = .8;
                            if (ulat < -.8) ulat = -.8;
                            negativeonebytes = System.BitConverter.GetBytes((float)ulat);
                            Array.Copy(negativeonebytes, 0, bytes, 13, negativeonebytes.Length);

  //                          uped = -1 * uped / 100.0;


//                            Util.K_SAI = Form1.trackBar1.Value / 100.0;
                         //   Form1.textBox1.Text = Util.K_SAI.ToString();
                            Util.K_Z = Form1.trackBar1.Value / 1000.0;
                            Form1.textBox1.Text = Util.K_Z.ToString();

                            Util.K_W = Form1.trackBar2.Value / 1000.0;
                            Form1.textBox2.Text = Util.K_W.ToString();


                            Util.K_ETA_Z = Form1.trackBar4.Value / 1000.0;
                            Form1.textBox4.Text = Util.K_ETA_Z.ToString();

                            /*ControlLaw.referenceMeasurementNED.Orientation.Sai_yawDegrees = Form1.trackBar3.Value / 1000.0;
                            Form1.textBox3.Text = ControlLaw.referenceMeasurementNED.Orientation.Sai_yawDegrees.ToString();*/

                            Util.Ti_Z = Form1.trackBar3.Value / 1000.0;
                            Form1.textBox3.Text = Util.Ti_Z.ToString();
                            //uped = Form1.trackBar1.Value / 10.0;
                            //Form1.textBox1.Text = uped.ToString() ;
                   //         if (uped < 0) uped = uped * -1;
                   //         if (uped < 0) uped = 0;


                            if (uped > Util.UPEDMAXCONTROLVALUE)
                            {
                                negativeonebytes = System.BitConverter.GetBytes((float)Util.UPEDMAXCONTROLVALUE);
                                olduped = (float) Util.UPEDMAXCONTROLVALUE;
                            }
                            else if (uped < Util.UPEDMINCONTROLVALUE)
                            {
                                negativeonebytes = System.BitConverter.GetBytes((float)Util.UPEDMINCONTROLVALUE);
                                olduped = (float) Util.UPEDMINCONTROLVALUE;
                            }
                            else
                            {
                                negativeonebytes = System.BitConverter.GetBytes((float)uped);
                            }
                            Array.Copy(negativeonebytes, 0, bytes, 193, negativeonebytes.Length);

                            
                           // ucol = -1 * ucol / 10.0;

                       //     if (ucol < 0) ucol = 0;


                            if (ucol > Util.UCOLMAXCONTROLVALUE)
                            {
                                negativeonebytes = System.BitConverter.GetBytes((float)Util.UCOLMAXCONTROLVALUE);
                                Array.Copy(negativeonebytes, 0, bytes, 189, negativeonebytes.Length);
                            }
                            else if (ucol < Util.UCOLMINCONTROLVALUE)
                            {
                                negativeonebytes = System.BitConverter.GetBytes((float)Util.UCOLMINCONTROLVALUE); // it was -5
                                Array.Copy(negativeonebytes, 0, bytes, 189, negativeonebytes.Length);
                            }
                            else
                            {
                                negativeonebytes = System.BitConverter.GetBytes((float)ucol);
                                Array.Copy(negativeonebytes, 0, bytes, 189, negativeonebytes.Length);
                            }

                            IPEndPoint ep = new IPEndPoint(broadcast, 49000);
                            s.SendTo(bytes, ep);

                         //   Form1.textBox1.Text = starTime.Subtract(DateTime.Now).Milliseconds.ToString();
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
}
