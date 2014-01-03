using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms.VisualStyles;
using GroundControlStation.Model;
using GroundControlStation.Messages;

namespace GroundControlStation.Interfaces
{
    public class SimulatorInterface : IDisposable
    {
        //Message header for sending data to xplane.
        static String INPUT_MESSAGE_HEADER = "DATA";

        //Identifier for indicating that the message data is for controlling the propellers collective.
        static int PROPELLER_COLLECTIVE_MSG_INDEX = 39;

        static int PROPELLER_CYCLIC_MSG_INDEX = 8;

        static int COLLECTIVE_MSG_PADDING = 24;

        static int CYCLIC_MSG_PADDING = 24;

        static int NUM_OF_MESSAGE_SEGMENTS = 2;

        //header (the word DATA is 4 chars thus 4 bytes) + 1 byte for indicating input vs. output message + 
        //(4 byte for the segment ID * num of segments in message) + 
        //(8*4 since each message segment consists of 8, 4 byte segments of data) * number of message segments
        static int TOTAL_MESSAGE_SIZE_BYTES = 4 + 1 + (4 * NUM_OF_MESSAGE_SEGMENTS) + (8 * 4 * NUM_OF_MESSAGE_SEGMENTS);

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


        public virtual void Transmit(GroundControlStationModel model)
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
             * The ID of the various messages can be found in xplane if you
             * click 'Settings->Data Input & output'. The values on the far left side represents
             * the message ID for that message. I.e. joystick ail/elv/rud for controlling
             * the cyclic has an ID of 8.
             */
            byte[] xplaneBytes = new byte[TOTAL_MESSAGE_SIZE_BYTES];

            //header
            index = populateMessageHeader(index, xplaneBytes);

            index = populatePropellerCollectiveMessage(model, index, xplaneBytes);

            index = populatePropellerCyclicMessage(model, index, xplaneBytes);

            broadcastUdpTransmitterSocket.SendTo(xplaneBytes, broadcastUdpTransmitterEndpoint);
        }

        private int populateMessageHeader(int index, byte[] xplaneBytes)
        {
            populateMessage(xplaneBytes, INPUT_MESSAGE_HEADER, ref index, 4);
            xplaneBytes[index++] = 0; //This field is set to 0 for pure input-into-xplane messages
            return index;
        }

        /// <summary>
        /// Populates the array of bytes with the data for controlling the main rotor's collective pitch (for controlling altitude)
        /// and tail rotor's collective pitch (for controlling yaw)
        /// </summary>
        /// <param name="model"></param>
        /// <param name="index"></param>
        /// <param name="xplaneBytes"></param>
        /// <returns></returns>
        private int populatePropellerCollectiveMessage(GroundControlStationModel model, int index, byte[] xplaneBytes)
        {
            //Indicates this is a propeller pitch (tail and main rotors) message
            populateMessage(xplaneBytes, PROPELLER_COLLECTIVE_MSG_INDEX, ref index, 4);


            //Set the Main rotor collective
            populateMessage(xplaneBytes, model.MainRotorCollectiveControl, ref index, 4);

            //Set the tail rotor collective
            populateMessage(xplaneBytes, model.YawControl, ref index, 4);

            //Set the index to the end of the collective message section
            return index + COLLECTIVE_MSG_PADDING;
        }

        /// <summary>
        /// Populates the array of bytes with the data for controlling the main rotors cycling (for controlling roll and pitch of the aircraft)
        /// </summary>
        /// <param name="model"></param>
        /// <param name="index"></param>
        /// <param name="xplaneBytes"></param>
        /// <returns></returns>
        private int populatePropellerCyclicMessage(GroundControlStationModel model, int index, byte[] xplaneBytes)
        {
            //Indicates this is a propeller pitch (tail and main rotors) message
            populateMessage(xplaneBytes, PROPELLER_CYCLIC_MSG_INDEX, ref index, 4);


            //Set the cyclic pitch (elevator)
            populateMessage(xplaneBytes, model.LongitudeControl, ref index, 4);

            //Set the cyclic roll (aileron)
            populateMessage(xplaneBytes, model.LateralControl, ref index, 4);

            //Set the index to the end of the cyclic message section
            return index + CYCLIC_MSG_PADDING;
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
