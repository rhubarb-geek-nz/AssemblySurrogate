/***********************************
 * Copyright (c) 2025 Roger Brown.
 * Licensed under the MIT License.
 ****/

#include <objbase.h>
#include <stdio.h>
#include <atlbase.h>
#import "RhubarbGeekNz.AssemblySurrogate.tlb"

int main(int argc, char** argv)
{
	HRESULT hr = CoInitializeEx(NULL, COINIT_MULTITHREADED);

	if (SUCCEEDED(hr))
	{
		DWORD ctx[] = { CLSCTX_LOCAL_SERVER, CLSCTX_INPROC_SERVER, CLSCTX_ALL };
		int i = 0;

		while (i < sizeof(ctx) / sizeof(ctx[0]))
		{
			CComPtr<RhubarbGeekNzAssemblySurrogate::IHelloWorld> helloWorld;

			hr = helloWorld.CoCreateInstance(__uuidof(RhubarbGeekNzAssemblySurrogate::CHelloWorld), NULL, ctx[i]);

			if (SUCCEEDED(hr))
			{
				CComBSTR bstr;

				hr = helloWorld->raw_GetMessage(1, &bstr);

				if (SUCCEEDED(hr))
				{
					printf("%08lx - %S\n", ctx[i], (BSTR)bstr);
				}
			}

			if (FAILED(hr))
			{
				fprintf(stderr, "%08lx - 0x%lx\n", ctx[i], (long)hr);
			}

			i++;
		}

		CoUninitialize();
	}

	return FAILED(hr);
}
