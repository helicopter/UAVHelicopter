using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Helicopter1
{
    public class Measurement
    {
        double referenceYaw = 0.0;

        public double ReferenceYaw
        {
            get { return referenceYaw; }
            set { referenceYaw = value; }
        }



        Position position = null; //positionInertialFrame

        internal Position Position
        {
            get { return position; }
            set { position = value; }
        }
        Orientation orientation = null;

        internal Orientation Orientation
        {
            get { return orientation; }
            set { orientation = value; }
        }

        Velocity velocity = null;

        internal Velocity Velocity
        {
            get { return velocity; }
            set { velocity = value; }
        }

        public Measurement()
        {
            position = new Position();
            orientation = new Orientation();
            velocity = new Velocity();
        }


        internal Measurement Copy()
        {
            Measurement measurement = new Measurement();
            measurement.position = this.position.Copy();
            measurement.orientation = this.orientation.Copy();
            measurement.velocity = this.velocity.Copy();

            return measurement;
        }

        internal Measurement ConvertFromGeodedicToLocalNED(Helicopter1.Position initialReferenceStartingPositionECEF, Helicopter1.Position initialReferenceStartingPositionGeodedic)
        {
            Measurement newMeasurementECEF = this.Copy();
            double[] ecef = Util.ConvertFromGeodeticToECEF(newMeasurementECEF.Position.YLongitudePosition, newMeasurementECEF.Position.XLatitudePosition, newMeasurementECEF.Position.ZAltitudePosition);
            newMeasurementECEF.Position.XLatitudePosition = ecef[0];
            newMeasurementECEF.Position.YLongitudePosition = ecef[1];
            newMeasurementECEF.Position.ZAltitudePosition = ecef[2];





            //Rotate from earth centered earth fixed to local NED (which is the same as vehicle carried NED)
            double[,] ecefToNEDRotationMatrix = Util.calculateEcefToNEDMatrix(initialReferenceStartingPositionGeodedic);


            Position currentMinusRef = new Position();
            currentMinusRef.XLatitudePosition = newMeasurementECEF.Position.XLatitudePosition - initialReferenceStartingPositionECEF.XLatitudePosition;
            currentMinusRef.YLongitudePosition = newMeasurementECEF.Position.YLongitudePosition - initialReferenceStartingPositionECEF.YLongitudePosition;
            currentMinusRef.ZAltitudePosition = newMeasurementECEF.Position.ZAltitudePosition - initialReferenceStartingPositionECEF.ZAltitudePosition;

            double[,] positionMatrix = currentMinusRef.GetPositionMatrix();
            double[,] rotatedPosition = new double[,] { { 0 }, { 0 }, { 0 } };


            Measurement newMeasurementNED = newMeasurementECEF.Copy();
            Position currentPositionLocalNED = new Position();

            //for each column in rotation matrix through columns
            for (int i = 0; i < ecefToNEDRotationMatrix.GetLength(1); i++)
            {

                for (int j = 0; j < positionMatrix.GetLength(0); j++)
                {
                    rotatedPosition[i, 0] += ecefToNEDRotationMatrix[i, j] * positionMatrix[j, 0];
                }
            }

            currentPositionLocalNED.XLatitudePosition = rotatedPosition[0, 0];
            currentPositionLocalNED.YLongitudePosition = rotatedPosition[1, 0];
            currentPositionLocalNED.ZAltitudePosition = rotatedPosition[2, 0];
            newMeasurementNED.Position = currentPositionLocalNED;

            return newMeasurementNED;
        }


        internal Measurement ConvertFromLocalNEDToBodyFrame()
        {
            Measurement copyNED = this.Copy();
            double[,] NEDToBodyRotationMatrix = Util.calculateNEDToBodyMatrix(this.Orientation);
            double[,] positionMatrix = this.Position.GetPositionMatrix();
            double[,] rotatedPosition = new double[,] { { 0 }, { 0 }, { 0 } };


            Position currentPositionBody = new Position();

            //for each column in rotation matrix through columns
            for (int i = 0; i < NEDToBodyRotationMatrix.GetLength(1); i++)
            {

                for (int j = 0; j < positionMatrix.GetLength(0); j++)
                {
                    rotatedPosition[i, 0] += NEDToBodyRotationMatrix[i, j] * positionMatrix[j, 0];
                }
            }

            currentPositionBody.XLatitudePosition = rotatedPosition[0, 0];
            currentPositionBody.YLongitudePosition = rotatedPosition[1, 0];
            currentPositionBody.ZAltitudePosition = rotatedPosition[2, 0];

            copyNED.Position = currentPositionBody;


            Velocity currentVelocityBody = new Velocity();
            //COnvert velocity
            double[,] velocityMatrix = this.Velocity.GetVelocityMatrix();
            double[,] rotatedVelocity = new double[,] { { 0 }, { 0 }, { 0 } };

            //for each column in rotation matrix through columns
            for (int i = 0; i < NEDToBodyRotationMatrix.GetLength(1); i++)
            {

                for (int j = 0; j < positionMatrix.GetLength(0); j++)
                {
                    rotatedVelocity[i, 0] += NEDToBodyRotationMatrix[i, j] * velocityMatrix[j, 0];
                }
            }

            currentVelocityBody.XVelocityBodyFrame = rotatedVelocity[0, 0];
            currentVelocityBody.YVelocityBodyFrame = rotatedVelocity[1, 0];
            currentVelocityBody.ZVelocityBodyFrame = rotatedVelocity[2, 0];

            copyNED.Velocity = currentVelocityBody;

            return copyNED;
        }
    }
}
