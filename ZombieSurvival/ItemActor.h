// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Item.h"
#include "ItemActor.generated.h"

UCLASS()
class ZOMBIESURVIVAL_API AItemActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItemActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UStaticMeshComponent* MeshComponent;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    TSubclassOf<UItem>ItemData; // Información del ítem en el inventario

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    UItem* Item;

    UFUNCTION(BlueprintCallable, Category = "Item")
    void SetItemData(TSubclassOf<UItem>NewItemData);

    UFUNCTION(BlueprintCallable, Category = "Item")
    TSubclassOf<UItem>GetItemData() const;

    UFUNCTION(BlueprintCallable, Category = "Item")
    void PickupItem();

};
