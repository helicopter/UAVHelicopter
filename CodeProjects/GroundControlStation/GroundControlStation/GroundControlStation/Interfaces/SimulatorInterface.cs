using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms.VisualStyles;
using GroundControlStation.Model;

namespace GroundControlStation.Interfaces
{
    public class SimulatorInterface : IDisposable
    {

        private int listenerPort;

        private int transmissionPort;

        private IPAddress transmissionAddress;

        private UdpClient broadcastUdpListener = null;

        private IPEndPoint broadcastUdpListenerEndpoint = null;

        private Socket broadcastUdpTransmitterSocket = null;

        private IPEndPoint broadcastUdpTransmitterEndpoint;

        /// <summary>
        /// Creates an interface to the xplane application
        /// </summary>
        /// <param name="listenerPort">The port on which to listen for xplane's UDP broadcast transmissions</param>
        /// <param name="transmissionPort">The port on which to send data to xplane via UDP broadcast transmissions</param>
        /// <param name="transmissionAddress">The local network's broadcast IP address to send UDP broadcasts to xplane</param>
        public SimulatorInterface(int listenerPort, int transmissionPort, IPAddress transmissionAddress)
        {
            this.listenerPort = listenerPort;
            this.transmissionPort = transmissionPort;
            this.transmissionAddress = transmissionAddress;
        }

        /// <summary>
        /// Sets up a listener to listen to broadcast UDP data on the local network
        /// on the specified port given in the constructure, as well as sets up
        /// a socket to transmit broadcast udp data on the local network at the
        /// port given in the constructor. 
        /// </summary>
        public virtual void Open()
        {
            //Setup a listener and endpoint listen for UDP data sent to the broadcast
            //local network ip address on the specified port.
            broadcastUdpListener = new UdpClient(listenerPort);
            broadcastUdpListenerEndpoint = new IPEndPoint(IPAddress.Any, listenerPort);

            //setup a socket to send UDP data to the broadcast IP address and port
            broadcastUdpTransmitterSocket = new Socket(AddressFamily.InterNetwork, SocketType.Dgram, ProtocolType.Udp);
            broadcastUdpTransmitterEndpoint = new IPEndPoint(transmissionAddress, transmissionPort);
        }

        /// <summary>
        /// Polls the listening client for data from XPlane.
        /// This is a synchronous blocking call so it will wait for data to arrive.
        /// </summary>
        /// <returns>
        /// null if there was an error parsing the received data, otherwise a
        /// XPlaneData object containing the received data.
        /// </returns>
        public virtual SimulatorTelemetry Receive()
        {
            byte[] xplaneBytes = broadcastUdpListener.Receive(ref broadcastUdpListenerEndpoint);

            return SimulatorTelemetry.Create(xplaneBytes);
        }


        public virtual void Transmit(FlightComputerTelemetry fcTelem)
        {
            int index = 0;

            /**
             * Structure for storing the data in byte format for sending to xplane.
             * See http://www.nuclearprojects.com/xplane/xplaneref.html for structure format.
             * Each message to send to xplane consists of a header
             * and multiple data input structures. Each data input structure consists of
             * a four byte integer index value, which matches the index of the data
             * to be set in the data input & output screen in xplain, and an
             * array of 8 four byte floating point values (i.e. float[8]).
             */
            byte[] xplaneBytes = new byte[41];

            String inputMessageHeader = "DATA";

            int PropellerPitchMsgIndex = 39;

            //This value tells the simulator to ignore the value in this field.
            float ignoreValue = -999;
            

            //header
            populateMessage(xplaneBytes, inputMessageHeader, ref index, 4);
            xplaneBytes[4] = 0; //This field is set to 0 for pure input messages
            index++;

            //Indicates this is a propeller pitch (tail and main rotors) message
            populateMessage(xplaneBytes, PropellerPitchMsgIndex, ref index, 4);


            //Set the Main rotor collective
            populateMessage(xplaneBytes, ignoreValue, ref index, 4);
            

            //Set the tail rotor collective
            //TODO fill in
            populateMessage(xplaneBytes, 2.2f, ref index, 4);

            broadcastUdpTransmitterSocket.SendTo(xplaneBytes, broadcastUdpTransmitterEndpoint);
        }

        private void populateMessage(byte[] xplaneBytes, float data, ref int index, int length)
        {
            Array.Copy(BitConverter.GetBytes(data), 0, xplaneBytes, index, length);

            index += length;
        }

        private void populateMessage(byte[] xplaneBytes, int data, ref int index, int length)
        {
            Array.Copy(BitConverter.GetBytes(data), 0, xplaneBytes, index, length);

            index += length;
        }

        private void populateMessage(byte[] xplaneBytes, string data, ref int index, int length)
        {
            Array.Copy(System.Text.Encoding.ASCII.GetBytes(data), 0, xplaneBytes, index, length);

            index += length;
        }

        /// <summary>
        /// Closes the UDP connection listening for XPlane messages.
        /// </summary>
        public virtual void Close()
        {
            if (broadcastUdpListener != null)
            {
                broadcastUdpListener.Close();
            }
        }

        /// <summary>
        /// Calls the close method to close the listener.
        /// </summary>
        public void Dispose()
        {
            Close();
        }
    }
}
