// Fill out your copyright notice in the Description page of Project Settings.


#include "TwoWizards.h"
#include "Enemy.h"
#include "GameController.h"
#include "UnrealNetwork.h"

// Sets default values
AEnemy::AEnemy()
{

    bReplicateMovement = true;
    bReplicates = true;
    bAlwaysRelevant = true;

	//SceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComp"));
	//RootComponent = SceneComp;

 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    maxHealth = 3;
    health = maxHealth;
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	if (!AGameController::instance->enemies.Contains(this))
		AGameController::instance->enemies.Add(this);
	
}

// Called every frame
void AEnemy::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
	if (health <= 0)
		AGameController::DisableActor(this);
	EnemyTick();
}

// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemy::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// Replicate to everyone
	DOREPLIFETIME(AEnemy, health);
	DOREPLIFETIME(AEnemy, maxHealth);
}