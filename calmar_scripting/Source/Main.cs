using System;
using System.Runtime.CompilerServices;

namespace Calmar
{ 
    public struct Vector3
    {
        public float x, y, z;

        public static Vector3 Zero => new Vector3(0.0f);

        public Vector3(float scaler)
        {
            x = scaler;
            y = scaler;
            z = scaler;
        }
        public Vector3(float _x, float _y, float _z)
        {
            x = _x;
            y = _y;
            z = _z;
        }

        public static Vector3 operator*(Vector3 vec, float scaler)
        {
            return new Vector3(vec.y * scaler, vec.x * scaler, vec.z * scaler);
        }
        public static Vector3 operator*(Vector3 vec1, Vector3 vec2)
        {
            return new Vector3(vec1.x * vec2.x, vec1.y * vec2.y, vec1.z * vec2.z);
        }
        public static Vector3 operator+(Vector3 vec1, Vector3 vec2)
        {
            return new Vector3(vec1.x + vec2.x, vec1.y + vec2.y, vec1.z + vec2.z);
        }
    }
    public static class InternalCalls
    {
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static float GetDeltaTime();

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static float SetEntityPosition(uint entity, ref Vector3 pos);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static float GetEntityPosition(uint entity, out Vector3 pos);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static bool IsKeyPressed(KeyCode keyCode);
    }
    public class Entity
    {
        protected Entity()
        {
            ID = 0;
        }
        internal Entity(uint id)
        {
            ID = id;
        }

        public readonly uint ID;

        public Vector3 Position
        {
            get
            {
                InternalCalls.GetEntityPosition(ID, out Vector3 position);
                return position;
            }
            set
            {
                InternalCalls.SetEntityPosition(ID, ref value);
            }
        }
       
    }
  
   
}
