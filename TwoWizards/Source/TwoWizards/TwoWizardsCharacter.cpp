// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "TwoWizards.h"
#include "TwoWizardsCharacter.h"
#include "TwoWizardsProjectile.h"
#include "Spell.h"
#include "GameController.h"
#include "Animation/AnimInstance.h"
#include "GameFramework/InputSettings.h"
#include "Kismet/HeadMountedDisplayFunctionLibrary.h"
#include "MotionControllerComponent.h"
#include "UnrealNetwork.h"

DEFINE_LOG_CATEGORY_STATIC(LogFPChar, Warning, All);

//////////////////////////////////////////////////////////////////////////
// ATwoWizardsCharacter

ATwoWizardsCharacter::ATwoWizardsCharacter()
{

	PrimaryActorTick.bCanEverTick = true;

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->RelativeLocation = FVector(-39.56f, 1.75f, 64.f); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	Mesh1P->RelativeRotation = FRotator(1.9f, -19.19f, 5.2f);
	Mesh1P->RelativeLocation = FVector(-0.5f, -4.4f, -155.7f);

	// Create a gun mesh component
	FP_Gun = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FP_Gun"));
	FP_Gun->SetOnlyOwnerSee(true);			// only the owning player will see this mesh
	FP_Gun->bCastDynamicShadow = false;
	FP_Gun->CastShadow = false;
	// FP_Gun->SetupAttachment(Mesh1P, TEXT("GripPoint"));
	FP_Gun->SetupAttachment(RootComponent);

	FP_MuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleLocation"));
	FP_MuzzleLocation->SetupAttachment(FP_Gun);
	FP_MuzzleLocation->SetRelativeLocation(FVector(0.2f, 48.4f, -10.6f));

	// Default offset from the character location for projectiles to spawn
	GunOffset = FVector(100.0f, 0.0f, 10.0f);

    health = 10;

	// Note: The ProjectileClass and the skeletal mesh/anim blueprints for Mesh1P, FP_Gun, and VR_Gun 
	// are set in the derived blueprint asset named MyCharacter to avoid direct content references in C++.

}

// Called every frame
void ATwoWizardsCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	Cooldown0 -= DeltaTime;
	Cooldown1 -= DeltaTime;
	Cooldown2 -= DeltaTime;
	Cooldown3 -= DeltaTime;
	Cooldown4 -= DeltaTime;
}

void ATwoWizardsCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	if (GetNetMode() != NM_Client) {
		if (!AGameController::instance->player1)
			AGameController::instance->player1 = this;
	}
	else {
		if (!AGameController::instance->player2)
			AGameController::instance->player2 = this;
	}

	//Attach gun mesh component to Skeleton, doing it here because the skeleton is not yet created in the constructor
	FP_Gun->AttachToComponent(Mesh1P, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("GripPoint"));


}

void ATwoWizardsCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> & OutLifetimeProps) const{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ATwoWizardsCharacter, Task);

}

//////////////////////////////////////////////////////////////////////////
// Input

void ATwoWizardsCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// set up gameplay key bindings
	check(PlayerInputComponent);

	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &ATwoWizardsCharacter::DoFire);


	PlayerInputComponent->BindAction("Spell1", IE_Pressed, this, &ATwoWizardsCharacter::DoSpell1);
	PlayerInputComponent->BindAction("Spell2", IE_Pressed, this, &ATwoWizardsCharacter::DoSpell2);
	PlayerInputComponent->BindAction("Spell3", IE_Pressed, this, &ATwoWizardsCharacter::DoSpell3);
	PlayerInputComponent->BindAction("Spell4", IE_Pressed, this, &ATwoWizardsCharacter::DoSpell4);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAxis("MoveForward", this, &ATwoWizardsCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ATwoWizardsCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &ATwoWizardsCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &ATwoWizardsCharacter::LookUpAtRate);
}


void ATwoWizardsCharacter::PerformTask(ETaskEnum::Type NewTask)
{
	Task = NewTask;
	if (GetNetMode() == NM_Client) {
		OnRep_Task();
		ServerPerformTask(NewTask);
		return;
	}
	OnRep_Task();

}

void ATwoWizardsCharacter::ServerPerformTask_Implementation(ETaskEnum::Type NewTask) {
	PerformTask(NewTask);
}

bool ATwoWizardsCharacter::ServerPerformTask_Validate(ETaskEnum::Type NewTask) {
	return true;
}

void ATwoWizardsCharacter::ClientPerformTask_Implementation(ETaskEnum::Type NewTask) {
	Task = NewTask;
	OnRep_Task();
}

bool ATwoWizardsCharacter::ClientPerformTask_Validate(ETaskEnum::Type NewTask) {
	return true;
}

void ATwoWizardsCharacter::DoFire() {
	PerformTask(ETaskEnum::Fire);
}

void ATwoWizardsCharacter::DoSpell1() {
	PerformTask(ETaskEnum::Spell1);
}

void ATwoWizardsCharacter::DoSpell2() {
	PerformTask(ETaskEnum::Spell2);
}

void ATwoWizardsCharacter::DoSpell3() {
	PerformTask(ETaskEnum::Spell3);
}

void ATwoWizardsCharacter::DoSpell4() {
	PerformTask(ETaskEnum::Spell4);
}

void ATwoWizardsCharacter::OnFire()
{
	// try and fire a projectile
	if (ProjectileClass != NULL)
	{
		UWorld* const World = GetWorld();
		if (World != NULL)
		{
			const FRotator SpawnRotation = GetControlRotation();
			// MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
			const FVector SpawnLocation = ((FP_MuzzleLocation != nullptr) ? FP_MuzzleLocation->GetComponentLocation() : GetActorLocation()) + SpawnRotation.RotateVector(GunOffset);

			//Set Spawn Collision Handling Override
			FActorSpawnParameters ActorSpawnParams;
			ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

			// spawn the projectile at the muzzle
			World->SpawnActor<ATwoWizardsProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);
			
		}
	}

	// try and play the sound if specified
	if (FireSound != NULL)
	{
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
	}

	// try and play a firing animation if specified
	if (FireAnimation != NULL)
	{
		// Get the animation object for the arms mesh
		UAnimInstance* AnimInstance = Mesh1P->GetAnimInstance();
		if (AnimInstance != NULL)
		{
			AnimInstance->Montage_Play(FireAnimation, 1.f);
		}
	}
}


void ATwoWizardsCharacter::OnSpell()
{
	if (FireAnimation != NULL)
	{
		// Get the animation object for the arms mesh
		UAnimInstance* AnimInstance = Mesh1P->GetAnimInstance();
		if (AnimInstance != NULL)
		{
			AnimInstance->Montage_Play(FireAnimation, 1.f);
		}
	}
	if (GetNetMode() == NM_Client) {
		return;
	}
	UWorld* const World = GetWorld();
	if (World != NULL)
	{
		const FRotator SpawnRotation = GetControlRotation();
		// MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
		const FVector SpawnLocation = ((FP_MuzzleLocation != nullptr) ? FP_MuzzleLocation->GetComponentLocation() : GetActorLocation()) + SpawnRotation.RotateVector(GunOffset);

		//Set Spawn Collision Handling Override
		FActorSpawnParameters ActorSpawnParams;
		//ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

		// spawn the projectile at the muzzle
		ASpell* spell;
		switch (Task) {
			case (ETaskEnum::None):
				break;
			case(ETaskEnum::Fire):
				spell = World->SpawnActor<ASpell>(Spell0, SpawnLocation, SpawnRotation, ActorSpawnParams);
				Cooldown0 = spell->cooldown;
				break;
			case(ETaskEnum::Spell1):
				spell = World->SpawnActor<ASpell>(Spell1, SpawnLocation, SpawnRotation, ActorSpawnParams);
				Cooldown1 = spell->cooldown;
				break;
			case(ETaskEnum::Spell2):
				spell = World->SpawnActor<ASpell>(Spell2, SpawnLocation, SpawnRotation, ActorSpawnParams);
				Cooldown2 = spell->cooldown;
				break;
			case(ETaskEnum::Spell3):
				spell = World->SpawnActor<ASpell>(Spell3, SpawnLocation, SpawnRotation, ActorSpawnParams);
				Cooldown3 = spell->cooldown;
				break;
			case(ETaskEnum::Spell4):
				spell = World->SpawnActor<ASpell>(Spell4, SpawnLocation, SpawnRotation, ActorSpawnParams);
				Cooldown4 = spell->cooldown;
				break;

		}

	}

}


void ATwoWizardsCharacter::MoveForward(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorForwardVector(), Value);
	}
}

void ATwoWizardsCharacter::MoveRight(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorRightVector(), Value);
	}
}

void ATwoWizardsCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ATwoWizardsCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}
void ATwoWizardsCharacter::OnRep_Task()
{
	switch (Task) {
		case (ETaskEnum::None):
			break;
		case(ETaskEnum::Fire):
			if (Cooldown0<=0)
				OnSpell();
			break;
		case(ETaskEnum::Spell1):
			if (Cooldown1 <= 0)
				OnSpell();
			break;
		case(ETaskEnum::Spell2):
			if (Cooldown2 <= 0)
				OnSpell();
			break;
		case(ETaskEnum::Spell3):
			if (Cooldown3 <= 0)
				OnSpell();
			break;
		case(ETaskEnum::Spell4):
			if (Cooldown4 <= 0)
				OnSpell();
			break;

	}
}
