// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "ClothingItem.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType)
struct FPocketSlot
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, Category = "Pocket")
    TSubclassOf<UItem> StoredItem = nullptr; // Ítem almacenado en esta casilla

    bool IsEmpty() const { return StoredItem == nullptr; }
};

/**
 * Estructura que representa el contenedor de bolsillos de una prenda.
 */
USTRUCT(BlueprintType)
struct FPocketContainer
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pocket")
    int32 Width;  // Ancho del contenedor en slots

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pocket")
    int32 Height; // Alto del contenedor en slots

    UPROPERTY(BlueprintReadWrite, Category = "Pocket")
    TArray<FPocketSlot> Slots; // Casillas disponibles dentro del contenedor

    // Inicializa la cantidad de slots según el tamaño definido
    void InitializeSlots()
    {
        Slots.SetNum(Width * Height);
    }

    // Verifica si hay suficiente espacio para un ítem de cierto tamaño
    bool HasSpaceForItem(int32 ItemSize) const
    {
        int32 EmptySlots = 0;
        for (const FPocketSlot& Slot : Slots)
        {
            if (Slot.IsEmpty())
            {
                EmptySlots++;
                if (EmptySlots >= ItemSize) return true;
            }
        }
        return false;
    }
};

UCLASS()
class ZOMBIESURVIVAL_API UClothingItem : public UItem
{
	GENERATED_BODY()
	
public:
    UClothingItem();

    // Contenedor de bolsillos de la prenda
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Clothing")
    FPocketContainer PocketContainer;

    // Agrega un ítem a los bolsillos de esta prenda
    bool AddItemToPockets(TSubclassOf<UItem> Item);

    // Remueve un ítem de los bolsillos
    bool RemoveItemFromPockets(TSubclassOf<UItem> Item);
};