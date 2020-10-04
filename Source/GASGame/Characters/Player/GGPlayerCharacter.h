// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../GGCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/SphereComponent.h"
#include "../../Projectiles/GGProjectile.h"
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
		USpringArmComponent * CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Camera, meta = (AllowPrivateAccess = "true"))
		UCameraComponent * Camera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera, meta = (AllowPrivateAccess = "true"))
		float MaxPitch;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera, meta = (AllowPrivateAccess = "true"))
		float MinPitch;

public:
	AGGPlayerCharacter(const FObjectInitializer& ObjectInitializer);

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Only called on the Server. Calls before Server's AcknowledgePossession.
	virtual void PossessedBy(AController* NewController) override;

	virtual void BeginPlay();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		USphereComponent * KunaiSpot;

	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class AGGProjectile> ProjectileType;

	AGGProjectile * Projectile;

protected:
	// Client only
	virtual void OnRep_PlayerState() override;

private:
	void MoveForward(float Value);
	void MoveRight(float Value);
	void LookUp(float Value);
	void Turn(float Value);
};
