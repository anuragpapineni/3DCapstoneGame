// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "TwoWizards.h"
#include "TwoWizardsProjectile.h"
#include "Enemy.h"
#include "GameController.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "TwoWizardsCharacter.h"

#define COLLISION_ENEMY ECollisionChannel::ECC_GameTraceChannel3
#define COLLISION_ALLY ECollisionChannel::ECC_GameTraceChannel2

ATwoWizardsProjectile::ATwoWizardsProjectile()
{
	bReplicateMovement = true;
	bReplicates = true;
	bAlwaysRelevant = true;
	// Use a sphere as a simple collision representation
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(5.0f);
	CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
	CollisionComp->OnComponentHit.AddDynamic(this, &ATwoWizardsProjectile::OnHit);		// set up a notification for when this component hits something blocking

																						// Players can't walk on it
	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;

	// Set as root component
	RootComponent = CollisionComp;

	CollisionComp->BodyInstance.SetCollisionProfileName("ally");
	CollisionComp->BodyInstance.SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics, true);

	CollisionComp->SetCollisionObjectType(COLLISION_ALLY);
	CollisionComp->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	CollisionComp->SetCollisionResponseToChannel(COLLISION_ALLY, ECollisionResponse::ECR_Ignore);
	CollisionComp->SetCollisionResponseToChannel(COLLISION_ENEMY, ECollisionResponse::ECR_Block);

	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 3000.f;
	ProjectileMovement->MaxSpeed = 3000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = false;
	ProjectileMovement->ProjectileGravityScale = 0.0f;

	// Die after 3 seconds by default
	InitialLifeSpan = 3.0f;
}

void ATwoWizardsProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	// Only add impulse and destroy projectile if we hit a physics
	if ((OtherActor != NULL) && (OtherActor != this) && (OtherComp != NULL) && OtherComp->IsSimulatingPhysics())
	{
		OtherComp->AddImpulseAtLocation(GetVelocity() * 100.0f, GetActorLocation());
	}

	if (OtherActor->IsA(AEnemy::StaticClass()))
	{
		((AEnemy*)OtherActor)->health--;
		if (((AEnemy*)OtherActor)->health <= 0)
		{
			AGameController::DisableActor(OtherActor);
		}
	}
	Destroy();
}