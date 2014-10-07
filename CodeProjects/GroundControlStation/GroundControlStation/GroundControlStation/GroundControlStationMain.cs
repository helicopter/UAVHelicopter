using System;
using System.Collections.Generic;
using System.IO.Ports;
using System.Linq;
using System.Net;
using System.Threading.Tasks;
using System.Windows.Forms;
using GroundControlStation.Controller;
using GroundControlStation.Interfaces;
using GroundControlStation.Model;
using GroundControlStation.Views;
using System.IO;
using GroundControlStation.Messages;
using System.Diagnostics;
using GroundControlStation.Util;
using UTIL;


namespace GroundControlStation
{
    static class GroundControlStationMain
    {
        enum FlightModes { SimulatedFlight, RealFlight };




        static float[] CrossProduct(float[] vector1, float[] vector2, float[] crossProduct)
        {
            crossProduct[0] = vector1[1] * vector2[2] - vector1[2] * vector2[1];
            crossProduct[1] = -1 * (vector1[0] * vector2[2] - vector1[2] * vector2[0]);
            crossProduct[2] = vector1[0] * vector2[1] - vector1[1] * vector2[0];

            return crossProduct;
        }

        [STAThread]
        static void Main54()
        {

            float[] vecacc = new float[] { 0, 0, -1 };
            float[] vecmag = new float[] { 1, 0, 0 };
            float[] vec3 = new float[3];
            float[] vec4 = new float[3]; 

            CrossProduct(vecacc, vecmag, vec3); // should get {0,-1,0}
            CrossProduct(vec3, vecacc, vec4); //want {1,0,0} but might get {-1,0,0}

            
            GroundControlStationModel mm = new GroundControlStationModel();
            SimulatorTelemetry tt = new SimulatorTelemetry();
            tt.MagHeadingDegrees = 90;
            tt.PitchDegrees = 0;
            tt.RollDegrees = 0;
            tt.XVelocityNEDMs = 10;
            tt.YVelocityNEDMs = 0;
            tt.ZVelocityNEDMs = 0;

            mm.PreviousXVelocityNEDCms = 500;
            mm.PreviousYVelocityNEDCms = 0;
            mm.PreviousZVelocityNEDCms = 0;


            mm.SimTelm = tt;





            FlightComputerTelemetryMessage dd = FlightComputerTelemetryMessage.CreateFromModel(mm);

            










            //shrink files
            //verify data being received and sent
            //test timing of ahrs, and enhance speed of ahrs. 

            List<SimulatorTelemetry> simTelemList = parseFile();





            SimulatorTelemetry val = simTelemList[0];

            val.PitchVelocityRadsPerS = 0;
            val.YawVelocityRadsPerS = 0;
            val.RollVelocityRadsPerS = 0;
            val.XVelocityNEDMs = 0;
            val.YVelocityNEDMs = 0;
            val.ZVelocityNEDMs = 0;



            GroundControlStationModel model = new GroundControlStationModel();
            model.SimTelm = val;





            int minCounter = 9999;
            float minMag = 0;
            float minAccel = 0;



            MadgwickAHRS madgwick = new MadgwickAHRS(1 / 20.0f, 0.041f);
            //MadgwickAHRS madgwick = new MadgwickAHRS(1 / 20.0f);



            for (int i = 1; i < 100; i++)
            {
                for (int j = 1; j < 1000; j++)
                {
                    int counter = 0;
                    AHRS.reset();

                    float magVal = i / 100f;
                    float accelVal = j / 1000f;

                    while (true)
                    {


                      




                        counter++;

                        FlightComputerTelemetryMessage data = FlightComputerTelemetryMessage.CreateFromModel(model);

                        SensorDataMessage sensorData = new SensorDataMessage();
                        sensorData.PitchAngularVelocityRadsPerSecond = data.PitchAngularVelocityRadsPerSecond;
                        sensorData.PressureMillibars = data.PressureMillibars;
                        sensorData.RollAngularVelocityRadsPerSecond = data.RollAngularVelocityRadsPerSecond;
                        sensorData.XAccelFrdMss = data.XAccelFrdMss;
                        sensorData.XEcefCm = data.XEcefCm;
                        sensorData.XMagFrd = data.XMagFrd;
                        sensorData.XVEcefCms = data.XVEcefCms;
                        sensorData.YAccelFrdMss = data.YAccelFrdMss;
                        sensorData.YawAngularVelocityRadsPerSecond = data.YawAngularVelocityRadsPerSecond;
                        sensorData.YEcefCm = data.YEcefCm;
                        sensorData.YMagFrd = data.YMagFrd;
                        sensorData.YVEcefCms = data.YVEcefCms;
                        sensorData.ZAccelFrdMss = data.ZAccelFrdMss;
                        sensorData.ZEcefCm = data.ZEcefCm;
                        sensorData.ZMagFrd = data.ZMagFrd;
                        sensorData.ZVEcefCms = data.ZVEcefCms;

                        AHRS.ahrs(sensorData.XAccelFrdMss, sensorData.YAccelFrdMss, sensorData.ZAccelFrdMss,
                        sensorData.RollAngularVelocityRadsPerSecond, sensorData.PitchAngularVelocityRadsPerSecond, sensorData.YawAngularVelocityRadsPerSecond,
                        sensorData.XMagFrd, sensorData.YMagFrd, sensorData.ZMagFrd, accelVal, magVal);

                        /*AHRS.ahrs(0.01f,0.01f,-1.0f,
                        0,0,0,
                        .5f,-.5f,0.001f, accelVal, magVal);
                         */
                        /*AHRS.ahrs(0.5f, 0.01f, -.50f,
                        0, 0, 0,
                        1f, -.01f, 0.001f, accelVal, magVal);*/

                        //roll example
                        /*AHRS.ahrs(0.01f, -0.5f, -.50f,
                        0, 0, 0,
                        1f, -.01f, 0.001f, accelVal, magVal);*/




                        /*madgwick.Update(0, 0, 0,
                          sensorData.XAccelFrdMss, sensorData.YAccelFrdMss, sensorData.ZAccelFrdMss,
                          sensorData.XMagFrd, sensorData.YMagFrd, sensorData.ZMagFrd);
                         */
                        madgwick.Update(0, 0, 0,
                          sensorData.XAccelFrdMss, sensorData.YAccelFrdMss*-1, sensorData.ZAccelFrdMss,
                          sensorData.XMagFrd, sensorData.YMagFrd*-1, sensorData.ZMagFrd);



                        //This assumes y axis mag sensor data is positive. for counterclockwise rotations, unlike my code. so have to multiply mag y * -1
                        //orientation for the x-imu was xfront, yleft, z down?
                        /*madgwick.Update(0, 0, 0,
                          0.01f, 0.01f, -1.0f,
                         .5f, .5f, 0.001f);*/

                        //accel is in same orientation. pitch example
                        /*madgwick.Update(0, 0, 0,
                          0.5f, -0.01f, -.50f,
                         1f, .01f, 0.001f);*/

                        //rollexample
                        //roll accel y axis assumes left is positive so have to * accel by *-1.
                        /*madgwick.Update(0, 0, 0,
                          0.01f, 0.5f, -.50f,
                         1f, .01f, 0.001f);*/
                        float y = 0;
                        float p = 0;
                        float r = 0;
                        madgwick.getYawPitchRoll(out y, out p, out r);





                        float yaw = AHRS.yawRads;
                        float pitch = AHRS.pitchRads;
                        float roll = AHRS.rollRads;

                        double rollError = (roll * (180 / Math.PI)) - model.SimTelm.RollDegrees;
                        rollError = Math.Abs(rollError);
                        model.RollMSE += Math.Pow(rollError, 2);



                        double pitchError = (pitch * (180 / Math.PI)) - model.SimTelm.PitchDegrees;
                        pitchError = Math.Abs(pitchError);
                        model.PitchMSE += Math.Pow(pitchError, 2);

                        yaw = (float)(yaw * (180 / Math.PI));

                        if (yaw > 180)
                        {
                            yaw = yaw - 360;
                        }
                        double simMag = model.SimTelm.MagHeadingDegrees;
                        if (simMag > 180)
                        {
                            simMag = simMag - 360;
                        }
                        //double yawError = (yaw * (180 / Math.PI)) - model.SimTelm.MagHeadingDegrees;
                        double yawError = yaw - simMag;
                        yawError = Math.Abs(yawError);
                        model.YawMSE += Math.Pow(yawError, 2);


                        if (yawError < 1 && pitchError < 1 && rollError < 1)
                        {
                            if (counter < minCounter)
                            {
                                minCounter = counter;
                                minMag = magVal;
                                minAccel = accelVal;

                                Console.WriteLine(minCounter + " " + minMag + " " + minAccel);

                            }
                            
                            break;
                        }

                    }
                }

            }
            



        }







        [STAThread]
        static void Main5()
        {



            /*
            GroundControlStationModel mm = new GroundControlStationModel();
            SimulatorTelemetry tt = new SimulatorTelemetry();
            tt.MagHeadingDegrees = 90;
            tt.PitchDegrees = 0;
            tt.RollDegrees = 0;
            tt.XVelocityNEDMs = 10;
            tt.YVelocityNEDMs = 0;
            tt.ZVelocityNEDMs = 0;

            mm.PreviousXVelocityNEDCms = 500;
            mm.PreviousYVelocityNEDCms = 0;
            mm.PreviousZVelocityNEDCms = 0;


            mm.SimTelm = tt;



            FlightComputerTelemetryMessage dd = FlightComputerTelemetryMessage.CreateFromModel(mm);

            */










            //shrink files
            //verify data being received and sent
            //test timing of ahrs, and enhance speed of ahrs. 

            List<SimulatorTelemetry> simTelemList = parseFile();

            SerialPort port = new SerialPort("COM7", 250000, Parity.None, 8, StopBits.One);
            port.ReadTimeout = 50000;

            SerialPortInterface portInterface = new SerialPortInterface(port);

            FlightComputerInterface fcInterface = new FlightComputerInterface(portInterface);
            //         fcInterface.Open();

            GroundControlStationModel model = new GroundControlStationModel();
            model.SimTelm = new SimulatorTelemetry();



            model.RollResults = new List<GroundControlStation.Model.GroundControlStationModel.Results>();
            model.YawResults = new List<GroundControlStation.Model.GroundControlStationModel.Results>();
            model.PitchResults = new List<GroundControlStation.Model.GroundControlStationModel.Results>();




            Debug.WriteLine(String.Format("Roll, Pitch, Yaw, Mag, Accel"));



            //          portInterface.Write(new byte[] { (byte)'t' }, 0, 1);
            //         portInterface.Write(BitConverter.GetBytes(simTelemList.Count), 0, 4);









            double lowestError = 999999;
            double lowestAccel = 0;
            double lowestMag = 0;

            //for (int mag = 1; mag < 100; mag++)
            //int mag = 75;//1;//8;
            //int mag = 150;
            //   int mag = 57;
            //int mag = 75;
            //int mag = 2;
            int mag = 1;
            //int mag = 6;
            {
                //for (int accel = 1; accel < 100; accel++)
                //for (int accel = 1; accel < 40; accel++)
                //for (int accel = 1; accel < 1000; accel++)

                //int accel = 1;//1;
                //int accel = 8;
                //       int accel = 7;
                //int accel = 8;
                //int accel = 14;
                //int accel = 1;
                //int accel = 20;
                int accel = 24;
                //int accel = 41;
                {
                    float magVal = mag / 100.0f;
                    float accelVal = accel / 1000.0f;


                    AHRS.reset();

                    //MadgwickAHRS madgwick = new MadgwickAHRS(1 / 20.0f, 0.041f);
                    MadgwickAHRS madgwick = new MadgwickAHRS(1 / 20.0f, accelVal);












                    model.RollMSE = 0;
                    model.PitchMSE = 0;
                    model.YawMSE = 0;


                    /*
                                        portInterface.Write(new byte[]{(byte)'t'},0,1);
                                        portInterface.Write(BitConverter.GetBytes(magVal), 0, sizeof(float));
                                        portInterface.Write(BitConverter.GetBytes(accelVal), 0, sizeof(float));
                    */

                    int counter = 0;

                    GroundControlStation.Model.GroundControlStationModel.Results rollResults = new GroundControlStation.Model.GroundControlStationModel.Results();
                    GroundControlStation.Model.GroundControlStationModel.Results pitchResults = new GroundControlStation.Model.GroundControlStationModel.Results();
                    GroundControlStation.Model.GroundControlStationModel.Results yawResults = new GroundControlStation.Model.GroundControlStationModel.Results();

                    yawResults.magVal = magVal;
                    pitchResults.magVal = magVal;
                    rollResults.magVal = magVal;
                    yawResults.accelVal = accelVal;
                    pitchResults.accelVal = accelVal;
                    rollResults.accelVal = accelVal;



                    model.SimTelm = simTelemList[0];


                    for (int i = 0; i < 10000; i++)
                    {


                        FlightComputerTelemetryMessage data = FlightComputerTelemetryMessage.CreateFromModel(model);

                        SensorDataMessage sensorData = new SensorDataMessage();
                        sensorData.PitchAngularVelocityRadsPerSecond = data.PitchAngularVelocityRadsPerSecond;
                        sensorData.PressureMillibars = data.PressureMillibars;
                        sensorData.RollAngularVelocityRadsPerSecond = data.RollAngularVelocityRadsPerSecond;
                        sensorData.XAccelFrdMss = data.XAccelFrdMss;
                        sensorData.XEcefCm = data.XEcefCm;
                        sensorData.XMagFrd = data.XMagFrd;
                        sensorData.XVEcefCms = data.XVEcefCms;
                        sensorData.YAccelFrdMss = data.YAccelFrdMss;
                        sensorData.YawAngularVelocityRadsPerSecond = data.YawAngularVelocityRadsPerSecond;
                        sensorData.YEcefCm = data.YEcefCm;
                        sensorData.YMagFrd = data.YMagFrd;
                        sensorData.YVEcefCms = data.YVEcefCms;
                        sensorData.ZAccelFrdMss = data.ZAccelFrdMss;
                        sensorData.ZEcefCm = data.ZEcefCm;
                        sensorData.ZMagFrd = data.ZMagFrd;
                        sensorData.ZVEcefCms = data.ZVEcefCms;

                        madgwick.Update(0, 0, 0,
                          sensorData.XAccelFrdMss, sensorData.YAccelFrdMss * -1, sensorData.ZAccelFrdMss,
                          sensorData.XMagFrd, sensorData.YMagFrd * -1, sensorData.ZMagFrd);
                    }


                    foreach (SimulatorTelemetry val in simTelemList)
                    {
                        try
                        {
                            /*
                            val.PitchVelocityRadsPerS = 0;
                            val.YawVelocityRadsPerS = 0;
                            val.RollVelocityRadsPerS = 0;
                            val.XVelocityNEDMs = 0;
                            val.YVelocityNEDMs = 0;
                            val.ZVelocityNEDMs = 0;
                            */





                            //Console.WriteLine(counter++);
                            model.SimTelm = val;

                            FlightComputerTelemetryMessage data = FlightComputerTelemetryMessage.CreateFromModel(model);

                            SensorDataMessage sensorData = new SensorDataMessage();
                            sensorData.PitchAngularVelocityRadsPerSecond = data.PitchAngularVelocityRadsPerSecond;
                            sensorData.PressureMillibars = data.PressureMillibars;
                            sensorData.RollAngularVelocityRadsPerSecond = data.RollAngularVelocityRadsPerSecond;
                            sensorData.XAccelFrdMss = data.XAccelFrdMss;
                            sensorData.XEcefCm = data.XEcefCm;
                            sensorData.XMagFrd = data.XMagFrd;
                            sensorData.XVEcefCms = data.XVEcefCms;
                            sensorData.YAccelFrdMss = data.YAccelFrdMss;
                            sensorData.YawAngularVelocityRadsPerSecond = data.YawAngularVelocityRadsPerSecond;
                            sensorData.YEcefCm = data.YEcefCm;
                            sensorData.YMagFrd = data.YMagFrd;
                            sensorData.YVEcefCms = data.YVEcefCms;
                            sensorData.ZAccelFrdMss = data.ZAccelFrdMss;
                            sensorData.ZEcefCm = data.ZEcefCm;
                            sensorData.ZMagFrd = data.ZMagFrd;
                            sensorData.ZVEcefCms = data.ZVEcefCms;





                            /*
                            for (int i = 0; i < 100000; i++)
                            {
                                madgwick.Update(0, 0, 0,
                                    sensorData.XAccelFrdMss, sensorData.YAccelFrdMss, sensorData.ZAccelFrdMss,
                                    sensorData.XMagFrd, sensorData.YMagFrd, sensorData.ZMagFrd);


                                float y = 0;
                                float p = 0;
                                float r = 0;
                                madgwick.getYawPitchRoll(out y, out p, out r);

                                AHRS.ahrs(sensorData.XAccelFrdMss, sensorData.YAccelFrdMss, sensorData.ZAccelFrdMss,
                                0,0,0,
                                sensorData.XMagFrd, sensorData.YMagFrd, sensorData.ZMagFrd, accelVal, magVal);


                            }
                            float y1 = 0;
                            float p1 = 0;
                            float r1 = 0;
                            madgwick.getYawPitchRoll(out y1, out p1, out r1);


                            */





                            /*
                            fcInterface.Transmit(sensorData);



                            //Send sim model data to FC. 
                            //fcInterface.Transmit(data);

                            //FlightComputerTelemetryMessage telem = (FlightComputerTelemetryMessage)fcInterface.Receive();

                            float yaw = portInterface.ReadFloat();
                            float pitch = portInterface.ReadFloat();
                            float roll = portInterface.ReadFloat();
                            */

                            /*AHRS.ahrs(sensorData.XAccelFrdMss, sensorData.YAccelFrdMss, sensorData.ZAccelFrdMss,
                                sensorData.RollAngularVelocityRadsPerSecond, sensorData.PitchAngularVelocityRadsPerSecond, sensorData.YawAngularVelocityRadsPerSecond,
                                sensorData.XMagFrd, sensorData.YMagFrd, sensorData.ZMagFrd, accelVal, magVal);
                            */


                            /* madgwick.Update(sensorData.RollAngularVelocityRadsPerSecond, sensorData.PitchAngularVelocityRadsPerSecond, sensorData.YawAngularVelocityRadsPerSecond,
                                 sensorData.XAccelFrdMss, sensorData.YAccelFrdMss, sensorData.ZAccelFrdMss,
                                 sensorData.XMagFrd, sensorData.YMagFrd, sensorData.ZMagFrd);*/
                            //madgwick.Update(sensorData.RollAngularVelocityRadsPerSecond, sensorData.PitchAngularVelocityRadsPerSecond, sensorData.YawAngularVelocityRadsPerSecond,
                            madgwick.Update(-1 * sensorData.RollAngularVelocityRadsPerSecond, sensorData.PitchAngularVelocityRadsPerSecond, -1 * sensorData.YawAngularVelocityRadsPerSecond,
                          sensorData.XAccelFrdMss, sensorData.YAccelFrdMss * -1, sensorData.ZAccelFrdMss,
                          sensorData.XMagFrd, sensorData.YMagFrd * -1, sensorData.ZMagFrd);


                            float y2 = 0;
                            float p2 = 0;
                            float r2 = 0;
                            madgwick.getYawPitchRoll(out y2, out p2, out r2);

                            /*
                            float yaw = AHRS.yawRads;
                            float pitch = AHRS.pitchRads;
                            float roll = AHRS.rollRads;
                             */

                            
                            if (y2 < 0) y2 = 360 + y2;

                            float yaw = (float) (y2 * (Math.PI / 180f));
                            float pitch = (float)(p2 * (Math.PI / 180f));
                            float roll = (float)(r2 * (Math.PI / 180f));
                            



                            //if (telem != null)
                            {
                                double rollError = (roll * (180 / Math.PI)) - model.SimTelm.RollDegrees;
                                rollError = Math.Abs(rollError);
                                model.RollMSE += Math.Pow(rollError, 2);



                                double pitchError = (pitch * (180 / Math.PI)) - model.SimTelm.PitchDegrees;
                                pitchError = Math.Abs(pitchError);
                                model.PitchMSE += Math.Pow(pitchError, 2);

                                yaw = (float)(yaw * (180 / Math.PI));

                                if (yaw > 180)
                                {
                                    yaw = yaw - 360;
                                }
                                double simMag = model.SimTelm.MagHeadingDegrees;
                                if (simMag > 180)
                                {
                                    simMag = simMag - 360;
                                }
                                //double yawError = (yaw * (180 / Math.PI)) - model.SimTelm.MagHeadingDegrees;
                                double yawError = yaw - simMag;
                                yawError = Math.Abs(yawError);
                                model.YawMSE += Math.Pow(yawError, 2);


                                rollResults.values.Add(Math.Pow(rollError, 2));
                                pitchResults.values.Add(Math.Pow(pitchError, 2));
                                yawResults.values.Add(Math.Pow(yawError, 2));





                                model.MSEIterations = model.MSEIterations + 1;


                                Debug.WriteLine(String.Format("{0},{1},{2}", rollError, pitchError, yawError));
                                Debug.WriteLine(String.Format("Actual Pitch {0},Calc pitch {1}, Error {2}", model.SimTelm.PitchDegrees, (pitch * (180 / Math.PI)), pitchError));
                                Debug.WriteLine(String.Format("Actual yaw {0},Calc yaw {1}, Error {2}", simMag, yaw, yawError));
                            }
                        }
                        catch (Exception e)
                        {

                        }
                    }

                    model.RollResults.Add(rollResults);
                    model.PitchResults.Add(pitchResults);
                    model.YawResults.Add(yawResults);

                    double totalAvgError = (model.RollMSE + model.PitchMSE + model.YawMSE) / 3;



                    //Debug.WriteLine("total: " + totalAvgError + ", MAG " + mag + ", accel: " + accel);

                    if (totalAvgError < lowestError)
                    {
                        lowestError = totalAvgError;
                        lowestAccel = accel;
                        lowestMag = mag;
                    }

                    //Debug.WriteLine(String.Format("Total: {0}, RollE {1}, PitchE {2}, YawE {3}, Mag {4}, Accel {5}, lowestaccel {6}, lowestMag {7}", totalAvgError, model.RollMSE, model.PitchMSE, model.YawMSE, mag, accel, lowestAccel, lowestMag));
                    Debug.WriteLine(String.Format("{0},{1},{2},{3},{4}", model.RollMSE, model.PitchMSE, model.YawMSE, mag, accel));


                }
            }
            //for 1000 iterations for mag value
            //for 1000 iterations for accel value
            //for each record, send it to the heli
            //retrieve the response
            //compare response with data
            //at end of loop, update if smallest error. 

            for (int i = 0; i < model.RollResults.Count; i++)
            {
                model.RollResults[i].calc();
                model.YawResults[i].calc();
                model.PitchResults[i].calc();

                Debug.WriteLine("Roll," + model.RollResults[i].ToString());
                Debug.WriteLine("Yaw," + model.YawResults[i].ToString());
                Debug.WriteLine("Pitch," + model.PitchResults[i].ToString());
            }

        }



















        [STAThread]
        static void Main43()
        {

           

            /*
            GroundControlStationModel mm = new GroundControlStationModel();
            SimulatorTelemetry tt = new SimulatorTelemetry();
            tt.MagHeadingDegrees = 90;
            tt.PitchDegrees = 0;
            tt.RollDegrees = 0;
            tt.XVelocityNEDMs = 10;
            tt.YVelocityNEDMs = 0;
            tt.ZVelocityNEDMs = 0;

            mm.PreviousXVelocityNEDCms = 500;
            mm.PreviousYVelocityNEDCms = 0;
            mm.PreviousZVelocityNEDCms = 0;


            mm.SimTelm = tt;



            FlightComputerTelemetryMessage dd = FlightComputerTelemetryMessage.CreateFromModel(mm);

            */

            








            //shrink files
            //verify data being received and sent
            //test timing of ahrs, and enhance speed of ahrs. 

            List<SimulatorTelemetry> simTelemList = parseFile();

            SerialPort port = new SerialPort("COM7", 250000, Parity.None, 8, StopBits.One);
            port.ReadTimeout = 50000;
           
            SerialPortInterface portInterface = new SerialPortInterface(port);

            FlightComputerInterface fcInterface = new FlightComputerInterface(portInterface);
   //         fcInterface.Open();

            GroundControlStationModel model = new GroundControlStationModel();
            model.SimTelm = new SimulatorTelemetry();



            model.RollResults = new List<GroundControlStation.Model.GroundControlStationModel.Results>();
            model.YawResults = new List<GroundControlStation.Model.GroundControlStationModel.Results>();
            model.PitchResults = new List<GroundControlStation.Model.GroundControlStationModel.Results>();




            Debug.WriteLine(String.Format("Roll, Pitch, Yaw, Mag, Accel"));



  //          portInterface.Write(new byte[] { (byte)'t' }, 0, 1);
   //         portInterface.Write(BitConverter.GetBytes(simTelemList.Count), 0, 4);






            


            double lowestError = 999999;
            double lowestAccel = 0;
            double lowestMag = 0;

            //for (int mag = 1; mag < 100; mag++)
            //int mag = 75;//1;//8;
            //int mag = 150;
         //   int mag = 57;
            //int mag = 75;
            //int mag = 2;
            int mag = 1;
            //int mag = 6;
            {
                //for (int accel = 1; accel < 100; accel++)
                //for (int accel = 1; accel < 40; accel++)
                //for (int accel = 1; accel < 1000; accel++)
                
                //int accel = 1;//1;
                //int accel = 8;
         //       int accel = 7;
                //int accel = 8;
                //int accel = 14;
                int accel = 1;
                //int accel = 20;
                //int accel = 5;
                {

                    AHRS.reset();

                    MadgwickAHRS madgwick = new MadgwickAHRS(1 / 20.0f, 0.041f);












                    model.RollMSE = 0;
                    model.PitchMSE = 0;
                    model.YawMSE = 0;

                    float magVal = mag / 100.0f;
                    float accelVal = accel / 1000.0f;

                    magVal = .01f;
                    accelVal = .0005f;


/*
                    portInterface.Write(new byte[]{(byte)'t'},0,1);
                    portInterface.Write(BitConverter.GetBytes(magVal), 0, sizeof(float));
                    portInterface.Write(BitConverter.GetBytes(accelVal), 0, sizeof(float));
*/

                    int counter = 0;

                    GroundControlStation.Model.GroundControlStationModel.Results rollResults = new GroundControlStation.Model.GroundControlStationModel.Results();
                    GroundControlStation.Model.GroundControlStationModel.Results pitchResults = new GroundControlStation.Model.GroundControlStationModel.Results();
                    GroundControlStation.Model.GroundControlStationModel.Results yawResults = new GroundControlStation.Model.GroundControlStationModel.Results();

                    yawResults.magVal = magVal;
                    pitchResults.magVal = magVal;
                    rollResults.magVal = magVal;
                    yawResults.accelVal = accelVal;
                    pitchResults.accelVal = accelVal;
                    rollResults.accelVal = accelVal;



                    model.SimTelm = simTelemList[0];


                    for (int i = 0; i < 1000; i++)
                    {
                        

                        FlightComputerTelemetryMessage data = FlightComputerTelemetryMessage.CreateFromModel(model);

                        SensorDataMessage sensorData = new SensorDataMessage();
                        sensorData.PitchAngularVelocityRadsPerSecond = data.PitchAngularVelocityRadsPerSecond;
                        sensorData.PressureMillibars = data.PressureMillibars;
                        sensorData.RollAngularVelocityRadsPerSecond = data.RollAngularVelocityRadsPerSecond;
                        sensorData.XAccelFrdMss = data.XAccelFrdMss;
                        sensorData.XEcefCm = data.XEcefCm;
                        sensorData.XMagFrd = data.XMagFrd;
                        sensorData.XVEcefCms = data.XVEcefCms;
                        sensorData.YAccelFrdMss = data.YAccelFrdMss;
                        sensorData.YawAngularVelocityRadsPerSecond = data.YawAngularVelocityRadsPerSecond;
                        sensorData.YEcefCm = data.YEcefCm;
                        sensorData.YMagFrd = data.YMagFrd;
                        sensorData.YVEcefCms = data.YVEcefCms;
                        sensorData.ZAccelFrdMss = data.ZAccelFrdMss;
                        sensorData.ZEcefCm = data.ZEcefCm;
                        sensorData.ZMagFrd = data.ZMagFrd;
                        sensorData.ZVEcefCms = data.ZVEcefCms;

                        /*madgwick.Update(0, 0, 0,
                          sensorData.XAccelFrdMss, sensorData.YAccelFrdMss * -1, sensorData.ZAccelFrdMss,
                          sensorData.XMagFrd, sensorData.YMagFrd * -1, sensorData.ZMagFrd);*/



                      AHRS.ahrs(sensorData.XAccelFrdMss, sensorData.YAccelFrdMss, sensorData.ZAccelFrdMss,
                                0,0,0,
                                sensorData.XMagFrd, sensorData.YMagFrd, sensorData.ZMagFrd, accelVal, magVal);
                    }


                    foreach (SimulatorTelemetry val in simTelemList)
                    {
                        try
                        {
/*
val.PitchVelocityRadsPerS = 0;
val.YawVelocityRadsPerS = 0;
val.RollVelocityRadsPerS = 0;
val.XVelocityNEDMs = 0;
val.YVelocityNEDMs = 0;
val.ZVelocityNEDMs = 0;
*/





                            //Console.WriteLine(counter++);
                            model.SimTelm = val;

                            FlightComputerTelemetryMessage data = FlightComputerTelemetryMessage.CreateFromModel(model);

                            SensorDataMessage sensorData = new SensorDataMessage();
                            sensorData.PitchAngularVelocityRadsPerSecond = data.PitchAngularVelocityRadsPerSecond;
                            sensorData.PressureMillibars = data.PressureMillibars;
                            sensorData.RollAngularVelocityRadsPerSecond = data.RollAngularVelocityRadsPerSecond;
                            sensorData.XAccelFrdMss = data.XAccelFrdMss;
                            sensorData.XEcefCm = data.XEcefCm;
                            sensorData.XMagFrd = data.XMagFrd;
                            sensorData.XVEcefCms = data.XVEcefCms;
                            sensorData.YAccelFrdMss = data.YAccelFrdMss;
                            sensorData.YawAngularVelocityRadsPerSecond = data.YawAngularVelocityRadsPerSecond;
                            sensorData.YEcefCm = data.YEcefCm;
                            sensorData.YMagFrd = data.YMagFrd;
                            sensorData.YVEcefCms = data.YVEcefCms;
                            sensorData.ZAccelFrdMss = data.ZAccelFrdMss;
                            sensorData.ZEcefCm = data.ZEcefCm;
                            sensorData.ZMagFrd = data.ZMagFrd;
                            sensorData.ZVEcefCms = data.ZVEcefCms;





/*
for (int i = 0; i < 100000; i++)
{
    madgwick.Update(0, 0, 0,
        sensorData.XAccelFrdMss, sensorData.YAccelFrdMss, sensorData.ZAccelFrdMss,
        sensorData.XMagFrd, sensorData.YMagFrd, sensorData.ZMagFrd);


    float y = 0;
    float p = 0;
    float r = 0;
    madgwick.getYawPitchRoll(out y, out p, out r);

    AHRS.ahrs(sensorData.XAccelFrdMss, sensorData.YAccelFrdMss, sensorData.ZAccelFrdMss,
    0,0,0,
    sensorData.XMagFrd, sensorData.YMagFrd, sensorData.ZMagFrd, accelVal, magVal);


}
float y1 = 0;
float p1 = 0;
float r1 = 0;
madgwick.getYawPitchRoll(out y1, out p1, out r1);


*/





                            /*
                            fcInterface.Transmit(sensorData);



                            //Send sim model data to FC. 
                            //fcInterface.Transmit(data);

                            //FlightComputerTelemetryMessage telem = (FlightComputerTelemetryMessage)fcInterface.Receive();

                            float yaw = portInterface.ReadFloat();
                            float pitch = portInterface.ReadFloat();
                            float roll = portInterface.ReadFloat();
                            */

                            AHRS.ahrs(sensorData.XAccelFrdMss, sensorData.YAccelFrdMss, sensorData.ZAccelFrdMss,
				                sensorData.RollAngularVelocityRadsPerSecond, sensorData.PitchAngularVelocityRadsPerSecond, sensorData.YawAngularVelocityRadsPerSecond,
				                sensorData.XMagFrd, sensorData.YMagFrd, sensorData.ZMagFrd, accelVal, magVal);



                           /* madgwick.Update(sensorData.RollAngularVelocityRadsPerSecond, sensorData.PitchAngularVelocityRadsPerSecond, sensorData.YawAngularVelocityRadsPerSecond,
                                sensorData.XAccelFrdMss, sensorData.YAccelFrdMss, sensorData.ZAccelFrdMss,
                                sensorData.XMagFrd, sensorData.YMagFrd, sensorData.ZMagFrd);*/
                            //madgwick.Update(sensorData.RollAngularVelocityRadsPerSecond, sensorData.PitchAngularVelocityRadsPerSecond, sensorData.YawAngularVelocityRadsPerSecond,
                           /* madgwick.Update(-1*sensorData.RollAngularVelocityRadsPerSecond, sensorData.PitchAngularVelocityRadsPerSecond, -1*sensorData.YawAngularVelocityRadsPerSecond,
                          sensorData.XAccelFrdMss, sensorData.YAccelFrdMss * -1, sensorData.ZAccelFrdMss,
                          sensorData.XMagFrd, sensorData.YMagFrd * -1, sensorData.ZMagFrd);*/


                            float y2 = 0;
                            float p2 = 0;
                            float r2 = 0;
                            madgwick.getYawPitchRoll(out y2, out p2, out r2);

                            
                            float yaw = AHRS.yawRads;
                            float pitch = AHRS.pitchRads;
                            float roll = AHRS.rollRads;

                            /*
                            if (y2 < 0) y2 = 360 + y2;

                            float yaw = (float) (y2 * (Math.PI / 180f));
                            float pitch = (float)(p2 * (Math.PI / 180f));
                            float roll = (float)(r2 * (Math.PI / 180f));
                            */



                            //if (telem != null)
                            {
                                double rollError = (roll * (180 / Math.PI)) - model.SimTelm.RollDegrees;
                                rollError = Math.Abs(rollError);
                                model.RollMSE += Math.Pow(rollError, 2);

                                

                                double pitchError = (pitch * (180 / Math.PI)) - model.SimTelm.PitchDegrees;
                                pitchError = Math.Abs(pitchError);
                                model.PitchMSE += Math.Pow(pitchError, 2);

                                yaw = (float) (yaw * (180 / Math.PI));

                                if (yaw > 180)
                                {
                                    yaw = yaw - 360;
                                }
                                double simMag = model.SimTelm.MagHeadingDegrees;
                                if (simMag > 180)
                                {
                                    simMag = simMag - 360;
                                }
                                //double yawError = (yaw * (180 / Math.PI)) - model.SimTelm.MagHeadingDegrees;
                                double yawError = yaw - simMag;
                                yawError = Math.Abs(yawError);
                                model.YawMSE += Math.Pow(yawError, 2);


                                rollResults.values.Add(Math.Pow(rollError, 2));
                                pitchResults.values.Add(Math.Pow(pitchError, 2));
                                yawResults.values.Add(Math.Pow(yawError, 2));
                                




                                model.MSEIterations = model.MSEIterations + 1;


                                Debug.WriteLine(String.Format("{0},{1},{2}", rollError, pitchError, yawError));
                                Debug.WriteLine(String.Format("Actual Pitch {0},Calc pitch {1}, Error {2}", model.SimTelm.PitchDegrees, (pitch * (180 / Math.PI)), pitchError));
                                Debug.WriteLine(String.Format("Actual yaw {0},Calc yaw {1}, Error {2}", simMag, yaw, yawError));
                            }
                        }
                        catch (Exception e)
                        {
                            
                        }
                    }

                    model.RollResults.Add(rollResults);
                    model.PitchResults.Add(pitchResults);
                    model.YawResults.Add(yawResults);

                    double totalAvgError = (model.RollMSE + model.PitchMSE + model.YawMSE) / 3;
                   

                    
                    //Debug.WriteLine("total: " + totalAvgError + ", MAG " + mag + ", accel: " + accel);

                    if (totalAvgError < lowestError)
                    {
                        lowestError = totalAvgError;
                        lowestAccel = accel;
                        lowestMag = mag;
                    }

                    //Debug.WriteLine(String.Format("Total: {0}, RollE {1}, PitchE {2}, YawE {3}, Mag {4}, Accel {5}, lowestaccel {6}, lowestMag {7}", totalAvgError, model.RollMSE, model.PitchMSE, model.YawMSE, mag, accel, lowestAccel, lowestMag));
                    Debug.WriteLine(String.Format("{0},{1},{2},{3},{4}", model.RollMSE, model.PitchMSE, model.YawMSE, mag, accel));


                }
            }
            //for 1000 iterations for mag value
            //for 1000 iterations for accel value
            //for each record, send it to the heli
            //retrieve the response
            //compare response with data
            //at end of loop, update if smallest error. 

            for (int i = 0; i < model.RollResults.Count; i++)
            {
                model.RollResults[i].calc();
                model.YawResults[i].calc();
                model.PitchResults[i].calc();

                Debug.WriteLine("Roll," + model.RollResults[i].ToString());
                Debug.WriteLine("Yaw," + model.YawResults[i].ToString());
                Debug.WriteLine("Pitch," + model.PitchResults[i].ToString());
            }

        }

        private static List<SimulatorTelemetry> parseFile()
        {
            
            List<SimulatorTelemetry> simTelemdata = new List<SimulatorTelemetry>();
            //parse file and create records
            //using (var reader = new StreamReader(@"C:\Heli\GitRepository\UAVHelicopter\CodeProjects\GroundControlStation\logs\ahrsopt\SimulatorLog_09_07_2014_093648.csv"))
            //using (var reader = new StreamReader(@"C:\Heli\GitRepository\UAVHelicopter\CodeProjects\GroundControlStation\logs\ahrsopt\SimulatorLog_09_10_2014_105228.csv"))
            using (var reader = new StreamReader(@"C:\Heli\GitRepository\UAVHelicopter\CodeProjects\GroundControlStation\logs\ahrsopt\SimulatorLog_09_11_2014_051115.csv"))
            {
                reader.ReadLine(); //throw away header.
                while (!reader.EndOfStream)
                {

                    string line = reader.ReadLine();

                    var values = line.Split(',');

                    if (values.Count() == 14)
                    {

                        SimulatorTelemetry obj = new SimulatorTelemetry();
                        obj.PitchVelocityRadsPerS = float.Parse(values[1]);
                        obj.RollVelocityRadsPerS = float.Parse(values[2]);
                        obj.YawVelocityRadsPerS = float.Parse(values[3]);
                        obj.PitchDegrees = float.Parse(values[4]);
                        obj.RollDegrees = float.Parse(values[5]);
                        obj.MagHeadingDegrees = float.Parse(values[6]);
                        obj.LatitudeDegrees = float.Parse(values[7]);
                        obj.LongitudeDegrees = float.Parse(values[8]);
                        obj.ZAltitudeFtAgl = float.Parse(values[9]);
                        obj.ZAltitudeFtMsl = float.Parse(values[10]);
                        obj.XVelocityNEDMs = float.Parse(values[11]);
                        obj.YVelocityNEDMs = float.Parse(values[12]);
                        obj.ZVelocityNEDMs = float.Parse(values[13]);

                        simTelemdata.Add(obj);
                    }
                }
            }

            return simTelemdata;
        }


        static float threeFactorial = 3 * 2 * 1;
        static float fiveFactorial = 5 * 4 * 3 * 2 * 1;
        static float sevenFactorial = 7 * 6 * 5 * 4 * 3 * 2 * 1;
        static float nineFactorial = 9 * 8 * 7 * 6 * 5 * 4 * 3 * 2 * 1;

        //static float fsin(float valueRads)
        static float fsin(float x)
        {


           /* float two = valueRads * valueRads;
            float three = two * valueRads;
            float five = three * two;
            float seven = five * two;
            //float nine = seven * two;


            return valueRads - (three / threeFactorial) + (five / fiveFactorial); //- (seven / sevenFactorial);// +(nine / nineFactorial);
            */

            float sin = 0;

            //always wrap input angle to -PI..PI
            if (x < -3.14159265)
                x += 6.28318531f;
            else
                if (x > 3.14159265)
                    x -= 6.28318531f;

            //compute sine
            if (x < 0)
                sin = 1.27323954f * x + .405284735f * x * x;
            else
                sin = 1.27323954f * x - 0.405284735f * x * x;

            //compute cosine: sin(x + PI/2) = cos(x)
            /*x += 1.57079632;
            if (x > 3.14159265)
                x -= 6.28318531;*/

            return sin;
        }

        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        [STAThread]
        static void Main()
        {


            DateTime start = DateTime.Now;

            for (int i = 0; i < 100000; i++)
            {
                float a = fsin(3.13f);
                float b = (float)(Math.Sin(3.13));
            }

            TimeSpan end = start - DateTime.Now;

            //TODO focus on thread ui update.
            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);

            FlightModes flightMode = FlightModes.RealFlight;
            //FlightModes flightMode = FlightModes.SimulatedFlight;

            //SerialPort port = new SerialPort("COM7", 76800, Parity.None, 8, StopBits.One);
            //SerialPort port = new SerialPort("COM7", 250000, Parity.None, 8, StopBits.One); //MOST RECENT
            //SerialPort port = new SerialPort("COM7", 76800, Parity.None, 8, StopBits.One);
           // SerialPort port = new SerialPort("COM7", 57600, Parity.None, 8, StopBits.One);

            SerialPort port = null;
            if (flightMode == FlightModes.RealFlight)
            {
                //port = new SerialPort("COM12", 57600, Parity.None, 8, StopBits.One);
                port = new SerialPort("COM12", 115200, Parity.None, 8, StopBits.One);
                //port = new SerialPort("COM7", 115200, Parity.None, 8, StopBits.One);
            }
            else
            {
                port = new SerialPort("COM7", 250000, Parity.None, 8, StopBits.One);
            }
     //SerialPort port = new SerialPort("COM12", 57600, Parity.None, 8, StopBits.One);
            //SerialPort port = new SerialPort("COM7", 57600, Parity.None, 8, StopBits.One);
            
 //           port.ReadTimeout = 500;

            SimulatorInterface xInterface = new SimulatorInterface(8089, 49000, IPAddress.Parse("127.0.0.255"));

            SerialPortInterface portInterface = new SerialPortInterface(port);

            FlightComputerInterface fcInterface = new FlightComputerInterface(portInterface);
            fcInterface.Open();

            

            GroundControlStationModel model = new GroundControlStationModel();
            model.SimTelm = new SimulatorTelemetry();


            /* Crap gains.
            model.YawIntegralGain = .806f;
            model.YawProportionalGain = 1.116f;
            model.YawDerivativeGain = .391f;
            model.YawAntiWindupGain = 0.135f;

            model.XIntegralGain = 0.0f;
            //model.XProportionalGain = 4.101f;
            //model.XDerivativeGain = 8.449f;
            model.XProportionalGain = .00289f;
            model.XDerivativeGain = .01859f;
            model.XAntiWindupGain = 0.0f;
            model.LongitudeInnerLoopGain = 0.000541f;
            model.PitchAngularVelocityGain = .0005235f;

            model.YIntegralGain = 0.0f;
            model.YProportionalGain = 5.638f;
            model.YDerivativeGain = 1.116f;
            model.YAntiWindupGain = 0.0f;
            model.LateralInnerLoopGain = 1.696f;
            model.RollAngularVelocityGain = .241f;


            model.ZIntegralGain = .0005f;
            model.ZProportionalGain = 0.003f;
            model.ZDerivativeGain = .0061f;
            model.ZAntiWindupGain = .6685f;
            */






















            //// //good gains - 5/31/2014
            //model.YawIntegralGain = .806f;
            //model.YawProportionalGain = 1.116f;
            ////model.YawDerivativeGain = .391f;
            //model.YawDerivativeGain = .391f;
            //model.YawAntiWindupGain = 0.135f;

            ///*
            //model.YawIntegralGain = 0f;
            //model.YawProportionalGain = 1.116f;
            ////model.YawDerivativeGain = .391f;
            //model.YawDerivativeGain = 0f;
            //model.YawAntiWindupGain = 0f;
            // * */

            //model.XIntegralGain = 0.0f;
            ////model.XProportionalGain = 4.101f;
            ////model.XDerivativeGain = 8.449f;
            //model.XProportionalGain = .00019f;
            //model.XDerivativeGain = .00153f;
            //model.XAntiWindupGain = 0.0f;
            //model.LongitudeInnerLoopGain = 1.7081f;
            //model.PitchAngularVelocityGain = 1.77509f;

            //model.YIntegralGain = 0.0f;
            //// model.YProportionalGain = 5.638f;
            ////model.YProportionalGain = .01439f;
            //model.YProportionalGain = .04577f;
            ////model.YDerivativeGain = 1.116f;
            ////model.YDerivativeGain = .02869f;
            ////model.YDerivativeGain = .00268f;//mostrecent 4/12/2014
            //model.YDerivativeGain = .13043f;
            //model.YAntiWindupGain = 0.0f;
            ////model.LateralInnerLoopGain = 1.696f;
            ////model.LateralInnerLoopGain = .53623f;
            ////model.LateralInnerLoopGain = .38271f;//mostrecent 4/12
            //model.LateralInnerLoopGain = .36102f;
            ////model.RollAngularVelocityGain = .241f;
            //model.RollAngularVelocityGain = .04348f;

            ///*
            //model.ZIntegralGain = .0005f;
            //model.ZProportionalGain = 0.003f;
            //model.ZDerivativeGain = .0061f;
            //model.ZAntiWindupGain = .6685f;
            //*/

            //model.ZIntegralGain = .000874f;
            //model.ZProportionalGain = 0.00445f;
            //model.ZDerivativeGain = .430435f;
            ////model.ZAntiWindupGain = .000874f;
            //model.ZAntiWindupGain = .300874f;









            //experimental gains - 5/31/2014
            /*
            model.YawIntegralGain = .02f;
            model.YawProportionalGain = .2f;
            model.YawDerivativeGain = 0.0f;
            model.YawAntiWindupGain = 0.00135f;


            model.XIntegralGain = 0.1f;
            model.XProportionalGain = .1500f;
            model.XDerivativeGain = 0.0f;
            model.XAntiWindupGain = 0.0135f;
            model.LongitudeInnerLoopGain = 4.5f;
            model.PitchAngularVelocityGain = 1.77509f;

            model.YIntegralGain = 0.10f;
            model.YProportionalGain = .1500f;
            model.YDerivativeGain = .0f;
            model.YAntiWindupGain = 0.0f;
            model.LateralInnerLoopGain = 4.5f;
            model.RollAngularVelocityGain = .04348f;


            model.ZIntegralGain = 1.5f;
            model.ZProportionalGain = .75f;
            model.ZDerivativeGain = 0.0f;
            model.ZAntiWindupGain = .300874f;
            */


            //experimental gains - 5/31/2014 - most recent
            /*
             model.YawIntegralGain = .806f;
             //model.YawProportionalGain = 1.116f;
             model.YawProportionalGain = 2.0f;
             model.YawDerivativeGain = .391f;
             model.YawAntiWindupGain = 0.135f;


            model.XIntegralGain = 0.0f;
            model.XProportionalGain = .0151f;
            model.XDerivativeGain = .052f;
            model.XAntiWindupGain = 0.0f;
            model.LongitudeInnerLoopGain = 1.7081f;
            //model.PitchAngularVelocityGain = 1.77509f;
            model.PitchAngularVelocityGain = 0.043f;

            model.YIntegralGain = 0.0f;
           
            model.YProportionalGain = .015f;
            model.YDerivativeGain = .049f;
            model.YAntiWindupGain = 0.0f;
            model.LateralInnerLoopGain = 1.73f;
            //model.RollAngularVelocityGain = 0.768f;
            model.RollAngularVelocityGain = 0.014f;

            model.ZIntegralGain = .001f;
            model.ZProportionalGain = 0.004f;
            model.ZDerivativeGain = .430435f;
            model.ZAntiWindupGain = .300874f;
             */


            //zeroed out gains - most recent 8/3/2014
            /*
            //model.YawIntegralGain = 0.072f;
            model.YawIntegralGain = 0.00f;
            model.YawProportionalGain = .507f;
            model.YawDerivativeGain = 0.0f;
            //model.YawAntiWindupGain = 0.02f;
            model.YawAntiWindupGain = 0.00f;


            model.XIntegralGain = 0.0f;
            model.XProportionalGain = .0151f;
            model.XDerivativeGain = 0.0f;
            model.XAntiWindupGain = 0.0f;
            //model.LongitudeInnerLoopGain = 1.0f;
            model.LongitudeInnerLoopGain = 0.17081f;
            model.PitchAngularVelocityGain = 0.0f;

            model.YIntegralGain = 0.0f;

            model.YProportionalGain = .0151f;
            model.YDerivativeGain = 0.0f;
            model.YAntiWindupGain = 0.0f;
            //model.LateralInnerLoopGain = 1.0f;
            model.LateralInnerLoopGain = 0.17081f;
            model.RollAngularVelocityGain = 0.0f;

            model.ZIntegralGain = 0.0f;
            model.ZProportionalGain = 0.004f;
            model.ZDerivativeGain = 0.0f;
            model.ZAntiWindupGain = 0.0f;
            */
            






            //experimental gains 9/9/2014
            /*model.YawIntegralGain = 0f;//.3f;
            model.YawProportionalGain = .400f;
            model.YawDerivativeGain = .0f;
            model.YawAntiWindupGain = 0f;// 0.05f;


            model.XIntegralGain = 0.0f;
            model.XProportionalGain = .0151f;
            model.XDerivativeGain = .052f;
            model.XAntiWindupGain = 0.0f;
            model.LongitudeInnerLoopGain = 1.7081f;
            //model.PitchAngularVelocityGain = 1.77509f;
            model.PitchAngularVelocityGain = 0.043f;

            model.YIntegralGain = 0.0f;

            model.YProportionalGain = .015f;
            model.YDerivativeGain = .049f;
            model.YAntiWindupGain = 0.0f;
            model.LateralInnerLoopGain = 1.73f;
            //model.RollAngularVelocityGain = 0.768f;
            model.RollAngularVelocityGain = 0.014f;

            model.ZIntegralGain = .001f;
            model.ZProportionalGain = 0.004f;
            model.ZDerivativeGain = .430435f;
            model.ZAntiWindupGain = .300874f;*/
































            /*
            model.YawIntegralGain = .806f;
            model.YawProportionalGain = 1.116f;
            model.YawDerivativeGain = .391f;
            model.YawAntiWindupGain = 0.135f;

            model.XIntegralGain = 0.0f;
            //model.XProportionalGain = 4.101f;
            //model.XDerivativeGain = 8.449f;
            model.XProportionalGain = 0.0f;
            model.XDerivativeGain = 0f;
            model.XAntiWindupGain = 0.0f;
            model.LongitudeInnerLoopGain = 1.667f;
            model.PitchAngularVelocityGain = 1.638f;

            model.YIntegralGain = 0.0f;
            model.YProportionalGain = 5.638f;
            model.YDerivativeGain = 1.116f;
            model.YAntiWindupGain = 0.0f;
            model.LateralInnerLoopGain = 1.696f;
            model.RollAngularVelocityGain = .241f;


            model.ZIntegralGain = .0005f;
            model.ZProportionalGain = 0.003f;
            model.ZDerivativeGain = .0061f;
            model.ZAntiWindupGain = .6685f;
             */





            /*model.ZIntegralGain = .019f;
            model.ZProportionalGain = 0.05901f;
            model.ZDerivativeGain = .011f;
            model.ZAntiWindupGain = .1f;*/






            model.YawIntegralGain = .263f;//0.333f;
            model.YawProportionalGain = .797f;//.42f;
            model.YawDerivativeGain = .172f;// 0.168f;
            //model.YawAntiWindupGain = 0.02f;
            model.YawAntiWindupGain = 0.11f;


            model.XIntegralGain = 0.0f;
            model.XProportionalGain = 0.000071f;//.00075f;//.001f;//.623f;//.0151f;
            model.XDerivativeGain = .00025f;//.004f;//1.783f;//0.0f;
            model.XAntiWindupGain = 0.0f;
            //model.LongitudeInnerLoopGain = 1.0f;
            model.LongitudeInnerLoopGain = .272f;//.252f;//0.17081f;
            model.PitchAngularVelocityGain =  .972f;//0.0f;

            model.YIntegralGain = 0.0f;

            model.YProportionalGain = 0.000114f;//.00151f;//.0151f;
            model.YDerivativeGain = 0f;//.0009f;//.009f;//0.0f;
            model.YAntiWindupGain = 0.0f;
            //model.LateralInnerLoopGain = 1.0f;
            model.LateralInnerLoopGain = .43f;//.277f;//0.17081f;
            model.RollAngularVelocityGain = .014f;//.739f;//0.0f;

            model.ZIntegralGain = 0f;//.0005f;// .001f;
            model.ZProportionalGain = .0015f;//0.003f;
            model.ZDerivativeGain = .22f;
            model.ZAntiWindupGain = .6685f;// .300874f;

























            /* orig 3/30/ */
            /*
            model.YawIntegralGain = .014f;
            model.YawProportionalGain = .073f;
            model.YawDerivativeGain = .01f;
            model.YawAntiWindupGain = 0.135f;
             
            model.XIntegralGain = 0.0f;
            model.XProportionalGain = .217f;
            model.XDerivativeGain = 1.859f;
            model.XAntiWindupGain = 0.0f;
            model.LongitudeInnerLoopGain = .015f;
            model.PitchAngularVelocityGain = .159f;

            model.YIntegralGain = 0.0f;
            model.YProportionalGain = 7.246f;
            model.YDerivativeGain = .014f;
            model.YAntiWindupGain = 0.0f;
            model.LateralInnerLoopGain = .036f;
            model.RollAngularVelocityGain = .652f;


            model.ZIntegralGain = .019f;
            model.ZProportionalGain = 0.05901f;
            model.ZDerivativeGain = .011f;
            model.ZAntiWindupGain = .1f;

            */

            /*
            model.ZIntegralGain = .02f;
            model.ZProportionalGain = 1.901f;
            model.ZDerivativeGain = 1.141f;
            model.ZAntiWindupGain = .1f;
             */


            ///*model.YawIntegralGain = .008f;
            //model.YawDerivativeGain = .85f;
            //model.YawProportionalGain = 1.0f;
            //model.YawAntiWindupGain = 0.1f;
            //*/

            ////original from other program
            ///*
            // * model.YawIntegralGain = .5f;
            //model.YawProportionalGain = 1.8471f;
            //model.YawDerivativeGain = model.YawProportionalGain * (0.09f);
            //model.YawAntiWindupGain = 0.1f;
            // */
            ///*model.YawIntegralGain = .05f;
            //model.YawProportionalGain = .018471f;
            //model.YawDerivativeGain = model.YawProportionalGain * (0.09f);
            //model.YawAntiWindupGain = 0.01f;*/

            //model.YawIntegralGain = .00f;
            //model.YawProportionalGain = 0.0058471f;
            //model.YawDerivativeGain = model.YawProportionalGain * (0.09f);
            //model.YawAntiWindupGain = 0.1f;

            //model.XIntegralGain = 0.0f;
            //model.XProportionalGain = .289f;
            //model.XDerivativeGain = 1.859f;
            //model.XAntiWindupGain = 0.0f; 
            //model.LongitudeInnerLoopGain = .031f;
            //model.PitchAngularVelocityGain = .03f;

            //model.YIntegralGain = 0.0f;
            //model.YProportionalGain = 1.437f;
            //model.YDerivativeGain = 2.8363f;
            //model.YAntiWindupGain = 0.0f; 
            //model.LateralInnerLoopGain = .092f;
            //model.RollAngularVelocityGain = .048f;

            ///*
            //model.ZIntegralGain = .02f;
            //model.ZProportionalGain = 1.901f;
            //model.ZDerivativeGain = 1.141f;
            //model.ZAntiWindupGain = .1f;
            // */

            //model.ZIntegralGain = .02f;
            //model.ZProportionalGain = 0.05901f;
            //model.ZDerivativeGain = .01141f;
            //model.ZAntiWindupGain = .1f;


            GroundControlStationController gcsController = null;

            if (flightMode == FlightModes.RealFlight)
            {
                gcsController = new GroundControlStationController(xInterface, fcInterface, GroundControlStationController.DATATOSEND.SENSORDATA, 1);
            }
            else
            {
                gcsController = new GroundControlStationController(xInterface, fcInterface, GroundControlStationController.DATATOSEND.SENSORDATA, 0);
            }

            gcsController.Model = model;

            GroundControlStationForm gcsView = new GroundControlStationForm(gcsController);

            gcsController.View = gcsView;

            gcsController.Start();

            Application.Run(gcsView);

            gcsController.Stop();
        }
    }
}
