using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GroundControlStation.Model
{
    public class GroundControlStationModel
    {
        public FlightControllerTelemetryData FcTelmData { get; set; }

        public SimulatorTelemetryData SimTelmData { get; set; }

        public GroundControlStationModel()
        {
            FcTelmData = new FlightControllerTelemetryData();
            SimTelmData = new SimulatorTelemetryData();
        }
    }
}
