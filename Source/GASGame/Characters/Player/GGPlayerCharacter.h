// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../GGCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GGPlayerCharacter.generated.h"

/**
 * 
 */
UCLASS()
class GASGAME_API AGGPlayerCharacter : public AGGCharacter
{
	GENERATED_BODY()
	
	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent * CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent * Camera;

public:
	AGGPlayerCharacter(const FObjectInitializer& ObjectInitializer);

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	void MoveForward(float Value);
	void MoveRight(float Value);
};
