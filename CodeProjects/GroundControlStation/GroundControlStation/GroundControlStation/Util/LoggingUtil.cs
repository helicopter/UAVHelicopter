using System;
using System.Collections.Generic;
using System.Linq;
using System.Reflection;
using System.Text;
using System.Threading.Tasks;

namespace GroundControlStation.Util
{
    public class LoggingUtil
    {
        public static string ToCsvHeader<T>(string separator, T obj)
        {
            Type t = typeof(T);
            FieldInfo[] fields = t.GetFields();

            string str = "";

            if (fields.Length == 0)
            {
                PropertyInfo[] flds = t.GetProperties();

                str = String.Join(separator, flds.Select(f => f.Name).ToArray().Except(new string[] { "RawData" }).ToArray());
            
            }
            else
            {
                str = String.Join(separator, fields.Select(f => f.Name).ToArray());
            }

            return str;
        }

        //from http://stackoverflow.com/questions/1179816/best-practices-for-serializing-objects-to-a-custom-string-format-for-use-in-an-o
        public static string ToCsv<T>(string separator, T obj)
        {
            Type t = typeof(T);
            FieldInfo[] fields = t.GetFields();

            StringBuilder csvdata = new StringBuilder();

            string str = "";

            if (fields.Length == 0)
            {
                PropertyInfo[] flds = t.GetProperties();

                str = ToCsvFields(separator, flds, obj);
            }
            else
            {
                str = ToCsvFields(separator, fields, obj);
            }

            csvdata.Append(str);

            return csvdata.ToString();
        }

        public static string ToCsvFields(string separator, PropertyInfo[] fields, object o)
        {
            StringBuilder linie = new StringBuilder();

            foreach (var f in fields)
            {
                if (linie.Length > 0)
                    linie.Append(separator);

                //Ignore the RawData field.
                if (!f.Name.Contains("RawData"))
                {
                    var x = f.GetValue(o);

                    if (x != null)
                        linie.Append(x.ToString());
                }
            }

            return linie.ToString();
        }

        public static string ToCsvFields(string separator, FieldInfo[] fields, object o)
        {
            StringBuilder linie = new StringBuilder();

            foreach (var f in fields)
            {
                if (linie.Length > 0)
                    linie.Append(separator);

                var x = f.GetValue(o);

                if (x != null)
                {
                    linie.Append(x.ToString());
                }
            }

            return linie.ToString();
        }
    }
}
