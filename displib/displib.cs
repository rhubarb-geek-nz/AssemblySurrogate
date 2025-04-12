/***********************************
 * Copyright (c) 2025 Roger Brown.
 * Licensed under the MIT License.
 ****/

using System;
using System.Runtime.InteropServices;

namespace RhubarbGeekNz.AssemblySurrogate
{
    [Guid("de03e384-1a1b-43cc-ae72-9865d01886dc")]
    public class CHelloWorld : IHelloWorld
    {
        public string GetMessage(int Hint)
        {
            return "Hello World";
        }
    }
}
