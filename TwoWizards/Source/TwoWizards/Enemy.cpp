// Fill out your copyright notice in the Description page of Project Settings.


#include "TwoWizards.h"
#include "Enemy.h"
#include "GameController.h"

// Sets default values
AEnemy::AEnemy()
{

    bReplicateMovement = true;
    bReplicates = true;
    bAlwaysRelevant = true;

 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    maxHealth = 3;
    health = maxHealth;
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
    AGameController::instance->enemies.push_back(this);
	
}

// Called every frame
void AEnemy::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
	EnemyTick();

}

// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

