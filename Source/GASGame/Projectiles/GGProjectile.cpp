// Fill out your copyright notice in the Description page of Project Settings.


#include "GGProjectile.h"

// Sets default values
AGGProjectile::AGGProjectile(const FObjectInitializer& OI) : Super(OI)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Collider = OI.CreateDefaultSubobject<UBoxComponent>(this, TEXT("Collider"));
	SetRootComponent(Collider);

	Mesh = OI.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("Mesh"));
	Mesh->SetupAttachment(Collider);
}

// Called when the game starts or when spawned
void AGGProjectile::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AGGProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

