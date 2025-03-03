// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemActor.h"
#include "Components/BoxComponent.h"

// Sets default values
AItemActor::AItemActor()
{
	PrimaryActorTick.bCanEverTick = false;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	RootComponent = MeshComponent;

    MeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    MeshComponent->SetCollisionObjectType(ECC_PhysicsBody);
    MeshComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
    MeshComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

    CollisionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionComponent"));
    CollisionComponent->SetupAttachment(RootComponent);
    CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    CollisionComponent->SetCollisionObjectType(ECC_WorldDynamic);
    CollisionComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
    CollisionComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
    CollisionComponent->SetGenerateOverlapEvents(true);
    CollisionComponent->SetBoxExtent(FVector(50.f, 50.f, 50.f));

	ItemData = nullptr;
}

// Called when the game starts or when spawned
void AItemActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void AItemActor::SetItemData(TSubclassOf<UItem>NewItemData)
{
    if (NewItemData)
    {
        ItemData = NewItemData;
    }
}

TSubclassOf<UItem>AItemActor::GetItemData() const
{
    return ItemData;
}

void AItemActor::PickupItem()
{
    if (ItemData)
    {
        Destroy(); // Destruye el objeto en el mundo después de recogerlo
    }
}