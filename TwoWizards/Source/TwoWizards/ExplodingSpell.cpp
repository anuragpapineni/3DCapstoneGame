// Fill out your copyright notice in the Description page of Project Settings.

#include "TwoWizards.h"
#include "ExplodingSpell.h"

#include "TwoWizardsCharacter.h"
#include "Enemy.h"
#include "GameController.h"

AExplodingSpell::AExplodingSpell()
{
	// Use a sphere as a simple collision representation
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	((USphereComponent*)CollisionComp)->InitSphereRadius(5.0f);

	CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
	CollisionComp->OnComponentHit.AddDynamic(this, &AExplodingSpell::OnHit);		// set up a notification for when this component hits something blocking

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
	
	InitialLifeSpan = 0.8f;

}


void AExplodingSpell::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{

	//if (OtherActor->IsA(AEnemy::StaticClass()))
	//{
	//	AEnemy* enemy = ((AEnemy*)OtherActor);
	//	enemy->health -= damage;
	//	enemy->lastElement = element;
	//	if (((AEnemy*)OtherActor)->health <= 0)
	//	{
	//		AGameController::DisableActor(OtherActor);
	//	}
	//}
	if (ExplosionClass != NULL)
	{
		UWorld* const World = GetWorld();
		if (World != NULL)
		{
			const FRotator SpawnRotation = this->GetActorRotation();
			// MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
			const FVector SpawnLocation = Hit.Location;

			//Set Spawn Collision Handling Override
			FActorSpawnParameters ActorSpawnParams;
			ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

			// spawn the projectile at the muzzle
			World->SpawnActor<AExplosion>(ExplosionClass, SpawnLocation, SpawnRotation, ActorSpawnParams);

		}
	}
	Destroy();
}

void AExplodingSpell::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}

void AExplodingSpell::BeginPlay() {
	Super::BeginPlay();
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATwoWizardsCharacter::StaticClass(), FoundActors);
	CollisionComp->MoveIgnoreActors.Add(FoundActors.Pop());
	CollisionComp->MoveIgnoreActors.Add(FoundActors.Pop());
}


void AExplodingSpell::LifeSpanExpired() {
	if (ExplosionClass != NULL)
	{
		UWorld* const World = GetWorld();
		if (World != NULL)
		{
			const FRotator SpawnRotation = this->GetActorRotation();
			// MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
			const FVector SpawnLocation = GetActorLocation();

			//Set Spawn Collision Handling Override
			FActorSpawnParameters ActorSpawnParams;
			ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

			// spawn the projectile at the muzzle
			World->SpawnActor<AExplosion>(ExplosionClass, SpawnLocation, SpawnRotation, ActorSpawnParams);

		}
	}
	Super::LifeSpanExpired();
}

