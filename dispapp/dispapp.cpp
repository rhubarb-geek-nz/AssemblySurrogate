/***********************************
 * Copyright (c) 2025 Roger Brown.
 * Licensed under the MIT License.
 ****/

#include <objbase.h>
#include <stdio.h>

int main(int argc, char** argv)
{
	HRESULT hr = CoInitializeEx(NULL, COINIT_MULTITHREADED);

	if (SUCCEEDED(hr))
	{
		CLSID clsid;
		HRESULT hr = CLSIDFromString(L"{DE03E384-1A1B-43CC-AE72-9865D01886DC}", &clsid);

		if (SUCCEEDED(hr))
		{
			DWORD ctx[] = { CLSCTX_LOCAL_SERVER, CLSCTX_INPROC_SERVER, CLSCTX_LOCAL_SERVER | CLSCTX_INPROC_SERVER };
			int i = 0;

			while (i < sizeof(ctx) / sizeof(ctx[0]))
			{
				IDispatch* dispatch = NULL;

				hr = CoCreateInstance(clsid, NULL, ctx[i], IID_IDispatch, (void**)&dispatch);

				if (SUCCEEDED(hr))
				{
					DISPID dispId;

					if (SUCCEEDED(hr))
					{
						BSTR name = SysAllocString(L"GetMessage");
						hr = dispatch->GetIDsOfNames(IID_NULL, &name, 1, LOCALE_USER_DEFAULT, &dispId);
						SysFreeString(name);

						if (SUCCEEDED(hr))
						{
							DISPPARAMS params = { 0,0,0,0 };
							VARIANT result;
							VariantInit(&result);
							UINT errArg;
							EXCEPINFO ex;
							VARIANT arg;

							arg.vt = VT_I4;
							arg.intVal = 1;

							ZeroMemory(&ex, sizeof(ex));
							params.rgvarg = &arg;
							params.cArgs = 1;

							hr = dispatch->Invoke(dispId, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &params, &result, &ex, &errArg);

							if (SUCCEEDED(hr) && result.vt == VT_BSTR)
							{
								printf("%08lx - %S\n", ctx[i], result.bstrVal);
							}

							VariantClear(&result);
						}
					}

					dispatch->Release();
				}

				if (FAILED(hr))
				{
					fprintf(stderr, "%08lx - 0x%lx\n", ctx[i], (long)hr);
				}

				i++;
			}
		}
		else
		{
			fprintf(stderr, "0x%lx\n", (long)hr);
		}

		CoUninitialize();
	}

	return FAILED(hr);
}
