// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryWidget.h"
#include "Components/UniformGridPanel.h"
#include "Components/VerticalBox.h"
#include "ZombieSurvivalCharacter.h"
#include "InventoryItemWidget.h"
#include "InventorySlotWidget.h"

void UInventoryWidget::NativeConstruct()
{
    Super::NativeConstruct();

    AZombieSurvivalCharacter* PlayerCharacter = Cast<AZombieSurvivalCharacter>(GetOwningPlayerPawn());
    if (PlayerCharacter)
    {
        SetInventoryComponent(PlayerCharacter->InventoryComponent);
    }

    // Actualizar UI al inicio
    if (InventoryComponent)
    {
        UpdateInventoryUI();
    }
}

void UInventoryWidget::UpdateInventoryUI()
{
    // Aquí se actualizarán los widgets de los ítems en el inventario
}

void UInventoryWidget::SetInventoryComponent(UInventoryComponent* NewInventoryComponent)
{
    InventoryComponent = NewInventoryComponent;

    if (InventoryComponent)
    {
        // Suscribir al evento para actualizar la UI cuando cambie el inventario
        InventoryComponent->OnInventoryUpdated.AddDynamic(this, &UInventoryWidget::UpdateInventoryUI);
    }
}

void UInventoryWidget::UpdateProximityUI(const TArray<TSubclassOf<UItem>>& NearbyItems)
{
    if (!ProximitySection) return;

    ProximitySection->ClearChildren(); // Limpiar la lista actual

    for (TSubclassOf<UItem> ItemClass : NearbyItems)
    {
        if (ItemClass)
        {
            // Crear una instancia del ítem para obtener el nombre
            UItem* ItemInstance = ItemClass->GetDefaultObject<UItem>();
            if (ItemInstance)
            {
                // Accede al nombre del ítem
                FName ItemName = ItemInstance->ItemName;

                // Crear un nuevo widget para el ítem
                UInventorySlotWidget* SlotWidget = CreateWidget<UInventorySlotWidget>(this, SlotWidgetClass);

                if (SlotWidget)
                {
                    // Configurar el nombre del ítem
                    SlotWidget->SetItemName(ItemName.ToString());

                    // Añadir el widget a la lista
                    ProximitySection->AddChild(SlotWidget);
                }
            }
        }
    }
}

void UInventoryWidget::ClearProximityUI()
{
    if (ProximitySection) // Asegurar que el widget de proximidad existe
    {
        ProximitySection->ClearChildren(); // Elimina los widgets viejos
    }
}