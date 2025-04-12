# rhubarb-geek-nz/AssemblySurrogate

Demonstration of inproc server COM object in C# with DllSurrogate.

IDL is compiled into a type library.

```
midl disptlb.idl /client none /server none /tlb RhubarbGeekNz.AssemblySurrogate.tlb /out .
```

Type library is used to create a .NET assembly.

```
tlbimp RhubarbGeekNz.AssemblySurrogate.tlb /asmversion:1.0.0.0 /namespace:RhubarbGeekNz.AssemblySurrogate /machine:Agnostic /out:RhubarbGeekNz.AssemblySurrogate.dll
```

Assembly is packaged into a nuget package.

```
mkdir base\lib\netstandard2.0
copy RhubarbGeekNz.AssemblySurrogate.dll base\lib\netstandard2.0
nuget pack disptlb.nuspec -BasePath base
```

Package is referenced by server project.

```
<PackageReference Include="rhubarb-geek-nz.AssemblySurrogate" Version="1.0.0" />
```

Implementation class derives from interface from assembly.

```
[Guid("de03e384-1a1b-43cc-ae72-9865d01886dc")]
public class CHelloWorld : IHelloWorld
```

Required registry information can be gathered using [regasm](https://learn.microsoft.com/en-us/dotnet/framework/tools/regasm-exe-assembly-registration-tool).

```
regasm RhubarbGeekNz.AssemblySurrogate.Library.dll /regfile:displib.reg /codebase
```

Test with a [C++ app](dispapp/dispapp.cpp) to validated both `CLSCTX_LOCAL_SERVER` and `CLSCTX_INPROC_SERVER` activation. A 64 bit installation cannot be loaded in a 32 bit process.

```
dispapp\bin\arm64\dispapp.exe
00000004 - Hello World
00000001 - Hello World
00000005 - Hello World
dispapp\bin\x64\dispapp.exe
00000004 - Hello World
00000001 - Hello World
00000005 - Hello World
dispapp\bin\x86\dispapp.exe
00000004 - Hello World
00000001 - 0x80040154
00000005 - Hello World
```
