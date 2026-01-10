// Copyright dual node interactive. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Interfaces/OnlineSessionInterface.h" // Required for EOnJoinSessionCompleteResult::Type
#include "OnlineSessionSettings.h" // Required for FOnlineSessionSearchResult

// Delegates for session events
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCreateSessionComplete, bool, bWasSuccessful);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnFindSessionsComplete, const TArray<FOnlineSessionSearchResult>&, SessionResults, bool, bWasSuccessful);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnJoinSessionComplete, EOnJoinSessionCompleteResult::Type, Result);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDestroySessionComplete, bool, bWasSuccessful);
