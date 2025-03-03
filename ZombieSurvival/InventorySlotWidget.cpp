// Fill out your copyright notice in the Description page of Project Settings.


#include "InventorySlotWidget.h"
#include "InventoryItemWidget.h"
#include "Blueprint/DragDropOperation.h"
#include "Item.h"
#include "ClothingItem.h"
#include "Components/UniformGridPanel.h"

void UInventorySlotWidget::NativeConstruct()
{
	Super::NativeConstruct();

	SetVisibility(ESlateVisibility::Visible);
}

void UInventorySlotWidget::SetItemName(const FString& Name)
{
	{
		if (ItemNameTextBlock) // Verifica que el TextBlock sea válido
		{
			ItemNameTextBlock->SetText(FText::FromString(Name));
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("ItemNameTextBlock is null!"));
		}
	}
}

bool UInventorySlotWidget::CanAcceptItem(UItem* InItem) const
{
	// Lógica para validar si el ítem puede colocarse aquí
	if (!InItem) return false;

	// Si este slot ya tiene un ítem, no aceptar otro
	if (Item) return false;

	// Aquí podrías validar si es una prenda de ropa, si va en manos, etc.
	// Por ahora aceptamos cualquier ítem
	return true;
}

bool UInventorySlotWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	UInventoryItemWidget* DraggedItemWidget = Cast<UInventoryItemWidget>(InOperation->Payload);
	if (!DraggedItemWidget || !DraggedItemWidget->Item) return false;

	UItem* DraggedItem = DraggedItemWidget->Item;
	if (!CanAcceptItem(DraggedItem)) return false;

	// Asignamos el ítem a este slot
	Item = DraggedItem;

	// Aquí puedes actualizar la UI para reflejar el cambio
	// (Ejemplo: cambiar la imagen del slot para mostrar el nuevo ítem)

	return true;
}