// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class GGAbilityInputID : uint8
{
	// 0 None
	None			UMETA(DisplayName = "None"),
	// 1 Sprint
	Sprint			UMETA(DisplayName = "Sprint"),
	// 2 Jump
	Jump			UMETA(DisplayName = "Jump"),
	// 3 Confirm
	Confirm			UMETA(DisplayName = "Confirm"),
	// 4 Cancel
	Cancel			UMETA(DisplayName = "Cancel"),
	// 5 Ability1
	Ability1		UMETA(DisplayName = "Ability1")
};


//// 3 LMB
//Ability1		UMETA(DisplayName = "Ability1"),
//// 4 RMB
//Ability2		UMETA(DisplayName = "Ability2"),
//// 5 Q
//Ability3		UMETA(DisplayName = "Ability3"),
//// 6 E
//Ability4		UMETA(DisplayName = "Ability4"),
//// 7 R
//Ability5		UMETA(DisplayName = "Ability5"),