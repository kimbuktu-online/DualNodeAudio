#include "DualNodeItemDefinition.h"

const UDualNodeItemFragment* UDualNodeItemDefinition::FindFragmentByClass(TSubclassOf<UDualNodeItemFragment> FragmentClass) const
{
	if (!FragmentClass) return nullptr;

	for (const TObjectPtr<UDualNodeItemFragment>& Fragment : Fragments)
	{
		if (Fragment && Fragment->IsA(FragmentClass))
		{
			return Fragment;
		}
	}
	return nullptr;
}