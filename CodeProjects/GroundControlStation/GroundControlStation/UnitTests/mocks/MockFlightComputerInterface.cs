using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using GroundControlStation.Interfaces;
using GroundControlStation.Model;
using GroundControlStation.Messages;

namespace UnitTests.mocks
{
    public class MockFlightComputerInterface : FlightComputerInterface
    {
        private double testValue;

        public MockFlightComputerInterface(double testValue)
            : base(null)
        {
            this.testValue = testValue;
        }

        public override void Open()
        {
            
        }

        public override void Close()
        {
            
        }

        /// <summary>
        /// Receives telemetry data from the helicopter and parses it
        /// into telemetry messages
        /// </summary>
        /// <returns>An object containing the telemetry data</returns>
        /// <exception cref="SystemException">Thrown when there is an issue parsing the received telemetry data</exception>
        public override Message Receive()
        {
            FlightComputerTelemetryMessage data = new FlightComputerTelemetryMessage();
           // data.MagX = (short)testValue;

            return data;
        }

        public override void Transmit(Message telemetry)
        {
            
        }
    }
}
