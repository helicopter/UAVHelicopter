using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using GroundControlStation.Controller;
using GroundControlStation.Model;

namespace GroundControlStation.Views
{
    public interface IDashboardView
    {

        GroundControlStationController Controller { get; set; }

        void UpdateLatestValues(List<Tuple<String, String>> items);
    }
}
