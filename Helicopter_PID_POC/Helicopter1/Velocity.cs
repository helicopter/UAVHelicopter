using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Helicopter1
{
    public class Velocity
    {
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

            return v;
        }
    }
}
