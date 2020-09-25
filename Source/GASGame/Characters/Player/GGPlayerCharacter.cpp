// Fill out your copyright notice in the Description page of Project Settings.


#include "GGPlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

AGGPlayerCharacter::AGGPlayerCharacter(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	Camera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	Camera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm
}

void AGGPlayerCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//Set up key bindings
	check(PlayerInputComponent);
	//PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	//PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAxis("MoveForward", this, &AGGPlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AGGPlayerCharacter::MoveRight);
}

void AGGPlayerCharacter::MoveForward(float Value)
{
	const auto controller = GetController();

	if ((controller != NULL) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void AGGPlayerCharacter::MoveRight(float Value)
{
	const auto controller = GetController();

	if ((controller != NULL) && (Value != 0.0f))
	{
		// find out which way is right
		const FRotator Rotation = controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}