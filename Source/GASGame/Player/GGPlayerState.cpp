// Fill out your copyright notice in the Description page of Project Settings.


#include "GGPlayerState.h"
#include "../Characters/Abilities/GGAbilitySystemComponent.h"

AGGPlayerState::AGGPlayerState()
{
	// Create ability system component, and set it to be explicitly replicated
	AbilitySystemComponent = CreateDefaultSubobject<UGGAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
}

UAbilitySystemComponent * AGGPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}