using System;
using System.Runtime.CompilerServices;

namespace Calmar
{ 
    public struct Vector3
    {
        public float x, y, z;

        public Vector3(float _x, float _y, float _z)
        {
            x = _x;
            y = _y;
            z = _z;
        }
    }
    public static class InternalCalls
    {

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static float GetDeltaTime();
    }
    public class Entity
    {
        public Entity()
        {
        }
       
    }
    public class Time
    {
        public static float DeltaTime()
        {
            return InternalCalls.GetDeltaTime();
        }
    }

}
