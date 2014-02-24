using GroundControlStation.Interfaces;
using GroundControlStation.Views;
using System;
using System.Collections.Generic;
using System.IO.Ports;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace HelicopterIntegrationTests
{
    public class SensorReader
    {
        static short readShort(SerialPortInterface portInterface)
        {
            short b1 = (short) (portInterface.ReadByte() << 8);
            short b2 = (short) portInterface.ReadByte();

            return (short) (b1 | b2);
        }

        static float readFloat(SerialPortInterface portInterface)
        {
            short b1 = (short)(portInterface.ReadByte() << 8);
            short b2 = (short)portInterface.ReadByte();

            return (float)(b1 | b2);
        }




        //Test reading barometer.
        public static void Main()
        {
            double baseAltitudeFeet = 0.0;
            int counter2 = 0;

            double alpha = .15;

            double previousAltitudeFeet = 0.0;
            bool isReady = false;

            SerialPort port = new SerialPort("COM7", 9600, Parity.None, 8, StopBits.One);
            SerialPortInterface portInterface = new SerialPortInterface(port);
            portInterface.Open();


            GraphForm rawTemp = new GraphForm("RawTemp");
            GraphForm rawPress = new GraphForm("RawPress");
            GraphForm temp = new GraphForm("Temp");
            GraphForm press = new GraphForm("Pressure");
            GraphForm alt = new GraphForm("altitude");

            rawTemp.Visible = true;
            rawPress.Visible = true;
            temp.Visible = true;
            press.Visible = true;
            alt.Visible = true;

            while (true)
            {
                int var = 0;
                while (var != 'S')
                {
                    var = portInterface.ReadByte();
                }
                uint rwTemp = portInterface.ReadUInt(portInterface);
                rawTemp.AddValueToGraph(rwTemp);

                uint rwPress = portInterface.ReadUInt(portInterface);
                rawPress.AddValueToGraph(rwPress);
                int temperature = portInterface.ReadInt(portInterface);
                temp.AddValueToGraph(temperature);
                int pressure = portInterface.ReadInt(portInterface);
                press.AddValueToGraph(pressure);

                //Application.DoEvents();

                //altitude equation from: http://www.barnardmicrosystems.com/L4E_FMU_sensors.htm#Pressure
                double altitudemeters = (288.15 / (6.5 / 1000.0)) * (1 - (Math.Pow((pressure / 101325.0), (6.5 / 1000.0) * (287.052 / 9.78))));
                double altitudefeet = altitudemeters * 3.28084;




                //*******note the barometer appears to need more time to settle
                if (counter2 == 40)
                {
                    baseAltitudeFeet = altitudefeet;
                    counter2 = 80;
                }
                else if (counter2 == 80)
                {
                    //do nothing
                }
                else
                {
                    counter2++;
                }

                if (isReady)
                {
                    //Danger *** This algorithm drifts. Sometimes it takes like 10 seconds
                    //or more to stabalize. So you have to wait a long time before you should 
                    //read the altitude and set it as the base altitude because of how long it takes. 
                    //Also, as the barometer heats up, the altitude changes. This isn't good as the barometer's
                    //temperature will probably change throughout the flight
                    double altitudefeettemp = (double)(altitudefeet - baseAltitudeFeet);
                    altitudefeet = alpha * (altitudefeettemp) + (1 - alpha) * previousAltitudeFeet;
                    previousAltitudeFeet = altitudefeet;

                    alt.AddValueToGraph(altitudefeet);
                }
                else
                {
                    altitudefeet = (double)(altitudefeet - baseAltitudeFeet);
                }


                if (altitudefeet < 20 && altitudefeet != 0)
                {
                    isReady = true;
                }

            }
        }








        //public static void Main()
        //{
        //    SerialPort port = new SerialPort("COM7", 9600, Parity.None, 8, StopBits.One);
        //    SerialPortInterface portInterface = new SerialPortInterface(port);
        //    portInterface.Open();


        //    GraphForm magX = new GraphForm("MagX");
        //    GraphForm magZ = new GraphForm("MagZ");
        //    GraphForm magY = new GraphForm("MagY");
        //    GraphForm frdmagX = new GraphForm("FRDMagX");
        //    GraphForm frdmagZ = new GraphForm("FRDMagZ");
        //    GraphForm frdmagY = new GraphForm("FRDMagY");

        //    magX.Visible = true;
        //    magZ.Visible = true;
        //    magY.Visible = true;
        //    frdmagX.Visible = true;
        //    frdmagZ.Visible = true;
        //    frdmagY.Visible = true;

        //    while (true)
        //    {
        //        int var = 0;
        //        while (var != 'S')
        //        {
        //            var = portInterface.ReadByte();
        //        }
        //        magX.AddValueToGraph(readShort(portInterface));
        //        magY.AddValueToGraph(readShort(portInterface));
        //        magZ.AddValueToGraph(readShort(portInterface));
        //        frdmagX.AddValueToGraph(readShort(portInterface));
        //        frdmagY.AddValueToGraph(readShort(portInterface));
        //        frdmagZ.AddValueToGraph(readShort(portInterface));
                
        //        Application.DoEvents();
        //    }
        //}


        //public static void Main()
        //{
        //    SerialPort port = new SerialPort("COM7", 9600, Parity.None, 8, StopBits.One);
        //    SerialPortInterface portInterface = new SerialPortInterface(port);
        //    portInterface.Open();


        //    GraphForm magX = new GraphForm("MagX");
        //    GraphForm magZ = new GraphForm("MagZ");
        //    GraphForm magY = new GraphForm("MagY");

        //    magX.Visible = true;
        //    magZ.Visible = true;
        //    magY.Visible = true;


        //    while (true)
        //    {
        //        int var = 0;
        //        while (var != 'S')
        //        {
        //            var = portInterface.ReadByte();
        //        }
        //        magX.AddValueToGraph(readShort(portInterface));
        //        magZ.AddValueToGraph(readShort(portInterface));
        //        magY.AddValueToGraph(readShort(portInterface));

        //        Application.DoEvents();
        //    }
        //}


        //public static void Main()
        //{
        //    SerialPort port = new SerialPort("COM7", 9600, Parity.None, 8, StopBits.One);
        //    SerialPortInterface portInterface = new SerialPortInterface(port);
        //    portInterface.Open();


        //    GraphForm accelRawX = new GraphForm("accelRawX");
        //    GraphForm accelRawY = new GraphForm("accelRawY");
        //    GraphForm accelRawZ = new GraphForm("accelRawZ");
        //    GraphForm gyroRawX = new GraphForm("gyroRawX");
        //    GraphForm gyroRawY = new GraphForm("gyroRawY");
        //    GraphForm gyroRawZ = new GraphForm("gyroRawZ");
        //    GraphForm accelFrdX = new GraphForm("accelFrdX");
        //    GraphForm accelFrdY = new GraphForm("accelFrdY");
        //    GraphForm accelFrdZ = new GraphForm("accelFrdZ");
        //    GraphForm gyroFrdX = new GraphForm("gyroFrdX");
        //    GraphForm gyroFrdY = new GraphForm("gyroFrdY");
        //    GraphForm gyroFrdZ = new GraphForm("gyroFrdZ");

        //    accelRawX.Visible = true;
        //    accelRawY.Visible = true;
        //    accelRawZ.Visible = true;
        //    gyroRawX.Visible = true;
        //    gyroRawY.Visible = true;
        //    gyroRawZ.Visible = true;
        //    accelFrdX.Visible = true;
        //    accelFrdY.Visible = true;
        //    accelFrdZ.Visible = true;
        //    gyroFrdX.Visible = true;
        //    gyroFrdY.Visible = true;
        //    gyroFrdZ.Visible = true;


        //    while (true)
        //    {
        //        int var = 0;
        //        while (var != 'S')
        //        {
        //            var = portInterface.ReadByte();
        //        }
        //        accelRawX.AddValueToGraph(readShort(portInterface));
        //        accelRawY.AddValueToGraph(readShort(portInterface));
        //        accelRawZ.AddValueToGraph(readShort(portInterface));
        //        gyroRawX.AddValueToGraph(readShort(portInterface));
        //        gyroRawY.AddValueToGraph(readShort(portInterface));
        //        gyroRawZ.AddValueToGraph(readShort(portInterface));

        //        accelFrdX.AddValueToGraph(readFloat(portInterface));
        //        accelFrdY.AddValueToGraph(readFloat(portInterface));
        //        accelFrdZ.AddValueToGraph(readFloat(portInterface));
        //        gyroFrdX.AddValueToGraph(readFloat(portInterface));
        //        gyroFrdY.AddValueToGraph(readFloat(portInterface));

        //        float x = readFloat(portInterface);
        //        gyroFrdZ.AddValueToGraph(x);

        //        Application.DoEvents();
        //    }
        //}
    }
}
