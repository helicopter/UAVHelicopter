using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Helicopter1
{
    class Orientation
    {
        double phi_rollDegrees = 0.0;

        public double Phi_rollDegrees
        {
            get { return phi_rollDegrees; }
            set { phi_rollDegrees = value; }
        }
        double theta_pitchDegrees = 0.0;

        public double Theta_pitchDegrees
        {
            get { return theta_pitchDegrees; }
            set { theta_pitchDegrees = value; }
        }
        double sai_yawDegrees = 0.0;

        public double Sai_yawDegrees
        {
            get { return sai_yawDegrees; }
            set { sai_yawDegrees = value; }
        }



        

        //i need to take the transpose of this matrix.


        internal Orientation Copy()
        {
            Orientation o = new Orientation();
            o.phi_rollDegrees = phi_rollDegrees;
            o.sai_yawDegrees = sai_yawDegrees;
            o.theta_pitchDegrees = theta_pitchDegrees;

            return o;
        }
    }
}
