// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Item.h"
#include "ClothingItem.h"
#include "InventoryComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInventoryUpdated);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ZOMBIESURVIVAL_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties

	UInventoryComponent();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	TArray<TSubclassOf<UItem>> ProximityItems;

	/** Ítem en manos */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	TSubclassOf<UItem> ItemInHands;

	/** Prendas equipadas */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	TArray<UClothingItem*> EquippedClothing;

	/** Radio de escaneo de proximidad */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	float ProximityScanRadius = 200.0f;

	/** Delegate que se activa cuando el inventario cambia */
	UPROPERTY(BlueprintAssignable, Category = "Inventory")
	FOnInventoryUpdated OnInventoryUpdated;

	bool AddItemToInventory(TSubclassOf<UItem> Item, int32 SlotIndex, UClothingItem* TargetClothing = nullptr);
	bool RemoveItemFromInventory(TSubclassOf<UItem> Item);
};
