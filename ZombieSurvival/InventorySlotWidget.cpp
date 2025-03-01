// Fill out your copyright notice in the Description page of Project Settings.


#include "InventorySlotWidget.h"
#include "InventoryItemWidget.h"
#include "Blueprint/DragDropOperation.h"

bool UInventorySlotWidget::CanAcceptItem(UItem* InItem) const
{
	// L�gica para validar si el �tem puede colocarse aqu�
	if (!InItem) return false;

	// Si este slot ya tiene un �tem, no aceptar otro
	if (Item) return false;

	// Aqu� podr�as validar si es una prenda de ropa, si va en manos, etc.
	// Por ahora aceptamos cualquier �tem
	return true;
}

bool UInventorySlotWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	UInventoryItemWidget* DraggedItemWidget = Cast<UInventoryItemWidget>(InOperation->Payload);
	if (!DraggedItemWidget || !DraggedItemWidget->Item) return false;

	UItem* DraggedItem = DraggedItemWidget->Item;
	if (!CanAcceptItem(DraggedItem)) return false;

	// Asignamos el �tem a este slot
	Item = DraggedItem;

	// Aqu� puedes actualizar la UI para reflejar el cambio
	// (Ejemplo: cambiar la imagen del slot para mostrar el nuevo �tem)

	return true;
}