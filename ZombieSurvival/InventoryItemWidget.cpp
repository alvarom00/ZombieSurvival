// Fill out your copyright notice in the Description page of Project Settings.

#include "InventoryItemWidget.h"
#include "Blueprint/DragDropOperation.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Item.h"

void UInventoryItemWidget::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	if (!Item) return;

	// Crear operación de Drag & Drop
	UDragDropOperation* DragOperation = NewObject<UDragDropOperation>();
	DragOperation->Payload = this; // Pasamos este widget como el payload
	DragOperation->DefaultDragVisual = this; // Opcional: Mostrar el mismo widget mientras se arrastra

	// Iniciar Drag & Drop
	OutOperation = DragOperation;
}