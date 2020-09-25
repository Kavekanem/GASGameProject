
#include "GGEngineSubsystem.h"
#include "AbilitySystemGlobals.h"

void UGGEngineSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	//Starting with 4.24, it is now mandatory to call UAbilitySystemGlobals::InitGlobalData() to use TargetData.
	UAbilitySystemGlobals::Get().InitGlobalData();
}
