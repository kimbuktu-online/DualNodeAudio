#include "DualNodeItemInstance.h"
#include "Engine/AssetManager.h"

bool FDualNodeItemInstance::ResolveDefinition()
{
	if (CachedDefinition) return true;

	if (ItemId.IsValid())
	{
		CachedDefinition = Cast<UDualNodeItemDefinition>(UAssetManager::Get().GetPrimaryAssetObject(ItemId));
		return CachedDefinition != nullptr;
	}
	return false;
}