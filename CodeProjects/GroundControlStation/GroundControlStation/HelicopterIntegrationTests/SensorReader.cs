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

        
        public static void Main()
        {
            SerialPort port = new SerialPort("COM7", 115200, Parity.None, 8, StopBits.One);
            SerialPortInterface portInterface = new SerialPortInterface(port);
            portInterface.Open();

            
            GraphForm yawG = new GraphForm("yaw");
            GraphForm pitchG = new GraphForm("pitch");
            GraphForm rollG = new GraphForm("roll");
            GraphForm lax = new GraphForm("lax");
            GraphForm lay = new GraphForm("lay");
            GraphForm laz = new GraphForm("laz");
            yawG.Visible = true;
            pitchG.Visible = true;
            rollG.Visible = true;
            lax.Visible = true;
            lay.Visible = true;
            laz.Visible = true;
            
            
            /*
            GraphForm accxg = new GraphForm("accx");
            GraphForm accyg = new GraphForm("accy");
            GraphForm acczg = new GraphForm("accz");
            GraphForm gyroxg = new GraphForm("gyrox");
            GraphForm gyroyg = new GraphForm("gyroy");
            GraphForm gyrozg = new GraphForm("gyroz");
            GraphForm magxg = new GraphForm("magx");
            GraphForm magyg = new GraphForm("magy");
            GraphForm magzg = new GraphForm("magz");
            accxg.Visible = true;
            accyg.Visible = true;
            acczg.Visible = true;
            gyroxg.Visible = true;
            gyroyg.Visible = true;
            gyrozg.Visible = true;
            magxg.Visible = true;
            magyg.Visible = true;
            magzg.Visible = true;
            */

            port.DiscardInBuffer();
            while (true)
            {
                while (portInterface.ReadByte() != 'S')
                {

                }

                
                float yaw = portInterface.ReadFloat() * (float)(180.0f / Math.PI);
                float pitch = portInterface.ReadFloat() * (float)(180.0f / Math.PI);
                float roll = portInterface.ReadFloat() * (float)(180.0f / Math.PI);
                float lineX = portInterface.ReadFloat();
                float lineY = portInterface.ReadFloat();
                float lineZ = portInterface.ReadFloat();
                yawG.AddValueToGraph(yaw);
                pitchG.AddValueToGraph(pitch);
                rollG.AddValueToGraph(roll);
                lax.AddValueToGraph(lineX);
                lay.AddValueToGraph(lineY);
                laz.AddValueToGraph(lineZ);
                
                
                /*
                float accx = portInterface.ReadFloat();
                float accy = portInterface.ReadFloat();
                float accz = portInterface.ReadFloat();
                float gyrox = portInterface.ReadFloat();
                float gyroy = portInterface.ReadFloat();
                float gyroz = portInterface.ReadFloat();
                float magx = portInterface.ReadFloat();
                float magy = portInterface.ReadFloat();
                float magz = portInterface.ReadFloat();
                accxg.AddValueToGraph(accx);
                accyg.AddValueToGraph(accy);
                acczg.AddValueToGraph(accz);
                gyroxg.AddValueToGraph(gyrox);
                gyroyg.AddValueToGraph(gyroy);
                gyrozg.AddValueToGraph(gyroz);
                magxg.AddValueToGraph(magx);
                magyg.AddValueToGraph(magy);
                magzg.AddValueToGraph(magz);                
                */



                Application.DoEvents();
            }

        }




        
        //public static void Main()
        //{
        //    SerialPort port = new SerialPort("COM7", 9600, Parity.None, 8, StopBits.One);
        //    SerialPortInterface portInterface = new SerialPortInterface(port);
        //    portInterface.Open();

        //    //var bytes = new char[400];
        //    var bytes = new byte[40000];

        //    port.DiscardInBuffer();
        //    while (true)
        //    {
        //        while (portInterface.ReadByte() != 'T')
        //        {

        //        }

        //        for (int i = 0; i < 40000; i++)
        //        {
        //            //bytes[i] = (char)portInterface.ReadByte();
        //            bytes[i] = portInterface.ReadByte();
        //        }

        //    }

        //}
         
        
        //public static void Main()
        //{
        //    SerialPort port = new SerialPort("COM7", 115200, Parity.None, 8, StopBits.One);
        //    SerialPortInterface portInterface = new SerialPortInterface(port);
        //    portInterface.Open();


        //    GraphForm x = new GraphForm("x");
        //    GraphForm y = new GraphForm("y");
        //    x.Visible = true;
        //    y.Visible = true;

        //    port.DiscardInBuffer();
        //    while (true)
        //    {
        //        while (portInterface.ReadByte() != 'T')
        //        {

        //        }

        //        float xNedCm = portInterface.ReadFloat();
        //        float yNedCm = portInterface.ReadFloat();
        //        float zNedCm = portInterface.ReadFloat();

        //        long initialx = portInterface.ReadInt();
        //        long initialy = portInterface.ReadInt();
        //        long initialz = portInterface.ReadInt();
        //        long curx = portInterface.ReadInt();
        //        long cury = portInterface.ReadInt();
        //        long curz = portInterface.ReadInt();
        //        long diffx = portInterface.ReadInt();
        //        long diffy = portInterface.ReadInt();
        //        long diffz = portInterface.ReadInt();
        //        short errorCount1 = portInterface.ReadShort();
        //        short errorCount2 = portInterface.ReadShort();
        //        int b = 0;
        //        System.Diagnostics.Debug.WriteLine("error count1 " + errorCount1 + ", error count2 " + errorCount2 + ", intx " + initialx + ", inty " + initialy + ", xned " + xNedCm + ", yned " + yNedCm);  


        //    }

        //}
         

    //    //Test reading GPS.
    //    public static void Main()
    //    {
    //        SerialPort port = new SerialPort("COM7", 9600, Parity.None, 8, StopBits.One);
    //        SerialPortInterface portInterface = new SerialPortInterface(port);
    //        portInterface.Open();


    //        GraphForm lat = new GraphForm("Latitude");
    //        GraphForm lon = new GraphForm("Longitude");
    //        lat.Visible = true;
    //        lon.Visible = true;

    //        byte var = 0;
    //        int counter = 0;
    //        byte[] bytes = new byte[80];


    //        port.DiscardInBuffer();
    //        while (true)
    //        {
    //            //List<byte> bts = new List<byte>();

    //            while (true)
    //            {

    //               // bts.Add(portInterface.ReadByte());
    //                /*
    //                byte[] buff = new byte[34];

    //                for (int i = 0; i < 34; i++)
    //                {
    //                    buff[i] = portInterface.ReadByte();
    //                }

    //                int a = 0;
    //                a++;
    //                */

    //                /**
    //                 * This is needed to prevent getting out of synch. After a restart
    //                 * the system could have already transmitted half of the data, so the first message
    //                 * after a restart will be junk. Then this will snychronize the messages after the first
    //                 * transmit.
    //                 */
    //                while (portInterface.ReadByte() != 'T')
    //                {

    //                }

    //                byte bb = 0;
    //                bb = portInterface.ReadByte();
    //                bb = portInterface.ReadByte();
    //                bb = portInterface.ReadByte();
                    
    //                int latitude = portInterface.ReadInt();
    //                int longitude = portInterface.ReadInt();
    //                int positionAcc = portInterface.ReadInt();
    //                short fixStatus = portInterface.ReadShort();
    //                int x = portInterface.ReadInt();
    //                int y = portInterface.ReadInt();
    //                int z = portInterface.ReadInt();
    //                int b = 0;
                    
    //                Console.WriteLine("lat: " + latitude.ToString() + " long: " + longitude.ToString() + " pos acc " + positionAcc.ToString() + " fixStatus " + fixStatus.ToString() +  " x: " + x.ToString() + " y: " + y.ToString() +" z " +  z.ToString());
                     





    //                /*
    //                bytes = new byte[80];

    //                for (int i = 0; i < 80; i++)
    //                {
    //                    var = portInterface.ReadByte();

    //                    bytes[i] = var;
    //                }

    //                byte b = 2;
    //                 */

    //                /*
    //                var = portInterface.ReadByte();
                      
                     
    //                if (var == 'S')
    //                {
    //                    counter = 0;
    //                    System.Diagnostics.Debug.WriteLine("Counter " + (bytes[0] << 24 | bytes[1] << 16 | bytes[2] << 8 | bytes[3]));
    //                }
    //                else if (var == 'B')
    //                {
    //                    System.Diagnostics.Debug.WriteLine("Timeout " + ((int)portInterface.ReadByte()).ToString());
    //                }
    //                else
    //                {
    //                    bytes[counter++] = var;
    //                }
    //                 * */

    //                //counter++;
    //                /*byte[] bytes2 = new byte[28];

                    

    //                for (int i = 0; i < 28; i++)
    //                {
    //                    var = portInterface.ReadByte();
    //                    bytes2[i] = var;
    //                }

    //                int b = 3;
    //                System.Diagnostics.Debug.WriteLine("Counter " + (bytes2[13] << 24 | bytes2[12] << 16 | bytes2[11] << 8 | bytes2[10]));
    //                 */ 
                     
    //                /*
    //                bytes [counter++] = var;

    //                if (counter == 3)
    //                {
    //                    counter = 0;
    //                    System.Diagnostics.Debug.WriteLine("Counter " + (bytes[0] <<24 | bytes[1] <<16 | bytes[2] << 8 | bytes[3]));
    //                }
    //                 */

    //                /*
    //                if (var == 0xB5)
    //                {
    //                    System.Diagnostics.Debug.WriteLine("Counter" + counter);
    //                    counter = 1;
                        
    //                }

    //                if (counter == 11 | counter == 12 | counter == 13 | counter == 14)
    //                {
    //                    //bytes[14 - counter] = var;
    //                    bytes[counter - 11] = var;
    //                }
    //                if (counter == 15)
    //                {
    //                    System.Diagnostics.Debug.WriteLine("Number" + BitConverter.ToInt32(bytes,0));
    //                }

    //                System.Diagnostics.Debug.Write((char)var);
    //                 */
    //                /*
    //                if (var == 'S')
    //                {
    //                    int bb = portInterface.ReadInt();
    //                    System.Diagnostics.Debug.WriteLine(" Int: " + bb.ToString());
                        
    //                }*/
    //            }

                
    //            while (var != 'S')
    //            {
    //                var = portInterface.ReadByte();
    //            }

    //            //Note:The status value might come back as a large value because after restart, the value S gets sent as one of the bytes ruining the synchronization
    //            /*
    //            //Read raw lat/long
    //            System.Diagnostics.Debug.WriteLine(portInterface.ReadInt(portInterface));
    //            int rwLat = portInterface.ReadInt(portInterface);
    //            lat.AddValueToGraph(rwLat);

    //            int rwLon = portInterface.ReadInt(portInterface);
    //            lon.AddValueToGraph(rwLon);*/

                
    //            //Read raw messages
    //            var = 0;
    //            short status = portInterface.ReadShort();
    //            System.Diagnostics.Debug.WriteLine("status: " + status); // status
    //            while (var != 'S')
    //            {
    //                var = portInterface.ReadByte();
    //                System.Diagnostics.Debug.Write((char)var);
    //            }



    //            //Read xned
    //            //System.Diagnostics.Debug.WriteLine(portInterface.ReadFloat().ToString());
                 
    //            Application.DoEvents();
    //        }
    //    }





    //    ////Test reading barometer.
    //    //public static void Main()
    //    //{
    //    //    double baseAltitudeFeet = 0.0;
    //    //    int counter2 = 0;

    //    //    double alpha = .15;

    //    //    double previousAltitudeFeet = 0.0;
    //    //    bool isReady = false;

    //    //    SerialPort port = new SerialPort("COM7", 9600, Parity.None, 8, StopBits.One);
    //    //    SerialPortInterface portInterface = new SerialPortInterface(port);
    //    //    portInterface.Open();


    //    //    GraphForm rawTemp = new GraphForm("RawTemp");
    //    //    GraphForm rawPress = new GraphForm("RawPress");
    //    //    GraphForm temp = new GraphForm("Temp");
    //    //    GraphForm press = new GraphForm("Pressure");
    //    //    GraphForm alt = new GraphForm("altitude");

    //    //    rawTemp.Visible = true;
    //    //    rawPress.Visible = true;
    //    //    temp.Visible = true;
    //    //    press.Visible = true;
    //    //    alt.Visible = true;

    //    //    while (true)
    //    //    {
    //    //        int var = 0;
    //    //        while (var != 'S')
    //    //        {
    //    //            var = portInterface.ReadByte();
    //    //        }
    //    //        uint rwTemp = portInterface.ReadUInt(portInterface);
    //    //        rawTemp.AddValueToGraph(rwTemp);

    //    //        uint rwPress = portInterface.ReadUInt(portInterface);
    //    //        rawPress.AddValueToGraph(rwPress);
    //    //        int temperature = portInterface.ReadInt(portInterface);
    //    //        temp.AddValueToGraph(temperature);
    //    //        int pressure = portInterface.ReadInt(portInterface);
    //    //        press.AddValueToGraph(pressure);

    //    //        //Application.DoEvents();

    //    //        //altitude equation from: http://www.barnardmicrosystems.com/L4E_FMU_sensors.htm#Pressure
    //    //        double altitudemeters = (288.15 / (6.5 / 1000.0)) * (1 - (Math.Pow((pressure / 101325.0), (6.5 / 1000.0) * (287.052 / 9.78))));
    //    //        double altitudefeet = altitudemeters * 3.28084;




    //    //        //*******note the barometer appears to need more time to settle
    //    //        if (counter2 == 40)
    //    //        {
    //    //            baseAltitudeFeet = altitudefeet;
    //    //            counter2 = 80;
    //    //        }
    //    //        else if (counter2 == 80)
    //    //        {
    //    //            //do nothing
    //    //        }
    //    //        else
    //    //        {
    //    //            counter2++;
    //    //        }

    //    //        if (isReady)
    //    //        {
    //    //            //Danger *** This algorithm drifts. Sometimes it takes like 10 seconds
    //    //            //or more to stabalize. So you have to wait a long time before you should 
    //    //            //read the altitude and set it as the base altitude because of how long it takes. 
    //    //            //Also, as the barometer heats up, the altitude changes. This isn't good as the barometer's
    //    //            //temperature will probably change throughout the flight
    //    //            double altitudefeettemp = (double)(altitudefeet - baseAltitudeFeet);
    //    //            altitudefeet = alpha * (altitudefeettemp) + (1 - alpha) * previousAltitudeFeet;
    //    //            previousAltitudeFeet = altitudefeet;

    //    //            alt.AddValueToGraph(altitudefeet);
    //    //        }
    //    //        else
    //    //        {
    //    //            altitudefeet = (double)(altitudefeet - baseAltitudeFeet);
    //    //        }


    //    //        if (altitudefeet < 20 && altitudefeet != 0)
    //    //        {
    //    //            isReady = true;
    //    //        }

    //    //    }
    //    //}








    //    //public static void Main()
    //    //{
    //    //    SerialPort port = new SerialPort("COM7", 9600, Parity.None, 8, StopBits.One);
    //    //    SerialPortInterface portInterface = new SerialPortInterface(port);
    //    //    portInterface.Open();


    //    //    GraphForm magX = new GraphForm("MagX");
    //    //    GraphForm magZ = new GraphForm("MagZ");
    //    //    GraphForm magY = new GraphForm("MagY");
    //    //    GraphForm frdmagX = new GraphForm("FRDMagX");
    //    //    GraphForm frdmagZ = new GraphForm("FRDMagZ");
    //    //    GraphForm frdmagY = new GraphForm("FRDMagY");

    //    //    magX.Visible = true;
    //    //    magZ.Visible = true;
    //    //    magY.Visible = true;
    //    //    frdmagX.Visible = true;
    //    //    frdmagZ.Visible = true;
    //    //    frdmagY.Visible = true;

    //    //    while (true)
    //    //    {
    //    //        int var = 0;
    //    //        while (var != 'S')
    //    //        {
    //    //            var = portInterface.ReadByte();
    //    //        }
    //    //        magX.AddValueToGraph(readShort(portInterface));
    //    //        magY.AddValueToGraph(readShort(portInterface));
    //    //        magZ.AddValueToGraph(readShort(portInterface));
    //    //        frdmagX.AddValueToGraph(readShort(portInterface));
    //    //        frdmagY.AddValueToGraph(readShort(portInterface));
    //    //        frdmagZ.AddValueToGraph(readShort(portInterface));
                
    //    //        Application.DoEvents();
    //    //    }
    //    //}


    //    //public static void Main()
    //    //{
    //    //    SerialPort port = new SerialPort("COM7", 9600, Parity.None, 8, StopBits.One);
    //    //    SerialPortInterface portInterface = new SerialPortInterface(port);
    //    //    portInterface.Open();


    //    //    GraphForm magX = new GraphForm("MagX");
    //    //    GraphForm magZ = new GraphForm("MagZ");
    //    //    GraphForm magY = new GraphForm("MagY");

    //    //    magX.Visible = true;
    //    //    magZ.Visible = true;
    //    //    magY.Visible = true;


    //    //    while (true)
    //    //    {
    //    //        int var = 0;
    //    //        while (var != 'S')
    //    //        {
    //    //            var = portInterface.ReadByte();
    //    //        }
    //    //        magX.AddValueToGraph(readShort(portInterface));
    //    //        magZ.AddValueToGraph(readShort(portInterface));
    //    //        magY.AddValueToGraph(readShort(portInterface));

    //    //        Application.DoEvents();
    //    //    }
    //    //}


    //    //public static void Main()
    //    //{
    //    //    SerialPort port = new SerialPort("COM7", 9600, Parity.None, 8, StopBits.One);
    //    //    SerialPortInterface portInterface = new SerialPortInterface(port);
    //    //    portInterface.Open();


    //    //    GraphForm accelRawX = new GraphForm("accelRawX");
    //    //    GraphForm accelRawY = new GraphForm("accelRawY");
    //    //    GraphForm accelRawZ = new GraphForm("accelRawZ");
    //    //    GraphForm gyroRawX = new GraphForm("gyroRawX");
    //    //    GraphForm gyroRawY = new GraphForm("gyroRawY");
    //    //    GraphForm gyroRawZ = new GraphForm("gyroRawZ");
    //    //    GraphForm accelFrdX = new GraphForm("accelFrdX");
    //    //    GraphForm accelFrdY = new GraphForm("accelFrdY");
    //    //    GraphForm accelFrdZ = new GraphForm("accelFrdZ");
    //    //    GraphForm gyroFrdX = new GraphForm("gyroFrdX");
    //    //    GraphForm gyroFrdY = new GraphForm("gyroFrdY");
    //    //    GraphForm gyroFrdZ = new GraphForm("gyroFrdZ");

    //    //    accelRawX.Visible = true;
    //    //    accelRawY.Visible = true;
    //    //    accelRawZ.Visible = true;
    //    //    gyroRawX.Visible = true;
    //    //    gyroRawY.Visible = true;
    //    //    gyroRawZ.Visible = true;
    //    //    accelFrdX.Visible = true;
    //    //    accelFrdY.Visible = true;
    //    //    accelFrdZ.Visible = true;
    //    //    gyroFrdX.Visible = true;
    //    //    gyroFrdY.Visible = true;
    //    //    gyroFrdZ.Visible = true;


    //    //    while (true)
    //    //    {
    //    //        int var = 0;
    //    //        while (var != 'S')
    //    //        {
    //    //            var = portInterface.ReadByte();
    //    //        }
    //    //        accelRawX.AddValueToGraph(readShort(portInterface));
    //    //        accelRawY.AddValueToGraph(readShort(portInterface));
    //    //        accelRawZ.AddValueToGraph(readShort(portInterface));
    //    //        gyroRawX.AddValueToGraph(readShort(portInterface));
    //    //        gyroRawY.AddValueToGraph(readShort(portInterface));
    //    //        gyroRawZ.AddValueToGraph(readShort(portInterface));

    //    //        accelFrdX.AddValueToGraph(readFloat(portInterface));
    //    //        accelFrdY.AddValueToGraph(readFloat(portInterface));
    //    //        accelFrdZ.AddValueToGraph(readFloat(portInterface));
    //    //        gyroFrdX.AddValueToGraph(readFloat(portInterface));
    //    //        gyroFrdY.AddValueToGraph(readFloat(portInterface));

    //    //        float x = readFloat(portInterface);
    //    //        gyroFrdZ.AddValueToGraph(x);

    //    //        Application.DoEvents();
    //    //    }
    //    //}
    }
}
