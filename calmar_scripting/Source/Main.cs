using System;
public class Main
{
    public float FloatVar { get; set; }
    public Main()
    {
        Console.WriteLine("Main Constructur!");
    }
    public void PrintMessage()
    {
        Console.WriteLine("Hello World from C#!");
    }

    public void PrintCustomMessage(string msg)
    {
        Console.WriteLine($"C# says: {msg}");
    }
}
