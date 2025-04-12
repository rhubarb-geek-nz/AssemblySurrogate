# Copyright (c) 2025 Roger Brown.
# Licensed under the MIT License.

param(
	$CertificateThumbprint = '601A8B683F791E51F647D34AD102C38DA4DDB65F',
	$Architectures = @('arm', 'arm64', 'x86', 'x64')
)

$ProgressPreference = 'SilentlyContinue'
$ErrorActionPreference = 'Stop'

trap
{
	throw $PSItem
}

foreach ($ARCH in $Architectures)
{
	$VERSION=(Get-Item -LiteralPath "..\displib\bin\Release\netstandard2.0\publish\RhubarbGeekNz.AssemblySurrogate.Library.dll").VersionInfo.ProductVersion

	$env:PRODUCTVERSION = $VERSION
	$env:PRODUCTARCH = $ARCH
	$env:PRODUCTWIN64 = 'yes'
	$env:PRODUCTPROGFILES = 'ProgramFiles64Folder'
	$env:INSTALLERVERSION = '500'

	switch ($ARCH)
	{
		'arm64' {
			$env:UPGRADECODE = 'D334B978-10B0-4698-B2AD-07B75960BABC'
		}

		'arm' {
			$env:UPGRADECODE = '28467426-FF2C-4EDF-A5D9-5C5E58B12D27'
			$env:PRODUCTWIN64 = 'no'
			$env:PRODUCTPROGFILES = 'ProgramFilesFolder'
		}

		'x86' {
			$env:UPGRADECODE = '52DEC99E-CD50-4734-9C03-8CF5E4DCBE16'
			$env:PRODUCTWIN64 = 'no'
			$env:PRODUCTPROGFILES = 'ProgramFilesFolder'
			$env:INSTALLERVERSION = '200'
		}

		'x64' {
			$env:UPGRADECODE = 'E083647D-30D3-4517-8738-13247679C724'
			$env:INSTALLERVERSION = '200'
		}
	}	

	& "${env:WIX}bin\candle.exe" -nologo "displib.wxs"

	if ($LastExitCode -ne 0)
	{
		exit $LastExitCode
	}

	$MsiFilename = "rhubarb-geek-nz.AssemblySurrogate-$VERSION-$ARCH.msi"

	& "${env:WIX}bin\light.exe" -nologo -cultures:null -out $MsiFilename 'displib.wixobj'

	if ($LastExitCode -ne 0)
	{
		exit $LastExitCode
	}

	Remove-Item 'displib.wix*'
	Remove-Item 'rhubarb-geek-nz.AssemblySurrogate-*.wixpdb'

	if ($CertificateThumbprint)
	{
		$codeSignCertificate = Get-ChildItem -path Cert:\ -Recurse -CodeSigningCert | Where-Object { $_.Thumbprint -eq $CertificateThumbprint }

		if ($codeSignCertificate -and ($codeSignCertificate.Count -eq 1))
		{
			$result = Set-AuthenticodeSignature -FilePath $MsiFilename -HashAlgorithm 'SHA256' -Certificate $codeSignCertificate -TimestampServer 'http://timestamp.digicert.com'
		}
		else
		{
			Write-Error "Error with certificate - $CertificateThumbprint"
		}
	}
}
