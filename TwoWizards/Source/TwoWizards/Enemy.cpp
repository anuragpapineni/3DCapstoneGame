// Fill out your copyright notice in the Description page of Project Settings.

#include "TwoWizards.h"
#include "Enemy.h"
#include "TwoWizardsProjectile.h"


#define COLLISION_ENEMY ECollisionChannel::ECC_GameTraceChannel2
#define COLLISION_ALLY ECollisionChannel::ECC_GameTraceChannel3

AEnemy::AEnemy(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionComp = ObjectInitializer.CreateDefaultSubobject<UBoxComponent>(this, TEXT("ColComp"));

	CollisionComp->OnComponentHit.AddDynamic(this, &AEnemy::OnHit);

	CollisionComp->BodyInstance.SetCollisionProfileName("enemy");
	CollisionComp->BodyInstance.SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics, true);

	CollisionComp->SetCollisionObjectType(COLLISION_ENEMY);
	CollisionComp->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	CollisionComp->SetCollisionResponseToChannel(COLLISION_ENEMY, ECollisionResponse::ECR_Ignore);
	CollisionComp->SetCollisionResponseToChannel(COLLISION_ALLY, ECollisionResponse::ECR_Block);

	RootComponent = CollisionComp;

	EnemyMesh = CreateDefaultSubobject <UStaticMeshComponent>(TEXT("EnemyMesh"));
	EnemyMesh->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepWorldTransform);

	health = 3;
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	CollisionComp->BodyInstance.SetCollisionProfileName("enemy");
	CollisionComp->BodyInstance.SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics, true);

	CollisionComp->SetCollisionObjectType(COLLISION_ENEMY);
	CollisionComp->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	CollisionComp->SetCollisionResponseToChannel(COLLISION_ENEMY, ECollisionResponse::ECR_Ignore);
	CollisionComp->SetCollisionResponseToChannel(COLLISION_ALLY, ECollisionResponse::ECR_Block);
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void AEnemy::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{

	if (OtherActor->IsA(ATwoWizardsProjectile::StaticClass()))
	{
		health--;
		if (health <= 0)
		{
			Destroy();
		}
	}

}
