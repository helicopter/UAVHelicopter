﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using GroundControlStation.Interfaces;
using GroundControlStation.Model;

namespace UnitTests.mocks
{
    public class MockSimulatorInterface : SimulatorInterface
    {
        private int testValue;

        public MockSimulatorInterface(int testValue):  base(0,0,null)
        {
            this.testValue = testValue;
        }

        public override void Open()
        {
            
        }

        
        public override SimulatorTelemetry Receive()
        {
            SimulatorTelemetry data = new SimulatorTelemetry();
            data.MagHeadingDegrees = testValue;

            return data;
        }

        public override void Close()
        {
            
        }
    }
}
