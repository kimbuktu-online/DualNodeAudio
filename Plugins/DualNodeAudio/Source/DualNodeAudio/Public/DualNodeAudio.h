#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class FDualNodeAudioModule : public IModuleInterface
{
public:
	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

#if WITH_EDITOR
	// Diese Funktion wird nach dem Engine-Start aufgerufen
	void CheckAndGenerateAssets();
#endif
};