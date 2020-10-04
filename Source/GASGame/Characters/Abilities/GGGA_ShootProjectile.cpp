// Fill out your copyright notice in the Description page of Project Settings.


#include "GGGA_ShootProjectile.h"
#include "../../GASGame.h"
#include "../Player/GGPlayerCharacter.h"
#include "../../Projectiles/GGProjectile.h"
#include <GameFramework/ProjectileMovementComponent.h>
#include "Camera/CameraComponent.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/EngineTypes.h"
#include "CollisionQueryParams.h"

UGGGA_ShootProjectile::UGGGA_ShootProjectile()
{
	AbilityInputID = GGAbilityInputID::Ability1;
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	AbilityTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.ThrowKunai")));
}

void UGGGA_ShootProjectile::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (HasAuthorityOrPredictionKey(ActorInfo, &ActivationInfo))
	{
		if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
		{
			EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
		}

		AGGPlayerCharacter * Character = CastChecked<AGGPlayerCharacter>(ActorInfo->AvatarActor.Get());
		
		if (AGGProjectile * projectile = Character->Projectile)
		{		
			projectile->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);

			UProjectileMovementComponent * projMovComp = NewObject<UProjectileMovementComponent>(projectile, UProjectileMovementComponent::StaticClass(), NAME_None, RF_Transient);
			check(projMovComp);

			projMovComp->RegisterComponent();


			UCameraComponent * camera = Cast<UCameraComponent>(Character->GetComponentByClass(UCameraComponent::StaticClass()));
			APlayerController * controller = Cast<APlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));

			FVector startPos;
			FVector endPos;

			FVector dir;

			int viewportx;
			int viewporty;
			FVector2D viewportCenter;
			
			controller->GetViewportSize(viewportx, viewporty);
			viewportCenter = FVector2D(viewportx, viewporty) / 2.0f;
			controller->DeprojectScreenPositionToWorld(viewportCenter.X, viewportCenter.Y, startPos, dir);

			endPos = startPos + dir * 2000;

			FHitResult OutHit;

			GetWorld()->LineTraceSingleByChannel(OutHit, startPos, endPos, ECC_Visibility);

			if (OutHit.bBlockingHit)
			{
				DrawDebugLine(GetWorld(), startPos, OutHit.ImpactPoint, FColor::Green, true);

				//projMovComp->Velocity = (OutHit.ImpactPoint - projectile->GetActorLocation()).GetSafeNormal() * 2000;
				projMovComp->Velocity = (OutHit.ImpactPoint - projectile->GetActorLocation()).GetSafeNormal() * 2000;
				projMovComp->ProjectileGravityScale = 0;
				projMovComp->bRotationFollowsVelocity = true;
			}
			else
			{
				DrawDebugLine(GetWorld(), startPos, endPos, FColor::Green, true);

				projMovComp->Velocity = (endPos - projectile->GetActorLocation()).GetSafeNormal() * 2000;
				projMovComp->ProjectileGravityScale = 0;
				projMovComp->bRotationFollowsVelocity = true;
			}
			
		}
	}
}


//FVector startPos;
//FVector endPos;
//FRotator camRot;
//
//controller->GetPlayerViewPoint(startPos, camRot);
//
//FVector dir = camRot.Vector();
//
//endPos = startPos + dir * 2000;
//
//FHitResult OutHit;
//FCollisionResponseParams CollisionParams;
//
//GetWorld()->LineTraceSingleByChannel(OutHit, projectile->GetActorLocation(), endPos, ECC_Visibility);
//
//
//DrawDebugLine(GetWorld(), projectile->GetActorLocation(), endPos, FColor::Green, true);
//
//projMovComp->Velocity = dir * 2000;
//projMovComp->ProjectileGravityScale = 0;



bool UGGGA_ShootProjectile::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags /*= nullptr*/, const FGameplayTagContainer* TargetTags /*= nullptr*/, OUT FGameplayTagContainer* OptionalRelevantTags /*= nullptr*/) const
{
	return true;
}

void UGGGA_ShootProjectile::InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	CancelAbility(Handle, ActorInfo, ActivationInfo, true);
}

void UGGGA_ShootProjectile::CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility)
{
	if (ScopeLockCount > 0)
	{
		WaitingToExecute.Add(FPostLockDelegate::CreateUObject(this, &UGGGA_ShootProjectile::CancelAbility, Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility));
		return;
	}

	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);

	AGGPlayerCharacter * Character = CastChecked<AGGPlayerCharacter>(ActorInfo->AvatarActor.Get());

	if (AGGProjectile * projectile = Character->Projectile)
	{
		FAttachmentTransformRules rules(EAttachmentRule::SnapToTarget, EAttachmentRule::KeepRelative, EAttachmentRule::KeepRelative, false);
		Cast<AActor>(projectile)->AttachToComponent(Character->KunaiSpot, rules);
		projectile->SetActorRelativeLocation(FVector::ZeroVector);
		projectile->SetActorRelativeRotation(FRotator::ZeroRotator);

		UProjectileMovementComponent * projMovComp = Cast<UProjectileMovementComponent>(projectile->GetComponentByClass(UProjectileMovementComponent::StaticClass()));
		check(projMovComp);

		projMovComp->DestroyComponent();
	}
}
