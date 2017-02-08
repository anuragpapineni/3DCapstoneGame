// Fill out your copyright notice in the Description page of Project Settings.

#include "TwoWizards.h"
#include "GameController.h"
#include "UnrealNetwork.h"

AGameController* AGameController::instance = nullptr;

// Sets default values
AGameController::AGameController()
{
	bReplicateMovement = true;
	bReplicates = true;
	bAlwaysRelevant = true;
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	AGameController::instance = this;
}

void AGameController::DisableActor(AActor * actor)
{
	actor->SetActorHiddenInGame(true);
	actor->SetActorEnableCollision(false);
	actor->SetActorTickInterval(FLT_MAX);
}


void AGameController::EnableActor(AActor * actor)
{
	actor->SetActorHiddenInGame(false);
	actor->SetActorEnableCollision(true);
	actor->SetActorTickInterval(0);
}

// Called when the game starts or when spawned
void AGameController::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AGameController::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
}

void AGameController::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// Replicate to everyone
	DOREPLIFETIME(AGameController, enemies);
}