// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryWidget.h"
#include "Components/UniformGridPanel.h"
#include "Components/UniformGridSlot.h"
#include "Components/VerticalBox.h"
#include "ZombieSurvivalCharacter.h"
#include "InventoryItemWidget.h"
#include "InventorySlotWidget.h"
#include "Components/SizeBox.h"
#include "PocketSlotWidget.h"

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

void UInventoryWidget::UpdateProximityUI(const TArray<AItemActor*>& NearbyItems)
{
    if (!ProximitySection) return;

    ProximitySection->ClearChildren(); // Limpiar la lista antes de actualizar

    for (AItemActor* ItemActor : NearbyItems)
    {
        if (ItemActor && ItemActor->ItemData) // Verificar que el ítem sea válido
        {
            UItem* ItemInstance = ItemActor->ItemData->GetDefaultObject<UItem>(); // Obtener la instancia del ítem
            FName ItemName = ItemInstance->ItemName; // Obtener el nombre del ítem

            // Crear un nuevo widget para el ítem
            UInventorySlotWidget* SlotWidget = CreateWidget<UInventorySlotWidget>(this, SlotWidgetClass);
            if (SlotWidget)
            {
                // Configurar el nombre del ítem
                SlotWidget->SetItemName(ItemName.ToString());

                // Agregar el SlotWidget a la sección de proximidad
                ProximitySection->AddChild(SlotWidget);

                // Verificar si es ropa con bolsillos
                UClothingItem* ClothingItem = Cast<UClothingItem>(ItemInstance);
                if (ClothingItem && SlotWidget->PocketGrid)
                {
                    SlotWidget->PocketGrid->ClearChildren();
                    SlotWidget->PocketGrid->SetVisibility(ESlateVisibility::Visible);

                    // Crear la cuadrícula de bolsillos
                    for (int32 Row = 0; Row < ClothingItem->PocketContainer.Height; ++Row)
                    {
                        for (int32 Col = 0; Col < ClothingItem->PocketContainer.Width; ++Col)
                        {
                            if (PocketSlotWidgetClass)
                            {
                                UPocketSlotWidget* PocketSlot = CreateWidget<UPocketSlotWidget>(this, PocketSlotWidgetClass);
                                if (PocketSlot)
                                {
                                    UUniformGridSlot* GridSlot = SlotWidget->PocketGrid->AddChildToUniformGrid(PocketSlot, Row, Col);
                                }
                            }
                        }
                    }
                }
                else if (SlotWidget->PocketGrid)
                {
                    SlotWidget->PocketGrid->SetVisibility(ESlateVisibility::Collapsed);
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