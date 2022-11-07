using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Calmar
{
    public class Input
    { 
        public static bool IsKeyPressed(KeyCode keyCode)
        {
            return InternalCalls.IsKeyPressed(keyCode);
        }
    }

}
