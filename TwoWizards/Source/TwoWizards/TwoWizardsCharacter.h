// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/Character.h"
#include "TwoWizardsCharacter.generated.h"

class UInputComponent;

UENUM()
namespace ETaskEnum {
	enum Type {
		None,
		Fire,
		Spell1,
		Spell2,
		Spell3,
		Spell4
	};
}
UCLASS(config=Game)
class ATwoWizardsCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Pawn mesh: 1st person view (arms; seen only by self) */
	UPROPERTY(VisibleDefaultsOnly, Category=Mesh)
	class USkeletalMeshComponent* Mesh1P;

	/** Gun mesh: 1st person view (seen only by self) */
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	class USkeletalMeshComponent* FP_Gun;

	/** Location on gun mesh where projectiles should spawn. */
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	class USceneComponent* FP_MuzzleLocation;

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FirstPersonCameraComponent;

	UPROPERTY(ReplicatedUsing = OnRep_Task)
		TEnumAsByte<ETaskEnum::Type> Task;

public:
	ATwoWizardsCharacter();

	virtual void BeginPlay();

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

	/** Gun muzzle's offset from the characters location */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
	FVector GunOffset;

	/** Projectile class to spawn */
	UPROPERTY(EditDefaultsOnly, Category=Projectile)
	TSubclassOf<class ATwoWizardsProjectile> ProjectileClass;

	/** Spell 1 */
	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class ASpell> Spell1;

	/** Spell 2 */
	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class ASpell> Spell2;

	/** Spell 3 */
	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class ASpell> Spell3;

	/** Spell 4 */
	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class ASpell> Spell4;


	/** Sound to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
	class USoundBase* FireSound;

	/** AnimMontage to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	class UAnimMontage* FireAnimation;


protected:
	
	void PerformTask(ETaskEnum::Type NewTask);

	UFUNCTION(Server, Reliable, WithValidation)
		void ServerPerformTask(ETaskEnum::Type NewTask);

	UFUNCTION(NetMulticast, Reliable, WithValidation)
		void ClientPerformTask(ETaskEnum::Type NewTask);

	void DoFire();

	/** Fires a projectile. */
	void OnFire();

	/* Spell Callbacks */
	void DoSpell1();
	void DoSpell2();
	void DoSpell3();
	void DoSpell4();

	/** Fires a spell. */
	void OnSpell();

	/** Handles moving forward/backward */
	void MoveForward(float Val);

	/** Handles stafing movement, left and right */
	void MoveRight(float Val);

	/**
	 * Called via input to turn at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	// End of APawn interface


public:
	/** Returns Mesh1P subobject **/

	UFUNCTION()
		void OnRep_Task();
	FORCEINLINE class USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }
	/** Returns FirstPersonCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

};

