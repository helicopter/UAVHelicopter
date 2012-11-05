using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Net.Sockets;
using System.Net;

namespace Helicopter1
{
    class Receiver
    {
        

        internal void Start()
        {

            bool done = false;

            UdpClient listener = new UdpClient(Util.LISTENERPORT);
            IPEndPoint groupEP = new IPEndPoint(IPAddress.Any, Util.LISTENERPORT);

            Socket s = new Socket(AddressFamily.InterNetwork, SocketType.Dgram, ProtocolType.Udp);
            IPAddress broadcast = IPAddress.Parse("192.168.2.255");


            try
            {
                while (!done)
                {
                    List<float> floats = new List<float>();

                    Console.WriteLine("Waiting for broadcast");
                    byte[] bytes = listener.Receive(ref groupEP);

                    for (int i = 5; i < bytes.Length; i = i + 4)
                    {
                        floats.Add(System.BitConverter.ToSingle(bytes.Skip(i).Take(4).ToArray(), 0));
                    }
                    foreach (float f in floats)
                    {
                        Console.Write(f.ToString() + ",");
                    }
                    Console.WriteLine(".");


                    if (!float.IsNaN(floats[1]))
                    {
                        if (System.BitConverter.ToInt32(bytes.Skip(5).Take(4).ToArray(),0) == 8)
                        {
                            byte[] negativeonebytes = System.BitConverter.GetBytes(((float).2));
                            Array.Copy(negativeonebytes, 0, bytes, 9, negativeonebytes.Length);

                            IPEndPoint ep = new IPEndPoint(broadcast, 49000);
                            //s.SendTo(bytes, ep);
                        }

                        if (System.BitConverter.ToInt32(bytes.Skip(41).Take(4).ToArray(), 0) == 39.0)
                        {
                            byte[] negativeonebytes = System.BitConverter.GetBytes(((float)10.0));
                            Array.Copy(negativeonebytes, 0, bytes, 45, negativeonebytes.Length);

                            IPEndPoint ep = new IPEndPoint(broadcast, 49000);
                            //s.SendTo(bytes, ep);
                        }

                    }


                    List<float> floats2 = new List<float>();
                    for (int i = 5; i < bytes.Length; i = i + 4)
                    {
                        floats2.Add(System.BitConverter.ToSingle(bytes.Skip(i).Take(4).ToArray(), 0));
                    }
                    foreach (float f in floats2)
                    {
                        Console.Write(f.ToString() + ",");
                    }
                    Console.WriteLine(".");
                    //Console.WriteLine("Received broadcast from {0} :\n {1}\n", groupEP.ToString(), Encoding.ASCII.GetString(bytes, 0, bytes.Length));
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
