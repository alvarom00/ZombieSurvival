// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryItemWidget.generated.h"

/**
 * 
 */

class UItem;


UCLASS()
class ZOMBIESURVIVAL_API UInventoryItemWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	/** �tem representado en este widget */
	UPROPERTY(BlueprintReadWrite, Category = "Inventory")
	UItem* Item;

	/** Cambia el estado de ocupado del widget */
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void SetOccupied(bool bIsOccupied);

	/** Verifica si el widget est� ocupado */
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool IsOccupied() const { return bOccupied; }

protected:
	/** Detecta cuando se empieza a arrastrar el �tem */
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;

private:
	UItem* ItemData;

	    /** Indica si el slot est� ocupado */
    UPROPERTY()
    bool bOccupied = false;
};
