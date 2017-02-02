// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "TwoWizardsCharacter.h"
#include "GameFramework/Actor.h"
#include "Spell.generated.h"

UENUM()
namespace Element {
	enum Type {
		None,
		Fire,
		Water,
		Ice,
		Earth
	};
}

UCLASS()
class TWOWIZARDS_API ASpell : public AActor
{
	GENERATED_BODY()


public:	

	/* wizard who cast the spell*/
	ATwoWizardsCharacter* caster;

	/** Sound to play on cast */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		class USoundBase* CastSound;


	/** Sound to play on impact */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		class USoundBase* HitSound;


	/** Sound to play while traveling */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TEnumAsByte<Element::Type> element;

	/** cooldown */
	UPROPERTY(EditAnywhere)
		float cooldown;

	/** damage */
	UPROPERTY(EditAnywhere)
		float damage;
	// Sets default values for this actor's properties
	ASpell();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;


	
};
