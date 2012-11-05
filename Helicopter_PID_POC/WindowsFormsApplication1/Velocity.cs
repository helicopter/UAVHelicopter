using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Helicopter1
{
    public class Velocity
    {


        double xVelocityNEDFrame = 0.0;

        public double XVelocityNEDFrame
        {
            get { return xVelocityNEDFrame; }
            set { xVelocityNEDFrame = value; }
        }

        double yVelocityNEDFrame = 0.0;

        public double YVelocityNEDFrame
        {
            get { return yVelocityNEDFrame; }
            set { yVelocityNEDFrame = value; }
        }


        double zVelocityNEDFrame = 0.0;

        public double ZVelocityNEDFrame
        {
            get { return zVelocityNEDFrame; }
            set { zVelocityNEDFrame = value; }
        }



        internal double[,] GetVelocityMatrix()
        {
            return new double[,] { { xVelocityNEDFrame }, { yVelocityNEDFrame }, { zVelocityNEDFrame } };
        }



        double rollVelocityDeg = 0.0;

        public double RollVelocityDeg
        {
            get { return rollVelocityDeg; }
            set { rollVelocityDeg = value; }
        }

        double pitchVelocityDeg = 0.0;

        public double PitchVelocityDeg
        {
            get { return pitchVelocityDeg; }
            set { pitchVelocityDeg = value; }
        }



        double xVelocityBodyFrame = 0.0;

        public double XVelocityBodyFrame
        {
            get { return xVelocityBodyFrame; }
            set { xVelocityBodyFrame = value; }
        }

        double yVelocityBodyFrame = 0.0;

        public double YVelocityBodyFrame
        {
            get { return yVelocityBodyFrame; }
            set { yVelocityBodyFrame = value; }
        }

        double zVelocityBodyFrame = 0.0;

        public double ZVelocityBodyFrame
        {
            get { return zVelocityBodyFrame; }
            set { zVelocityBodyFrame = value; }
        }

        double yawVelocity = 0.0;

        public double YawVelocity
        {
            get { return yawVelocity; }
            set { yawVelocity = value; }
        }


        internal Velocity Copy()
        {
            Velocity v = new Velocity();
            v.xVelocityBodyFrame = xVelocityBodyFrame;
            v.yVelocityBodyFrame = yVelocityBodyFrame;
            v.zVelocityBodyFrame = zVelocityBodyFrame;
            v.yawVelocity = yawVelocity;
            v.PitchVelocityDeg = pitchVelocityDeg;
            v.RollVelocityDeg = rollVelocityDeg;
            v.xVelocityNEDFrame = xVelocityNEDFrame;
            v.yVelocityNEDFrame = yVelocityNEDFrame;
            v.zVelocityNEDFrame = zVelocityNEDFrame;

            return v;
        }
    }
}
