// Fill out your copyright notice in the Description page of Project Settings.


#include "GGCharacter.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "UObject/UObjectGlobals.h"
#include "GGCharacterMovementComponent.h"
#include "Abilities/GGGameplayAbility.h"
#include "AbilitySystemComponent.h"

// Sets default values
AGGCharacter::AGGCharacter(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer.SetDefaultSubobjectClass<UGGCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	bAlwaysRelevant = true;
}

UAbilitySystemComponent * AGGCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

int32 AGGCharacter::GetAbilityLevel(GGAbilityInputID AbilityID) const
{
	return 1;
}

// Called when the game starts or when spawned
void AGGCharacter::BeginPlay()
{
	Super::BeginPlay();	
}

void AGGCharacter::AddCharacterAbilities()
{
	// Grant abilities, but only on the server	
	if (GetLocalRole() != ROLE_Authority || !IsValid(AbilitySystemComponent) /*|| AbilitySystemComponent->CharacterAbilitiesGiven*/)
	{
		return;
	}

	for (TSubclassOf<UGGGameplayAbility> & StartupAbility : CharacterAbilities)
	{
		AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(StartupAbility, GetAbilityLevel(StartupAbility.GetDefaultObject()->AbilityID), static_cast<int32>(StartupAbility.GetDefaultObject()->AbilityInputID), this));
	}

	AbilitySystemComponent->CharacterAbilitiesGiven = true;
}
