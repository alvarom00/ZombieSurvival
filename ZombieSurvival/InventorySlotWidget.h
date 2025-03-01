// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventorySlotWidget.generated.h"

/**
 * 
 */

class UItem;
class UInventoryItemWidget;

UCLASS()
class ZOMBIESURVIVAL_API UInventorySlotWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	/** �tem actual en este slot */
	UPROPERTY(BlueprintReadWrite, Category = "Inventory")
	UItem* Item;

	/** Verifica si el �tem puede colocarse en este slot */
	bool CanAcceptItem(UItem* InItem) const;

protected:
	/** Maneja el evento de soltar un �tem en este slot */
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
};
