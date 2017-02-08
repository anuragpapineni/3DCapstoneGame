// Fill out your copyright notice in the Description page of Project Settings.

#include "TwoWizards.h"
#include "ArtifactPlace.h"
#include "Artifact.h"


// Sets default values
AArtifactPlace::AArtifactPlace()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	TBox->bGenerateOverlapEvents = true;
	TBox->OnComponentBeginOverlap.AddDynamic(this, &AArtifactPlace::TriggerEnter);
	TBox->OnComponentEndOverlap.AddDynamic(this, &AArtifactPlace::TriggerExit);

	RootComponent = TBox;

	TBoxMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Box Mesh"));
	TBoxMesh->AttachTo(RootComponent); //Look into AttachRootComponentTo
}

// Called when the game starts or when spawned
void AArtifactPlace::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AArtifactPlace::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	if (playerController != NULL)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Has artifact? %s"), playerController->hasArtifact ? TEXT("has artifact true") : TEXT("has artifact false")));
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Has artifact? %s"), playerController->bIsPuttingDown ? TEXT("is put down true") : TEXT("is put down false")));
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Has artifact? %s"), bPlayerIsWithinRange ? TEXT("is in range true") : TEXT("is in range false")));


		if (playerController->hasArtifact && playerController->bIsPuttingDown && bPlayerIsWithinRange)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Passed IF checks"));
			PutDown();
		}
	}
}

void AArtifactPlace::PutDown()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("Put Down Method called"));
	AArtifact* artifact = playerController->artifact;

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Artifact determined"));

	artifact->DetachFromActor(FDetachmentTransformRules::KeepRelativeTransform);
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Artifact detached"));
	artifact->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Artifact attached"));

	playerController->hasArtifact = false;
}

void AArtifactPlace::GetPlayer(AActor * player)
{
	playerController = Cast<ATwoWizardsCharacter>(player);
}

void AArtifactPlace::TriggerEnter(UPrimitiveComponent* overlappedComp, AActor* OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	bPlayerIsWithinRange = true;
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Press 'E' to put down the artifact!"));
	GetPlayer(OtherActor);
}

void AArtifactPlace::TriggerExit(UPrimitiveComponent* overlappedComp, AActor* OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	bPlayerIsWithinRange = false;
}
