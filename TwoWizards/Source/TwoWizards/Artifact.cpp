// Fill out your copyright notice in the Description page of Project Settings.

#include "TwoWizards.h"
#include "Artifact.h"
#include "Engine.h"

// Sets default values
AArtifact::AArtifact()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	TBox->bGenerateOverlapEvents = true;
	TBox->OnComponentBeginOverlap.AddDynamic(this, &AArtifact::TriggerEnter);
	TBox->OnComponentEndOverlap.AddDynamic(this, &AArtifact::TriggerExit);
	
	RootComponent = TBox;

	TBoxMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Box Mesh"));
	TBoxMesh->AttachTo(RootComponent);
}

// Called when the game starts or when spawned
void AArtifact::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AArtifact::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	if (playerController != NULL) 
	{
		if (playerController->bIsPickingUp && bItemIsWithinRange)
		{
			Pickup();
		}
	}
}

void AArtifact::Pickup()
{
	playerController->hasArtifact = true;
	this->AttachToActor(playerController, FAttachmentTransformRules::KeepRelativeTransform);
	playerController->artifact = this;
	
	//this->AttachToActor(playerController, FAttachmentTransformRules::KeepRelativeTransform, playerController->GetMesh()->GetSocketBoneName("thumb_03_l"));
	//this->AttachRootComponentToActor(playerController);
}

void AArtifact::PlaceOnHolder()
{
	//this->DetachRootComponentFromParent();
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Place On Holder Method Called"));
}

void AArtifact::GetPlayer(AActor * player)
{
	playerController = Cast<ATwoWizardsCharacter>(player);
}

void AArtifact::TriggerEnter(UPrimitiveComponent* overlappedComp, AActor* OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	bItemIsWithinRange = true;
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Press 'E' to pick up the artifact!"));
	GetPlayer(OtherActor);
}

void AArtifact::TriggerExit(UPrimitiveComponent* overlappedComp, AActor* OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	bItemIsWithinRange = false;
}

