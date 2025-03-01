// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemActor.h"

// Sets default values
AItemActor::AItemActor()
{
	PrimaryActorTick.bCanEverTick = false;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	RootComponent = MeshComponent;

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