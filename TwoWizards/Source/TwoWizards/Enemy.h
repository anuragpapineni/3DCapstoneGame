// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
#include "Enemy.generated.h"

UCLASS()
class TWOWIZARDS_API AEnemy : public AActor
{
	GENERATED_UCLASS_BODY()

public:
	// Sets default values for this actor's properties
	// Aenemy();

	UPROPERTY(Category = Mesh, VisibleDefaultsOnly, BlueprintReadOnly)
		class UBoxComponent* CollisionComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
		UStaticMeshComponent* EnemyMesh;


	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaSeconds) override;

	UFUNCTION()
		void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UPROPERTY(EditAnywhere, BlueprintReadWrite) //Not sure if this is necessary?
		float health;


	UPROPERTY(EditAnywhere, BlueprintReadWrite) //Not sure if this is necessary?
		float maxHealth;
};
