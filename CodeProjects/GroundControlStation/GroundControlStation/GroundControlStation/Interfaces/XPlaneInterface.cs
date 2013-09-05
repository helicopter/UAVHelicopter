﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms.VisualStyles;

namespace GroundControlStation.Interfaces
{
    public class XPlaneInterface : IDisposable
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
        public XPlaneInterface(int listenerPort, int transmissionPort, IPAddress transmissionAddress)
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
        public void Open()
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
        public XPlaneData Receive()
        {
            byte[] xplaneBytes = broadcastUdpListener.Receive(ref broadcastUdpListenerEndpoint);

            return XPlaneData.Create(xplaneBytes);
        }

        /// <summary>
        /// Closes the UDP connection listening for XPlane messages.
        /// </summary>
        public void Close()
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