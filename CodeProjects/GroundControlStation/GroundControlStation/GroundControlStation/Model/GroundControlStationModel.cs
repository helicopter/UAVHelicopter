using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GroundControlStation.Model
{
    public class GroundControlStationModel
    {
        public FlightComputerTelemetry FcTelm { get; set; }

        public SimulatorTelemetry SimTelm { get; set; }

        public GroundControlStationModel()
        {
            FcTelm = new FlightComputerTelemetry();
            SimTelm = new SimulatorTelemetry();
        }
    }
}
