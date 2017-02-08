// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Spell.h"
#include "Explosion.h"
#include "ExplodingSpell.generated.h"

/**
 * 
 */
UCLASS()
class TWOWIZARDS_API AExplodingSpell : public ASpell
{
	GENERATED_BODY()

	AExplodingSpell();

	UPROPERTY(EditAnywhere)
		float range;

	/** Sphere collision component */
	UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
		class USphereComponent* CollisionComp;
	/** Projectile movement component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
		class UProjectileMovementComponent* ProjectileMovement;

	/** called when projectile hits something */
	UFUNCTION()
		void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	/** Projectile class to spawn */
	UPROPERTY(EditAnywhere)
		TSubclassOf<class AExplosion> ExplosionClass;
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	
	
};
