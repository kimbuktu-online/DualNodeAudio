// Copyright dual node interactive. All Rights Reserved.

#include "DualNodeDevConnectSettings.h"

UDualNodeDevConnectSettings::UDualNodeDevConnectSettings()
{
	// You can add a default developer Steam ID for testing purposes here
	// DeveloperSteamIDs.Add(TEXT("7656119..."));
}

const UDualNodeDevConnectSettings* UDualNodeDevConnectSettings::Get()
{
	return GetDefault<UDualNodeDevConnectSettings>();
}
