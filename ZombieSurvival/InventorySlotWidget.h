// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
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

	virtual void NativeConstruct() override;
	/** Ítem actual en este slot */
	UPROPERTY(BlueprintReadWrite, Category = "Inventory")
	UItem* Item;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* ItemNameTextBlock;

	UPROPERTY(meta = (BindWidget))
	class UUniformGridPanel* PocketGrid;

	virtual void SetItemName(const FString& Name);

	/** Verifica si el ítem puede colocarse en este slot */
	bool CanAcceptItem(UItem* InItem) const;


protected:
	/** Maneja el evento de soltar un ítem en este slot */
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
};
