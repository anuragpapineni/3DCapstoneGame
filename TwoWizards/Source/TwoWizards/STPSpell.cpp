// Fill out your copyright notice in the Description page of Project Settings.

#include "TwoWizards.h"
#include "STPSpell.h"
#include "Enemy.h"
#include "GameController.h"
#include <string>

ASTPSpell::ASTPSpell()
{
	// Use a sphere as a simple collision representation
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	((USphereComponent*)CollisionComp)->InitSphereRadius(5.0f);
	CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
	CollisionComp->OnComponentHit.AddDynamic(this, &ASTPSpell::OnHit);		// set up a notification for when this component hits something blocking
																								// Players can't walk on it
	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;
	// Set as root component
	RootComponent = CollisionComp;

	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 3000.f;
	ProjectileMovement->MaxSpeed = 3000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = false;
	ProjectileMovement->ProjectileGravityScale = 0.0f;

}



void ASTPSpell::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{

	if (OtherActor->IsA(AEnemy::StaticClass()))
	{
		AEnemy* enemy = ((AEnemy*)OtherActor);
		enemy->health -= damage;
		enemy->lastElement = element;
		if (((AEnemy*)OtherActor)->health <= 0)
		{
			AGameController::DisableActor(OtherActor);
		}
	}
	Destroy();
}

void ASTPSpell::BeginPlay() {
	Super::BeginPlay();
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATwoWizardsCharacter::StaticClass(), FoundActors);
	CollisionComp->MoveIgnoreActors.Add(FoundActors.Pop());
	CollisionComp->MoveIgnoreActors.Add(FoundActors.Pop());
}