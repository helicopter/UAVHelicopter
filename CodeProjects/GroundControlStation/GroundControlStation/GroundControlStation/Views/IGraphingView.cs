using System;
using System.Collections.Generic;
using System.Linq;
using System.Security.Cryptography.X509Certificates;
using System.Text;
using System.Threading.Tasks;

namespace GroundControlStation.Views
{
    /// <summary>
    /// This interface represents views which display graphing information.
    /// </summary>
    public interface IGraphingView
    {
        bool IsActive
        {
            get;
        }

        void ActivateView();

        void InactivateView();

        void AddValueToGraph(double value);
    }
}
