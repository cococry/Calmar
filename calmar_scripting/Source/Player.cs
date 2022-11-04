using Calmar;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Game
{
    public class Player : Entity
    {
        void Init()
        {
            Console.WriteLine("Initialized Player!"); 
        }

        void Update()
        {
            Console.WriteLine($"Updated player: {Time.DeltaTime()}");
        }
    }
}
