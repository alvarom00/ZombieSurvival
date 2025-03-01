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
    // Aqu� se actualizar�n los widgets de los �tems en el inventario
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
    if (!ProximitySection || !ItemWidgetClass) return;

    ProximitySection->ClearChildren(); // Limpia la secci�n antes de actualizar

    for (TSubclassOf<UItem> Item : NearbyItems)
    {
        if (!Item) continue;

        UInventoryItemWidget* ItemWidget = CreateWidget<UInventoryItemWidget>(this, ItemWidgetClass);
        if (ItemWidget)
        {

            GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Item In Proximity: %d"));
            ItemWidget->SetItemData(Item); // Aseg�rate de tener esta funci�n en tu `UInventoryItemWidget`
            ProximitySection->AddChildToUniformGrid(ItemWidget);

        }
    }
}