// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystemComponent.h"
#include "../GASGame.h"
#include "Abilities/GGAbilitySystemComponent.h"
#include "GGCharacter.generated.h"

UCLASS()
class GASGAME_API AGGCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AGGCharacter(const FObjectInitializer & ObjectInitializer);

	// Implement IAbilitySystemInterface
	virtual UAbilitySystemComponent * GetAbilitySystemComponent() const override;

	// Switch on AbilityID to return individual ability levels. Hardcoded to 1 for every ability in this project.
	UFUNCTION(BlueprintCallable, Category = "GGCharacter")
		virtual int32 GetAbilityLevel(GGAbilityInputID AbilityID) const;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;	

	UGGAbilitySystemComponent * AbilitySystemComponent;

	// Default abilities for this Character. These will be removed on Character death and re given if Character respawns.
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Abilities")
		TArray<TSubclassOf<class UGGGameplayAbility>> CharacterAbilities;

	// Grant abilities on the Server. The Ability Specs will be replicated to the owning client.
	virtual void AddCharacterAbilities();
};
