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

            return String.Join(separator, fields.Select(f => f.Name).ToArray());
        }

        //from http://stackoverflow.com/questions/1179816/best-practices-for-serializing-objects-to-a-custom-string-format-for-use-in-an-o
        public static string ToCsv<T>(string separator, T obj)
        {
            Type t = typeof(T);
            FieldInfo[] fields = t.GetFields();

            StringBuilder csvdata = new StringBuilder();

            csvdata.Append(ToCsvFields(separator, fields, obj));

            return csvdata.ToString();
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
                    linie.Append(x.ToString());
            }

            return linie.ToString();
        }
    }
}
