
#pragma once

#include "CoreMinimal.h"
#include "Subsystems/EngineSubsystem.h"
#include "GGEngineSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class GASGAME_API UGGEngineSubsystem : public UEngineSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;	
};
