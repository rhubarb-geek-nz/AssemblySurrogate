# Copyright (c) 2025 Roger Brown.
# Licensed under the MIT License.

IDLNAME=disptlb
LIBNAME=RhubarbGeekNz.AssemblySurrogate
TLBFILE=$(LIBNAME).tlb
DLLFILE=$(LIBNAME).dll
PKGNAME=rhubarb-geek-nz.AssemblySurrogate.1.0.0.nupkg

all: $(PKGNAME)

clean:
	if exist $(TLBFILE) del $(TLBFILE)
	if exist $(DLLFILE) del $(DLLFILE)
	if exist $(PKGNAME) del $(PKGNAME)
	if exist base rmdir /q /s base

$(TLBFILE): $(IDLNAME).idl 
	midl $(IDLNAME).idl /client none /server none /tlb $@ /out .

$(DLLFILE): $(TLBFILE)
	tlbimp $(TLBFILE) /asmversion:1.0.0.0 /namespace:$(LIBNAME) /machine:Agnostic /out:$@ $(TLBIMPFLAGS)
	if not "$(CertificateThumbprint)"=="" signtool sign /a /sha1 $(CertificateThumbprint) /fd SHA256 /t http://timestamp.digicert.com $@

$(PKGNAME): $(DLLFILE) $(IDLNAME).nuspec
	mkdir base\lib\netstandard2.0
	copy $(DLLFILE) base\lib\netstandard2.0
	copy ..\README.md base
	nuget pack $(IDLNAME).nuspec -BasePath base
	rmdir /q /s base
	if not "$(CertificateThumbprint)"=="" nuget sign $@ -CertificateFingerprint $(CertificateThumbprint) -Timestamper http://timestamp.digicert.com
