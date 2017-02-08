// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "TwoWizardsCharacter.h"
#include "GameFramework/Actor.h"
#include "Enemy.h"
#include <vector>
#include "GameController.generated.h"

UCLASS()
class TWOWIZARDS_API AGameController : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGameController();

	static AGameController* instance;

	static void DisableActor(AActor* actor);

	static void EnableActor(AActor* actor);
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ATwoWizardsCharacter* player1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ATwoWizardsCharacter* player2;

	std::vector<AEnemy*> enemies;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	
	
};
