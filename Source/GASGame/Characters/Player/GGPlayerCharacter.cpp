// Fill out your copyright notice in the Description page of Project Settings.


#include "GGPlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "../../Player/GGPlayerState.h"
#include "../../GASGame.h"
#include "Engine/EngineTypes.h"

AGGPlayerCharacter::AGGPlayerCharacter(const FObjectInitializer& OI) : Super(OI)
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
	CameraBoom = OI.CreateDefaultSubobject<USpringArmComponent>(this, TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	Camera = OI.CreateDefaultSubobject<UCameraComponent>(this, TEXT("FollowCamera"));
	Camera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	Camera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	KunaiSpot = OI.CreateDefaultSubobject<USphereComponent>(this, TEXT("KunaiSpot"));
	KunaiSpot->SetVisibility(false);
	KunaiSpot->SetRelativeLocation(FVector(0.0f, 0.0f, 150.0f));
	KunaiSpot->SetupAttachment(RootComponent);
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

	if (IsValid(AbilitySystemComponent) && IsValid(InputComponent))
	{
		AbilitySystemComponent->BindAbilityActivationToInputComponent(InputComponent, FGameplayAbilityInputBinds(FString("ConfirmTarget"),
			FString("CancelTarget"), FString("GGAbilityInputID"), static_cast<int32>(GGAbilityInputID::Confirm), static_cast<int32>(GGAbilityInputID::Cancel)));
	}
}

void AGGPlayerCharacter::PossessedBy(AController * NewController)
{
	Super::PossessedBy(NewController);

	AGGPlayerState * PS = GetPlayerState<AGGPlayerState>();

	if (PS)
	{
		// Set the ASC on the Server. Clients do this in OnRep_PlayerState()
		AbilitySystemComponent = Cast<UGGAbilitySystemComponent>(PS->GetAbilitySystemComponent());

		// AI won't have PlayerControllers so we can init again here just to be sure. No harm in initiating twice for heroes that have PlayerControllers.
		PS->GetAbilitySystemComponent()->InitAbilityActorInfo(PS, this);

	//	// Set the AttributeSetBase for convenience attribute functions
	//	AttributeSetBase = PS->GetAttributeSetBase();
	//
	//	// If we handle players disconnecting and rejoining in the future, we'll have to change this so that possession from rejoining doesn't reset attributes.
	//	// For now assume possession = spawn/respawn.
	//	InitializeAttributes();
	//
	//	AddStartupEffects();

		AddCharacterAbilities();
	}
}

void AGGPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	Projectile = GetWorld()->SpawnActor<AGGProjectile>(ProjectileType, KunaiSpot->GetComponentLocation(), FRotator::ZeroRotator);
	
	FAttachmentTransformRules rules(EAttachmentRule::SnapToTarget, EAttachmentRule::KeepRelative, EAttachmentRule::KeepRelative, false);
	Cast<AActor>(Projectile)->AttachToComponent(KunaiSpot, rules);
}

void AGGPlayerCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	AGGPlayerState * PS = GetPlayerState<AGGPlayerState>();
	if (PS)
	{
		// Set the ASC for clients. Server does this in PossessedBy.
		AbilitySystemComponent = Cast<UGGAbilitySystemComponent>(PS->GetAbilitySystemComponent());

		// Init ASC Actor Info for clients. Server will init its ASC when it possesses a new Actor.
		AbilitySystemComponent->InitAbilityActorInfo(PS, this);

		// Bind player input to the AbilitySystemComponent. Also called in SetupPlayerInputComponent because of a potential race condition.
		//BindASCInput();

		// Set the AttributeSetBase for convenience attribute functions
		//AttributeSetBase = PS->GetAttributeSetBase();

		// If we handle players disconnecting and rejoining in the future, we'll have to change this so that posession from rejoining doesn't reset attributes.
		// For now assume possession = spawn/respawn.
		//InitializeAttributes();

		//AGDPlayerController* PC = Cast<AGDPlayerController>(GetController());
		//if (PC)
		//{
		//	PC->CreateHUD();
		//}

		// Simulated on proxies don't have their PlayerStates yet when BeginPlay is called so we call it again here
		//InitializeFloatingStatusBar();


		// Respawn specific things that won't affect first possession.

		// Forcibly set the DeadTag count to 0
		//AbilitySystemComponent->SetTagMapCount(DeadTag, 0);

		// Set Health/Mana/Stamina to their max. This is only necessary for *Respawn*.
		//SetHealth(GetMaxHealth());
		//SetMana(GetMaxMana());
		//SetStamina(GetMaxStamina());
	}
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