// Fill out your copyright notice in the Description page of Project Settings.

#include "ClothingItem.h"
#include "Item.h" // Include the header file for UItem

UClothingItem::UClothingItem()
{
    // Ejemplo de pantalón con un contenedor de 5x4 casillas
    PocketContainer.Width = 5;
    PocketContainer.Height = 4;
    PocketContainer.InitializeSlots();
}

// Agrega un ítem a los bolsillos si hay espacio suficiente
bool UClothingItem::AddItemToPockets(TSubclassOf<UItem> Item)
{
    if (!Item || !PocketContainer.HasSpaceForItem(Item.GetDefaultObject()->ItemSize))
    {
        return false; // No hay espacio suficiente
    }

    // Encuentra casillas vacías y coloca el ítem en ellas
    int32 NeededSlots = Item.GetDefaultObject()->ItemSize;
    for (FPocketSlot& Slot : PocketContainer.Slots)
    {
        if (Slot.IsEmpty())
        {
            Slot.StoredItem = Item;
            NeededSlots--;

            if (NeededSlots == 0) return true;
        }
    }
    return false;
}

// Remueve un ítem de los bolsillos
bool UClothingItem::RemoveItemFromPockets(TSubclassOf<UItem> Item)
{
    for (FPocketSlot& Slot : PocketContainer.Slots)
    {
        if (Slot.StoredItem == Item)
        {
            Slot.StoredItem = nullptr;
        }
    }
    return true;
}