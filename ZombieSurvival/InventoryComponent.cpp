// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryComponent.h"

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	ItemInHands = nullptr;
}

bool UInventoryComponent::AddItemToInventory(TSubclassOf<UItem> Item, int32 SlotIndex, UClothingItem* TargetClothing)
{
    if (!Item) return false;

    bool bAdded = false;

    // Si se arrastra a las manos
    if (!TargetClothing && SlotIndex == -1)
    {
        if (!ItemInHands)
        {
            ItemInHands = Item;
            bAdded = true;
        }
    }
    // Si se arrastra a una prenda
    else if (TargetClothing && SlotIndex >= 0)
    {
        if (TargetClothing->PocketContainer.Slots.IsValidIndex(SlotIndex) &&
            TargetClothing->PocketContainer.Slots[SlotIndex].StoredItem == nullptr)
        {
            TargetClothing->PocketContainer.Slots[SlotIndex].StoredItem = Item;
            bAdded = true;
        }
    }

    // Si se agregó con éxito, notificamos a la UI
    if (bAdded)
    {
        OnInventoryUpdated.Broadcast();
    }

    return bAdded;
}

bool UInventoryComponent::RemoveItemFromInventory(TSubclassOf<UItem> Item)
{
    if (!Item) return false;

    bool bRemoved = false;

    // Si el ítem está en las manos, lo eliminamos
    if (Item == ItemInHands)
    {
        ItemInHands = nullptr;
        bRemoved = true;
    }
    // Revisamos en cada prenda equipada
    else
    {
        for (UClothingItem* Clothing : EquippedClothing)
        {
            for (FPocketSlot& Slot : Clothing->PocketContainer.Slots)
            {
                if (Slot.StoredItem == Item)
                {
                    Slot.StoredItem = nullptr;
                    bRemoved = true;
                    break;
                }
            }
            if (bRemoved) break;
        }
    }

    // Si se eliminó con éxito, notificamos a la UI
    if (bRemoved)
    {
        OnInventoryUpdated.Broadcast();
    }

    return bRemoved;
}
