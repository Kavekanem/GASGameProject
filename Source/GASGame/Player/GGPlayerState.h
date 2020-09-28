// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "AbilitySystemInterface.h"
#include "GGPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class GASGAME_API AGGPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()
	
public:
	AGGPlayerState();

	// Implement IAbilitySystemInterface
	class UAbilitySystemComponent * GetAbilitySystemComponent() const override;

protected:
	UPROPERTY()
		class UGGAbilitySystemComponent * AbilitySystemComponent;
};
