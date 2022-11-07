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
            Console.WriteLine($"Initialized Player! - {ID}"); 
        }

        void Update()
        {
            float speed = 1.0f;
            Vector3 velocity = Vector3.Zero;

            if (Input.IsKeyPressed(KeyCode.W))
            {
                velocity.y = 1.0f;
                Console.Write("Pressed W");
            }
            else if (Input.IsKeyPressed(KeyCode.S))
                velocity.y = -1.0f;

            if (Input.IsKeyPressed(KeyCode.A))
                velocity.x = -1.0f;
            else if (Input.IsKeyPressed(KeyCode.D))
                velocity.x = 1.0f;

            velocity *= speed;

            Vector3 pos = Position;
            Console.WriteLine($"{pos.x}, {pos.y}, {pos.z}");
            pos += velocity * Time.DeltaTime();
            Position = pos;

        }
    }
}
