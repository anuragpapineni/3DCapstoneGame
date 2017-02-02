// Fill out your copyright notice in the Description page of Project Settings.

#include "TwoWizards.h"
#include "Spell.h"
#include "Enemy.h"
#include "GameController.h"

// Sets default values
ASpell::ASpell()
{
	bReplicateMovement = true;
	bReplicates = true;
	bAlwaysRelevant = true;
	element = Element::None;
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	// Die after 3 seconds by default
	InitialLifeSpan = 3.0f;

}

// Called when the game starts or when spawned
void ASpell::BeginPlay()
{
	Super::BeginPlay();
	UGameplayStatics::PlaySoundAtLocation(this, CastSound, GetActorLocation());
	
}

// Called every frame
void ASpell::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}
