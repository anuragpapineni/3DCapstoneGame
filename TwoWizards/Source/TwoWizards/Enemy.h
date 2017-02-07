// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Spell.h"
#include "GameFramework/Character.h"
#include "Enemy.generated.h"

UCLASS()
class TWOWIZARDS_API AEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemy();


	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
        TEnumAsByte<Element::Type> lastElement;

    UPROPERTY(EditAnywhere, BlueprintReadWrite) //Not sure if this is necessary?
        float health;

    UPROPERTY(EditAnywhere, BlueprintReadWrite) //Not sure if this is necessary?
        float maxHealth;
};
