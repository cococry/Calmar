using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Calmar
{
    public class Time
    {
        public static float DeltaTime()
        {
            return InternalCalls.GetDeltaTime();
        }
    }
}
