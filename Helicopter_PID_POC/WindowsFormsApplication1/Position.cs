using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Helicopter1
{
    class Position
    {
        double xLatitudePosition = 0.0;

        public double XLatitudePosition
        {
            get { return xLatitudePosition; }
            set { xLatitudePosition = value; }
        }


        double yLongitudePosition = 0.0;

        public double YLongitudePosition
        {
            get { return yLongitudePosition; }
            set { yLongitudePosition = value; }
        }

        double zAltitudePosition = 0.0;

        public double ZAltitudePosition
        {
            get { return zAltitudePosition; }
            set { zAltitudePosition = value; }
        }

        internal Position SubtractPosition(Position currentPosition)
        {
            Position newPosition = new Position();
/*
            newPosition.xLatitudePosition = xLatitudePosition - currentPosition.xLatitudePosition;
            newPosition.yLongitudePosition = yLongitudePosition - currentPosition.yLongitudePosition;
            newPosition.zAltitudePosition = zAltitudePosition - currentPosition.zAltitudePosition;
 */

            newPosition.xLatitudePosition = currentPosition.xLatitudePosition - xLatitudePosition;
            newPosition.yLongitudePosition = currentPosition.yLongitudePosition - yLongitudePosition;
            newPosition.zAltitudePosition = currentPosition.zAltitudePosition - zAltitudePosition;

            return newPosition;
        }


        internal double[,] GetPositionMatrix()
        {
            return new double[,] { { xLatitudePosition }, { yLongitudePosition }, { zAltitudePosition } };
        }

        internal Position Copy()
        {
            Position p = new Position();
            p.xLatitudePosition = xLatitudePosition;
            p.yLongitudePosition = yLongitudePosition;
            p.zAltitudePosition = zAltitudePosition;
            return p;
        }
    }
}
